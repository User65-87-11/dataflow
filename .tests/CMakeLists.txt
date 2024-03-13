cmake_minimum_required(VERSION 3.28)

project(
  tests
  VERSION 1.0
  LANGUAGES CXX)

set(SOURCES 
main.cpp
namespace_impl_a.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../src/dataflow.cpp
)  

set(INCLUDES
    include 
)

set(CMAKE_CXX_FLAGS "-fpermissive")

add_executable(tests ${SOURCES})

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../${INCLUDES})

set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)