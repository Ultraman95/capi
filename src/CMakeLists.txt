cmake_minimum_required(VERSION 3.14)

# Standalone build mode: this CMakeLists.txt can be configured directly
# (cmake -S src -B build/lib) to produce libccapi_core.a + cmake package config
# without depending on example/, benchmark/, or any other consumer. When used
# via add_subdirectory(...) from a parent project, the if(NOT ...) guards skip
# the standalone bootstrap and use whatever the parent already supplied.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  project(ccapi_core LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
  endif()
  if(NOT APPLE AND NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
  endif()
  find_program(CCACHE_PROGRAM ccache)
  if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  endif()
  if(UNIX AND NOT APPLE)
    find_program(MOLD_PROGRAM mold)
    if(MOLD_PROGRAM)
      add_link_options("-fuse-ld=mold")
    endif()
  endif()
  add_compile_options(-Wno-deprecated -Wno-nonnull -Wno-deprecated-declarations)
endif()

# Common deps: each parent project would also set these up; the guards keep
# them shared across modes.
if(NOT DEFINED CCAPI_PROJECT_DIR)
  get_filename_component(CCAPI_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
endif()

if(NOT BOOST_INCLUDE_DIR)
  set(BOOST_INCLUDE_DIR "$ENV{HOME}/boost_subset")
endif()
if(NOT EXISTS "${BOOST_INCLUDE_DIR}/boost/version.hpp")
  message(FATAL_ERROR
    "BOOST_INCLUDE_DIR='${BOOST_INCLUDE_DIR}' is missing boost/version.hpp. "
    "Generate the bcp subset with: bcp --boost=<boost_source> asio beast '${BOOST_INCLUDE_DIR}/' "
    "(or pass -DBOOST_INCLUDE_DIR=<path>).")
endif()

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
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(NAME ccapi_core)

# Optional library-level log level. Off by default (zero runtime cost);
# set e.g. -DCCAPI_LOG_LEVEL=TRACE to build the library with internal
# CCAPI_LOGGER_* macros expanded.
set(CCAPI_LOG_LEVEL "" CACHE STRING
    "Internal log verbosity (FATAL/ERROR/WARN/INFO/DETAIL/FINE/DEBUG/TRACE). Empty = disabled.")

add_library(${NAME} STATIC ccapi_session.cpp)

if(CCAPI_LOG_LEVEL)
  string(TOUPPER "${CCAPI_LOG_LEVEL}" _ccapi_log_upper)
  target_compile_definitions(${NAME} PUBLIC "CCAPI_ENABLE_LOG_${_ccapi_log_upper}")
  message(STATUS "ccapi_core: library logging enabled at CCAPI_ENABLE_LOG_${_ccapi_log_upper}")
endif()

# All services enabled: market data, execution management, FIX.
# All currently maintained exchanges enabled.
target_compile_definitions(${NAME} PUBLIC
  # ---- services ----
  CCAPI_ENABLE_SERVICE_MARKET_DATA
  CCAPI_ENABLE_SERVICE_EXECUTION_MANAGEMENT
  CCAPI_ENABLE_SERVICE_FIX
  # ---- exchanges ----
  CCAPI_ENABLE_EXCHANGE_COINBASE
  CCAPI_ENABLE_EXCHANGE_GEMINI
  CCAPI_ENABLE_EXCHANGE_KRAKEN
  CCAPI_ENABLE_EXCHANGE_KRAKEN_FUTURES
  CCAPI_ENABLE_EXCHANGE_BITSTAMP
  CCAPI_ENABLE_EXCHANGE_BITFINEX
  CCAPI_ENABLE_EXCHANGE_BITMEX
  CCAPI_ENABLE_EXCHANGE_BINANCE_US
  CCAPI_ENABLE_EXCHANGE_BINANCE
  CCAPI_ENABLE_EXCHANGE_BINANCE_USDS_FUTURES
  CCAPI_ENABLE_EXCHANGE_BINANCE_COIN_FUTURES
  CCAPI_ENABLE_EXCHANGE_HUOBI
  CCAPI_ENABLE_EXCHANGE_HUOBI_USDT_SWAP
  CCAPI_ENABLE_EXCHANGE_HUOBI_COIN_SWAP
  CCAPI_ENABLE_EXCHANGE_OKX
  CCAPI_ENABLE_EXCHANGE_KUCOIN
  CCAPI_ENABLE_EXCHANGE_KUCOIN_FUTURES
  CCAPI_ENABLE_EXCHANGE_DERIBIT
  CCAPI_ENABLE_EXCHANGE_GATEIO
  CCAPI_ENABLE_EXCHANGE_GATEIO_PERPETUAL_FUTURES
  CCAPI_ENABLE_EXCHANGE_CRYPTOCOM
  CCAPI_ENABLE_EXCHANGE_BYBIT
  CCAPI_ENABLE_EXCHANGE_ASCENDEX
  CCAPI_ENABLE_EXCHANGE_BITGET
  CCAPI_ENABLE_EXCHANGE_BITGET_FUTURES
  CCAPI_ENABLE_EXCHANGE_BITGET_UTA
  CCAPI_ENABLE_EXCHANGE_BITMART
  CCAPI_ENABLE_EXCHANGE_MEXC
  CCAPI_ENABLE_EXCHANGE_MEXC_FUTURES
  CCAPI_ENABLE_EXCHANGE_HYPERLIQUID
)

# Hyperliquid 执行管理需要 secp256k1(签名)+ msgpack-cxx(header-only,订单序列化)。
# 两者都找到才启用;否则只编行情服务,执行管理在 ccapi_session.cpp 里被跳过。
find_path(SECP256K1_INCLUDE_DIR secp256k1_recovery.h)
find_library(SECP256K1_LIBRARY secp256k1)
find_path(MSGPACK_INCLUDE_DIR msgpack.hpp)
if(SECP256K1_INCLUDE_DIR AND SECP256K1_LIBRARY AND MSGPACK_INCLUDE_DIR)
  target_include_directories(${NAME} PRIVATE ${SECP256K1_INCLUDE_DIR} ${MSGPACK_INCLUDE_DIR})
  target_link_libraries(${NAME} PUBLIC ${SECP256K1_LIBRARY})
  message(STATUS "ccapi_core: hyperliquid execution management enabled")
else()
  target_compile_definitions(${NAME} PRIVATE CCAPI_DISABLE_HYPERLIQUID_EXECUTION_MANAGEMENT)
  message(STATUS "ccapi_core: secp256k1/msgpack not found, hyperliquid execution management disabled (market data only)")
endif()

target_include_directories(${NAME} PUBLIC
  $<BUILD_INTERFACE:${CCAPI_PROJECT_DIR}/include>
  $<BUILD_INTERFACE:${BOOST_INCLUDE_DIR}>
  $<BUILD_INTERFACE:${RAPIDJSON_INCLUDE_DIR}>
  $<BUILD_INTERFACE:${OPENSSL_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# hffix needed for FIX service support
if(DEFINED HFFIX_INCLUDE_DIR)
  target_include_directories(${NAME} PUBLIC $<BUILD_INTERFACE:${HFFIX_INCLUDE_DIR}>)
endif()

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(${NAME} PUBLIC
  OpenSSL::Crypto OpenSSL::SSL
  ZLIB::ZLIB
  Threads::Threads)

if(TARGET boost)
  add_dependencies(${NAME} boost)
endif()
if(TARGET rapidjson)
  add_dependencies(${NAME} rapidjson)
endif()
if(TARGET hffix)
  add_dependencies(${NAME} hffix)
endif()

# ---------------------- install / packaging ----------------------

# Public headers (user-facing only; service/* and *_internal.h stay inside lib)
set(CCAPI_PUBLIC_HEADERS
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_element.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_event.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_event_dispatcher.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_event_handler.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_logger.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_macro.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_market_data_message.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_message.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_queue.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_request.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_session.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_session_configs.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_session_options.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_subscription.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_url.h
  ${CCAPI_PROJECT_DIR}/include/ccapi_cpp/ccapi_util.h
)

install(TARGETS ${NAME}
  EXPORT ccapiTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${CCAPI_PUBLIC_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ccapi_cpp)

install(EXPORT ccapiTargets
  FILE ccapiTargets.cmake
  NAMESPACE ccapi::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccapi)

# Also export to the build tree, so downstream projects can use
# find_package(ccapi) against the build dir directly without `cmake --install`.
# Pair it with the build-tree ccapiConfig.cmake generated below.
export(EXPORT ccapiTargets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/ccapiTargets.cmake
  NAMESPACE ccapi::)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ccapiConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ccapiConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccapi)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ccapiConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccapi)
