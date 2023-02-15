cmake_minimum_required(VERSION 3.15)
project(SampleService)

if(TARGET asyncpp_grpc)
  message(STATUS "Using existing asyncpp-grpc target.")
else()
  message(STATUS "Missing asyncpp-grpc, using FetchContent to import it.")
  include(FetchContent)
  FetchContent_Declare(asyncpp_grpc
                      GIT_REPOSITORY "https://github.com/asyncpp/asyncpp-grpc.git")
  FetchContent_MakeAvailable(asyncpp_grpc)
endif()

include(GenerateGRPC)
set(PROTOBUF_GENERATE_CPP_APPEND_PATH TRUE)
grpc_pb_generate_cpp(
  PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR}/protos/
  ${CMAKE_CURRENT_SOURCE_DIR}/proto/echo.proto)

add_executable(sample_service ${PROTO_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(sample_service PRIVATE asyncpp_grpc ${CMAKE_DL_LIBS})
target_include_directories(sample_service PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/protos/)
