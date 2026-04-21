include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(NAME ccapi_core)

add_library(${NAME} STATIC ccapi_session.cpp)

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
  CCAPI_ENABLE_EXCHANGE_BITMART
  CCAPI_ENABLE_EXCHANGE_MEXC
  CCAPI_ENABLE_EXCHANGE_MEXC_FUTURES
)

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

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ccapiConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ccapiConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccapi)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ccapiConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccapi)
