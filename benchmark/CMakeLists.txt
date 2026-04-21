cmake_minimum_required(VERSION 3.14)
set(NAME benchmark)
project(${NAME})
set(CMAKE_CXX_STANDARD 20)
if(NOT APPLE AND NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
if(WIN32)
  set(CMAKE_BUILD_TYPE "Release")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
get_filename_component(CCAPI_PROJECT_DIR ../ ABSOLUTE)
message(STATUS "CCAPI_PROJECT_DIR: ${CCAPI_PROJECT_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
endif()

if(UNIX AND NOT APPLE)
  find_program(MOLD_PROGRAM mold)
  if(MOLD_PROGRAM)
    add_link_options("-fuse-ld=mold")
    message(STATUS "Using mold linker: ${MOLD_PROGRAM}")
  endif()
endif()

if(NOT BOOST_INCLUDE_DIR)
  set(BOOST_INCLUDE_DIR "$ENV{HOME}/boost_subset")
endif()
if(NOT EXISTS "${BOOST_INCLUDE_DIR}/boost/version.hpp")
  message(FATAL_ERROR "BOOST_INCLUDE_DIR='${BOOST_INCLUDE_DIR}' is missing boost/version.hpp. Generate the bcp subset with: bcp --boost=<boost_source> asio beast '${BOOST_INCLUDE_DIR}/' (or pass -DBOOST_INCLUDE_DIR=<path>).")
endif()
add_library(boost INTERFACE)

if(NOT RAPIDJSON_INCLUDE_DIR)
  include(ExternalProject)
  ExternalProject_Add(
    rapidjson
    URL https://github.com/Tencent/rapidjson/archive/3b2441b87f99ab65f37b141a7b548ebadb607b96.tar.gz
    SOURCE_DIR "${CMAKE_BINARY_DIR}/rapidjson"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND "")
  set(RAPIDJSON_INCLUDE_DIR "${CMAKE_BINARY_DIR}/rapidjson/include")
else()
  add_library(rapidjson INTERFACE)
endif()

include(FetchContent)
FetchContent_Declare(
  hdr_histogram
  GIT_REPOSITORY https://github.com/HdrHistogram/HdrHistogram_c.git
  GIT_TAG 0.11.8)
set(HDR_HISTOGRAM_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(HDR_HISTOGRAM_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(HDR_LOG_REQUIRED OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(hdr_histogram)

include_directories(
  ${CCAPI_PROJECT_DIR}/include ${BOOST_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR})

find_package(OpenSSL REQUIRED)
set(ADDITIONAL_LINK_LIBRARIES "")
if(WIN32)
  set(ADDITIONAL_LINK_LIBRARIES ws2_32)
endif()
link_libraries(OpenSSL::Crypto OpenSSL::SSL ${ADDITIONAL_LINK_LIBRARIES})
add_compile_options(-Wno-deprecated -Wno-nonnull -Wno-deprecated-declarations)

set(CCAPI_BENCHMARK_TARGETS market_data_latency)

foreach(t IN LISTS CCAPI_BENCHMARK_TARGETS)
  add_subdirectory(${t})
endforeach()
