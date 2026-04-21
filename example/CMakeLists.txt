cmake_minimum_required(VERSION 3.14)
set(NAME example)
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
if(NOT HFFIX_INCLUDE_DIR)
  include(ExternalProject)
  ExternalProject_Add(
    hffix
    URL https://github.com/jamesdbrock/hffix/archive/refs/tags/v1.4.1.tar.gz
    SOURCE_DIR "${CMAKE_BINARY_DIR}/hffix"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND "")
  set(HFFIX_INCLUDE_DIR "${CMAKE_BINARY_DIR}/hffix/include")
else()
  add_library(hffix INTERFACE)
endif()
include_directories(
  ${CCAPI_PROJECT_DIR}/include ${BOOST_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIR}
  ${HFFIX_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

find_package(OpenSSL REQUIRED)
set(ADDITIONAL_LINK_LIBRARIES "")
if(WIN32)
  set(ADDITIONAL_LINK_LIBRARIES ws2_32)
endif()
link_libraries(OpenSSL::Crypto OpenSSL::SSL ${ADDITIONAL_LINK_LIBRARIES})
add_compile_options(-Wno-deprecated -Wno-nonnull -Wno-deprecated-declarations)
# Build the Pimpl'd ccapi core library (feasibility test).
add_subdirectory(${CCAPI_PROJECT_DIR}/src ${CMAKE_BINARY_DIR}/ccapi_core_build)

set(CCAPI_EXAMPLE_TARGETS
    market_data_simple_request
    market_data_simple_subscription
    market_data_advanced_request
    market_data_advanced_subscription
    generic_public_request
    generic_private_request
    execution_management_simple_request
    execution_management_simple_subscription
    execution_management_advanced_request
    execution_management_advanced_subscription
    fix_simple
    fix_advanced
    enable_library_logging
    set_timer
    heartbeat
    override_exchange_url_at_runtime
    test_order_latency
    test_cpu_usage
    use_multiple_sessions
    reduce_build_time)

foreach(t IN LISTS CCAPI_EXAMPLE_TARGETS)
  add_subdirectory(${t})
endforeach()
