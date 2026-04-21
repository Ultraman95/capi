#include "ccapi_cpp/ccapi_session.h"

// start: enable exchanges for market data
#ifdef CCAPI_ENABLE_SERVICE_MARKET_DATA
#ifdef CCAPI_ENABLE_EXCHANGE_COINBASE
#include "ccapi_cpp/service/ccapi_market_data_service_coinbase.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GEMINI
#include "ccapi_cpp/service/ccapi_market_data_service_gemini.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KRAKEN
#include "ccapi_cpp/service/ccapi_market_data_service_kraken.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KRAKEN_FUTURES
#include "ccapi_cpp/service/ccapi_market_data_service_kraken_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITSTAMP
#include "ccapi_cpp/service/ccapi_market_data_service_bitstamp.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITFINEX
#include "ccapi_cpp/service/ccapi_market_data_service_bitfinex.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITMEX
#include "ccapi_cpp/service/ccapi_market_data_service_bitmex.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_US
#include "ccapi_cpp/service/ccapi_market_data_service_binance_us.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE
#include "ccapi_cpp/service/ccapi_market_data_service_binance.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_USDS_FUTURES
#include "ccapi_cpp/service/ccapi_market_data_service_binance_usds_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_COIN_FUTURES
#include "ccapi_cpp/service/ccapi_market_data_service_binance_coin_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI
#include "ccapi_cpp/service/ccapi_market_data_service_huobi.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI_USDT_SWAP
#include "ccapi_cpp/service/ccapi_market_data_service_huobi_usdt_swap.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI_COIN_SWAP
#include "ccapi_cpp/service/ccapi_market_data_service_huobi_coin_swap.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_OKX
#include "ccapi_cpp/service/ccapi_market_data_service_okx.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KUCOIN
#include "ccapi_cpp/service/ccapi_market_data_service_kucoin.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KUCOIN_FUTURES
#include "ccapi_cpp/service/ccapi_market_data_service_kucoin_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_DERIBIT
#include "ccapi_cpp/service/ccapi_market_data_service_deribit.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GATEIO
#include "ccapi_cpp/service/ccapi_market_data_service_gateio.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GATEIO_PERPETUAL_FUTURES
#include "ccapi_cpp/service/ccapi_market_data_service_gateio_perpetual_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_CRYPTOCOM
#include "ccapi_cpp/service/ccapi_market_data_service_cryptocom.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BYBIT
#include "ccapi_cpp/service/ccapi_market_data_service_bybit.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_ASCENDEX
#include "ccapi_cpp/service/ccapi_market_data_service_ascendex.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITGET
#include "ccapi_cpp/service/ccapi_market_data_service_bitget.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITGET_FUTURES
#include "ccapi_cpp/service/ccapi_market_data_service_bitget_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITMART
#include "ccapi_cpp/service/ccapi_market_data_service_bitmart.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_MEXC
#include "ccapi_cpp/service/ccapi_market_data_service_mexc.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_MEXC_FUTURES
#include "ccapi_cpp/service/ccapi_market_data_service_mexc_futures.h"
#endif
#endif
// end: enable exchanges for market data

// start: enable exchanges for execution management
#ifdef CCAPI_ENABLE_SERVICE_EXECUTION_MANAGEMENT
#ifdef CCAPI_ENABLE_EXCHANGE_COINBASE
#include "ccapi_cpp/service/ccapi_execution_management_service_coinbase.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GEMINI
#include "ccapi_cpp/service/ccapi_execution_management_service_gemini.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KRAKEN
#include "ccapi_cpp/service/ccapi_execution_management_service_kraken.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KRAKEN_FUTURES
#include "ccapi_cpp/service/ccapi_execution_management_service_kraken_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITSTAMP
#include "ccapi_cpp/service/ccapi_execution_management_service_bitstamp.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITFINEX
#include "ccapi_cpp/service/ccapi_execution_management_service_bitfinex.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITMEX
#include "ccapi_cpp/service/ccapi_execution_management_service_bitmex.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_US
#include "ccapi_cpp/service/ccapi_execution_management_service_binance_us.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE
#include "ccapi_cpp/service/ccapi_execution_management_service_binance.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_USDS_FUTURES
#include "ccapi_cpp/service/ccapi_execution_management_service_binance_usds_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_COIN_FUTURES
#include "ccapi_cpp/service/ccapi_execution_management_service_binance_coin_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI
#include "ccapi_cpp/service/ccapi_execution_management_service_huobi.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI_USDT_SWAP
#include "ccapi_cpp/service/ccapi_execution_management_service_huobi_usdt_swap.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI_COIN_SWAP
#include "ccapi_cpp/service/ccapi_execution_management_service_huobi_coin_swap.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_OKX
#include "ccapi_cpp/service/ccapi_execution_management_service_okx.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KUCOIN
#include "ccapi_cpp/service/ccapi_execution_management_service_kucoin.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KUCOIN_FUTURES
#include "ccapi_cpp/service/ccapi_execution_management_service_kucoin_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_DERIBIT
#include "ccapi_cpp/service/ccapi_execution_management_service_deribit.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GATEIO
#include "ccapi_cpp/service/ccapi_execution_management_service_gateio.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GATEIO_PERPETUAL_FUTURES
#include "ccapi_cpp/service/ccapi_execution_management_service_gateio_perpetual_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_CRYPTOCOM
#include "ccapi_cpp/service/ccapi_execution_management_service_cryptocom.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BYBIT
#include "ccapi_cpp/service/ccapi_execution_management_service_bybit.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_ASCENDEX
#include "ccapi_cpp/service/ccapi_execution_management_service_ascendex.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITGET
#include "ccapi_cpp/service/ccapi_execution_management_service_bitget.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITGET_FUTURES
#include "ccapi_cpp/service/ccapi_execution_management_service_bitget_futures.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITMART
#include "ccapi_cpp/service/ccapi_execution_management_service_bitmart.h"
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_MEXC
#include "ccapi_cpp/service/ccapi_execution_management_service_mexc.h"
#endif
#endif
// end: enable exchanges for execution management

// start: enable exchanges for FIX
#ifdef CCAPI_ENABLE_SERVICE_FIX
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE
#include "ccapi_cpp/service/ccapi_fix_service_binance.h"
#endif
#endif
// end: enable exchanges for FIX

#include "ccapi_cpp/service/ccapi_service.h"
#include "ccapi_cpp/service/ccapi_service_context.h"

#include <algorithm>
#include <map>
#include <unordered_set>
#include <utility>

namespace ccapi {

using steady_timer = boost::asio::steady_timer;

/**
 * Opaque implementation holding every asio/service/state member of Session.
 * Lives entirely in this translation unit so user TUs never see boost.asio.
 */
struct Session::Impl {
  SessionOptions sessionOptions;
  SessionConfigs sessionConfigs;
  EventHandler* eventHandler{nullptr};
  EventDispatcher* eventDispatcher{nullptr};
  std::map<std::string, std::map<std::string, std::shared_ptr<Service>>> serviceByServiceNameExchangeMap;
  Queue<Event> eventQueue;
  ServiceContext* serviceContextPtr{nullptr};
  bool useInternalServiceContextPtr{};
  std::function<void(Event& event, Queue<Event>* eventQueue)> onEventFunc;
  std::map<std::string, std::shared_ptr<steady_timer>> delayTimerByIdMap;
  std::shared_ptr<steady_timer> heartbeatTimerPtr{nullptr};

  Impl(const SessionOptions& so, const SessionConfigs& sc, EventHandler* eh, EventDispatcher* ed, ServiceContext* scPtr)
      : sessionOptions(so), sessionConfigs(sc), eventHandler(eh), eventDispatcher(ed), eventQueue(so.maxEventQueueSize), serviceContextPtr(scPtr) {
    if (!this->serviceContextPtr) {
      this->serviceContextPtr = new ServiceContext();
      this->useInternalServiceContextPtr = true;
    }
  }

  ~Impl() {
    CCAPI_LOGGER_FUNCTION_ENTER;
    this->delayTimerByIdMap.clear();
    this->serviceByServiceNameExchangeMap.clear();
    if (this->useInternalServiceContextPtr) {
      delete this->serviceContextPtr;
    }
    CCAPI_LOGGER_FUNCTION_EXIT;
  }

  void start(Session* self) {
    CCAPI_LOGGER_FUNCTION_ENTER;
    if (this->useInternalServiceContextPtr) {
      this->serviceContextPtr->start();
    }
    this->onEventFunc = [self](Event& event, Queue<Event>* q) { self->onEvent(event, q); };
#ifdef CCAPI_ENABLE_SERVICE_MARKET_DATA
#ifdef CCAPI_ENABLE_EXCHANGE_COINBASE
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_COINBASE] =
        std::make_shared<MarketDataServiceCoinbase>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GEMINI
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_GEMINI] =
        std::make_shared<MarketDataServiceGemini>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KRAKEN
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_KRAKEN] =
        std::make_shared<MarketDataServiceKraken>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KRAKEN_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_KRAKEN_FUTURES] =
        std::make_shared<MarketDataServiceKrakenFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITSTAMP
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BITSTAMP] =
        std::make_shared<MarketDataServiceBitstamp>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITFINEX
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BITFINEX] =
        std::make_shared<MarketDataServiceBitfinex>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITMEX
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BITMEX] =
        std::make_shared<MarketDataServiceBitmex>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_US
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BINANCE_US] =
        std::make_shared<MarketDataServiceBinanceUs>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BINANCE] =
        std::make_shared<MarketDataServiceBinance>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_USDS_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BINANCE_USDS_FUTURES] =
        std::make_shared<MarketDataServiceBinanceUsdsFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_COIN_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BINANCE_COIN_FUTURES] =
        std::make_shared<MarketDataServiceBinanceCoinFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_HUOBI] =
        std::make_shared<MarketDataServiceHuobi>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI_USDT_SWAP
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_HUOBI_USDT_SWAP] =
        std::make_shared<MarketDataServiceHuobiUsdtSwap>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI_COIN_SWAP
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_HUOBI_COIN_SWAP] =
        std::make_shared<MarketDataServiceHuobiCoinSwap>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_OKX
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_OKX] =
        std::make_shared<MarketDataServiceOkx>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KUCOIN
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_KUCOIN] =
        std::make_shared<MarketDataServiceKucoin>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KUCOIN_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_KUCOIN_FUTURES] =
        std::make_shared<MarketDataServiceKucoinFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_DERIBIT
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_DERIBIT] =
        std::make_shared<MarketDataServiceDeribit>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GATEIO
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_GATEIO] =
        std::make_shared<MarketDataServiceGateio>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GATEIO_PERPETUAL_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_GATEIO_PERPETUAL_FUTURES] =
        std::make_shared<MarketDataServiceGateioPerpetualFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_CRYPTOCOM
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_CRYPTOCOM] =
        std::make_shared<MarketDataServiceCryptocom>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BYBIT
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BYBIT] =
        std::make_shared<MarketDataServiceBybit>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_ASCENDEX
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_ASCENDEX] =
        std::make_shared<MarketDataServiceAscendex>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITGET
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BITGET] =
        std::make_shared<MarketDataServiceBitget>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITGET_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BITGET_FUTURES] =
        std::make_shared<MarketDataServiceBitgetFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITMART
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_BITMART] =
        std::make_shared<MarketDataServiceBitmart>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_MEXC
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_MEXC] =
        std::make_shared<MarketDataServiceMexc>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_MEXC_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_MARKET_DATA][CCAPI_EXCHANGE_NAME_MEXC_FUTURES] =
        std::make_shared<MarketDataServiceMexcFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#endif
#ifdef CCAPI_ENABLE_SERVICE_EXECUTION_MANAGEMENT
#ifdef CCAPI_ENABLE_EXCHANGE_COINBASE
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_COINBASE] =
        std::make_shared<ExecutionManagementServiceCoinbase>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GEMINI
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_GEMINI] =
        std::make_shared<ExecutionManagementServiceGemini>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KRAKEN
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_KRAKEN] =
        std::make_shared<ExecutionManagementServiceKraken>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KRAKEN_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_KRAKEN_FUTURES] =
        std::make_shared<ExecutionManagementServiceKrakenFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITSTAMP
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BITSTAMP] =
        std::make_shared<ExecutionManagementServiceBitstamp>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITFINEX
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BITFINEX] =
        std::make_shared<ExecutionManagementServiceBitfinex>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITMEX
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BITMEX] =
        std::make_shared<ExecutionManagementServiceBitmex>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_US
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BINANCE_US] =
        std::make_shared<ExecutionManagementServiceBinanceUs>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BINANCE] =
        std::make_shared<ExecutionManagementServiceBinance>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_USDS_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BINANCE_USDS_FUTURES] =
        std::make_shared<ExecutionManagementServiceBinanceUsdsFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_COIN_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BINANCE_COIN_FUTURES] =
        std::make_shared<ExecutionManagementServiceBinanceCoinFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_HUOBI] =
        std::make_shared<ExecutionManagementServiceHuobi>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI_USDT_SWAP
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_HUOBI_USDT_SWAP] =
        std::make_shared<ExecutionManagementServiceHuobiUsdtSwap>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_HUOBI_COIN_SWAP
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_HUOBI_COIN_SWAP] =
        std::make_shared<ExecutionManagementServiceHuobiCoinSwap>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_OKX
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_OKX] =
        std::make_shared<ExecutionManagementServiceOkx>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KUCOIN
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_KUCOIN] =
        std::make_shared<ExecutionManagementServiceKucoin>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_KUCOIN_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_KUCOIN_FUTURES] =
        std::make_shared<ExecutionManagementServiceKucoinFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_DERIBIT
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_DERIBIT] =
        std::make_shared<ExecutionManagementServiceDeribit>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GATEIO
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_GATEIO] =
        std::make_shared<ExecutionManagementServiceGateio>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_GATEIO_PERPETUAL_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_GATEIO_PERPETUAL_FUTURES] =
        std::make_shared<ExecutionManagementServiceGateioPerpetualFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_CRYPTOCOM
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_CRYPTOCOM] =
        std::make_shared<ExecutionManagementServiceCryptocom>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BYBIT
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BYBIT] =
        std::make_shared<ExecutionManagementServiceBybit>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_ASCENDEX
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_ASCENDEX] =
        std::make_shared<ExecutionManagementServiceAscendex>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITGET
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BITGET] =
        std::make_shared<ExecutionManagementServiceBitget>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITGET_FUTURES
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BITGET_FUTURES] =
        std::make_shared<ExecutionManagementServiceBitgetFutures>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_BITMART
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_BITMART] =
        std::make_shared<ExecutionManagementServiceBitmart>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#ifdef CCAPI_ENABLE_EXCHANGE_MEXC
    this->serviceByServiceNameExchangeMap[CCAPI_EXECUTION_MANAGEMENT][CCAPI_EXCHANGE_NAME_MEXC] =
        std::make_shared<ExecutionManagementServiceMexc>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#endif

#ifdef CCAPI_ENABLE_SERVICE_FIX
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE
    this->serviceByServiceNameExchangeMap[CCAPI_FIX][CCAPI_EXCHANGE_NAME_BINANCE] =
        std::make_shared<FixServiceBinance>(this->onEventFunc, sessionOptions, sessionConfigs, this->serviceContextPtr);
#endif
#endif
    for (const auto& x : this->serviceByServiceNameExchangeMap) {
      auto serviceName = x.first;
      for (const auto& y : x.second) {
        auto exchange = y.first;
        CCAPI_LOGGER_INFO("enabled service: " + serviceName + ", exchange: " + exchange);
      }
    }
    CCAPI_LOGGER_FUNCTION_EXIT;
  }

  void stop() {
    boost::asio::post(*this->serviceContextPtr->ioContextPtr, [this]() {
      for (const auto& [_, delayTimer] : this->delayTimerByIdMap) {
        delayTimer->cancel();
      }
      if (heartbeatTimerPtr) {
        heartbeatTimerPtr->cancel();
      }
      for (const auto& x : this->serviceByServiceNameExchangeMap) {
        for (const auto& y : x.second) {
          y.second->stop();
        }
      }
    });
    if (this->useInternalServiceContextPtr) {
      this->serviceContextPtr->stop();
    }
  }

  using ErrorCode = boost::system::error_code;

  void setHeartbeatTimer(Session* self, long heartbeatIntervalMilliseconds) {
    auto timerPtr =
        std::make_shared<boost::asio::steady_timer>(*this->serviceContextPtr->ioContextPtr, std::chrono::milliseconds(heartbeatIntervalMilliseconds));
    timerPtr->async_wait([this, self, heartbeatIntervalMilliseconds](ErrorCode const& ec) {
      if (ec) {
        if (ec != boost::asio::error::operation_aborted) {
          std::string errorMessage = "heartbeat timer error: " + ec.message();
          CCAPI_LOGGER_ERROR(errorMessage);
          self->onError(Event::Type::SESSION_STATUS, Message::Type::GENERIC_ERROR, errorMessage);
        }
      } else {
        Event event;
        event.setType(Event::Type::HEARTBEAT);
        self->onEvent(event, nullptr);
        self->setHeartbeatTimer(heartbeatIntervalMilliseconds);
      }
    });
    this->heartbeatTimerPtr = timerPtr;
  }

  void subscribe(Session* self, Subscription& subscription) {
    std::vector<Subscription> subscriptionList;
    subscriptionList.push_back(subscription);
    self->subscribe(subscriptionList);
  }

  void subscribe(Session* self, std::vector<Subscription>& subscriptionList) {
    CCAPI_LOGGER_FUNCTION_ENTER;
    for (auto& subscription : subscriptionList) {
      CCAPI_LOGGER_TRACE("subscription = " + toString(subscription));
      if (subscription.getField() == CCAPI_HEARTBEAT) {
        self->setHeartbeatTimer(std::stol(subscription.getOptionMap().at(CCAPI_HEARTBEAT_INTERVAL_MILLISECONDS)));
        continue;
      }
      auto exchange = subscription.getExchange();
      if (exchange == CCAPI_EXCHANGE_NAME_BYBIT) {
        auto instrumentType = subscription.getInstrumentType();
        if (instrumentType.empty()) {
          instrumentType = "spot";
        }
        std::vector<std::string> instrumentTypeList = {"spot", "linear", "inverse", "option"};
        if (std::find(instrumentTypeList.begin(), instrumentTypeList.end(), instrumentType) == instrumentTypeList.end()) {
          self->onError(Event::Type::SUBSCRIPTION_STATUS, Message::Type::SUBSCRIPTION_FAILURE,
                        "unsupported exchange instrument types: " + toString(instrumentType) + ". Allowed values: " + toString(instrumentTypeList) + ".");
          return;
        }
        subscription.setInstrumentType(instrumentType);
      } else if (exchange == CCAPI_EXCHANGE_NAME_BITGET_FUTURES) {
        auto instrumentType = subscription.getInstrumentType();
        if (instrumentType.empty()) {
          instrumentType = "USDT-FUTURES";
        }
        std::vector<std::string> instrumentTypeList = {"USDT-FUTURES", "COIN-FUTURES", "USDC-FUTURES"};
        if (std::find(instrumentTypeList.begin(), instrumentTypeList.end(), instrumentType) == instrumentTypeList.end()) {
          self->onError(Event::Type::SUBSCRIPTION_STATUS, Message::Type::SUBSCRIPTION_FAILURE,
                        "unsupported exchange instrument types: " + toString(instrumentType) + ". Allowed values: " + toString(instrumentTypeList) + ".");
          return;
        }
        subscription.setInstrumentType(instrumentType);
      }
    }
    std::map<std::string, std::vector<Subscription>> subscriptionListByServiceNameMap;
    for (const auto& subscription : subscriptionList) {
      if (subscription.getField() == CCAPI_HEARTBEAT) {
        continue;
      }
      auto serviceName = subscription.getServiceName();
      subscriptionListByServiceNameMap[serviceName].push_back(subscription);
    }
    for (const auto& x : subscriptionListByServiceNameMap) {
      auto serviceName = x.first;
      auto subList = x.second;
      if (this->serviceByServiceNameExchangeMap.find(serviceName) == this->serviceByServiceNameExchangeMap.end()) {
        self->onError(Event::Type::SUBSCRIPTION_STATUS, Message::Type::SUBSCRIPTION_FAILURE,
                      "please enable service: " + serviceName + ", and the exchanges that you want for subscriptionList " + toString(subList));
        return;
      }
      if (serviceName == CCAPI_MARKET_DATA) {
        std::unordered_set<std::string> unsupportedExchangeFieldSet;
        auto exchangeFieldMap = this->sessionConfigs.getExchangeFieldMap();
        CCAPI_LOGGER_DEBUG("exchangeFieldMap = " + toString(exchangeFieldMap));
        for (const auto& subscription : subList) {
          auto exchange = subscription.getExchange();
          CCAPI_LOGGER_DEBUG("exchange = " + exchange);
          auto field = subscription.getField();
          auto optionMap = subscription.getOptionMap();
          CCAPI_LOGGER_DEBUG("field = " + field);
          if (exchangeFieldMap.find(exchange) == exchangeFieldMap.end() ||
              std::find(exchangeFieldMap.find(exchange)->second.begin(), exchangeFieldMap.find(exchange)->second.end(), field) ==
                  exchangeFieldMap.find(exchange)->second.end()) {
            CCAPI_LOGGER_DEBUG("unsupported exchange " + exchange + ", field = " + field);
            unsupportedExchangeFieldSet.insert(exchange + "|" + field);
          }
        }
        if (!unsupportedExchangeFieldSet.empty()) {
          self->onError(Event::Type::SUBSCRIPTION_STATUS, Message::Type::SUBSCRIPTION_FAILURE, "unsupported exchange fields: " + toString(unsupportedExchangeFieldSet));
          return;
        }
        std::map<std::string, std::vector<Subscription>> subscriptionListByExchangeMap;
        for (const auto& subscription : subList) {
          auto exchange = subscription.getExchange();
          subscriptionListByExchangeMap[exchange].push_back(subscription);
        }
        CCAPI_LOGGER_TRACE("subscriptionListByExchangeMap = " + toString(subscriptionListByExchangeMap));
        for (auto& subscriptionListByExchange : subscriptionListByExchangeMap) {
          auto exchange = subscriptionListByExchange.first;
          auto innerList = subscriptionListByExchange.second;
          std::map<std::string, std::shared_ptr<Service>>& serviceByExchangeMap = this->serviceByServiceNameExchangeMap.at(serviceName);
          if (serviceByExchangeMap.find(exchange) == serviceByExchangeMap.end()) {
            self->onError(Event::Type::SUBSCRIPTION_STATUS, Message::Type::SUBSCRIPTION_FAILURE,
                          "please enable exchange: " + exchange + " for subscriptionList " + toString(innerList));
            return;
          }
          serviceByExchangeMap.at(exchange)->subscribe(innerList);
        }
      } else if (serviceName == CCAPI_EXECUTION_MANAGEMENT || serviceName == CCAPI_FIX) {
        std::map<std::string, std::vector<Subscription>> subscriptionListByExchangeMap;
        for (const auto& subscription : subList) {
          auto exchange = subscription.getExchange();
          subscriptionListByExchangeMap[exchange].push_back(subscription);
        }
        CCAPI_LOGGER_TRACE("subscriptionListByExchangeMap = " + toString(subscriptionListByExchangeMap));
        for (auto& subscriptionListByExchange : subscriptionListByExchangeMap) {
          auto exchange = subscriptionListByExchange.first;
          auto innerList = subscriptionListByExchange.second;
          std::map<std::string, std::shared_ptr<Service>>& serviceByExchangeMap = this->serviceByServiceNameExchangeMap.at(serviceName);
          if (serviceByExchangeMap.find(exchange) == serviceByExchangeMap.end()) {
            self->onError(Event::Type::SUBSCRIPTION_STATUS, Message::Type::SUBSCRIPTION_FAILURE,
                          "please enable exchange: " + exchange + " for subscriptionList " + toString(innerList));
            return;
          }
          serviceByExchangeMap.at(exchange)->subscribe(innerList);
        }
      }
    }
    CCAPI_LOGGER_FUNCTION_EXIT;
  }

  void onEvent(Session* self, Event& event, Queue<Event>* eventQueueArg) {
    CCAPI_LOGGER_FUNCTION_ENTER;
    CCAPI_LOGGER_DEBUG("event = " + toString(event));
    if (eventQueueArg) {
      eventQueueArg->pushBack(std::move(event));
    } else {
      if (this->eventHandler) {
        CCAPI_LOGGER_TRACE("handle event in immediate mode");
        if (!this->eventDispatcher) {
          try {
            this->eventHandler->processEvent(event, self);
          } catch (const std::runtime_error& e) {
            CCAPI_LOGGER_ERROR(e.what());
          }
        } else {
          this->eventDispatcher->dispatch([self, eh = this->eventHandler, event = std::move(event)] {
            try {
              eh->processEvent(event, self);
            } catch (const std::runtime_error& e) {
              CCAPI_LOGGER_ERROR(e.what());
            }
          });
        }
      } else {
        CCAPI_LOGGER_TRACE("handle event in batching mode");
        this->eventQueue.pushBack(std::move(event));
      }
    }
    CCAPI_LOGGER_FUNCTION_EXIT;
  }

  void sendRequestByFix(Session* self, const std::string& fixOrderEntrySubscriptionCorrelationId, Request& request) {
    CCAPI_LOGGER_FUNCTION_ENTER;
    auto serviceName = request.getServiceName();
    CCAPI_LOGGER_DEBUG("serviceName = " + serviceName);
    if (this->serviceByServiceNameExchangeMap.find(serviceName) == this->serviceByServiceNameExchangeMap.end()) {
      self->onError(Event::Type::FIX_STATUS, Message::Type::FIX_FAILURE,
                    "please enable service: " + serviceName + ", and the exchanges that you want for request " + toString(request));
      return;
    }
    std::map<std::string, std::shared_ptr<Service>>& serviceByExchangeMap = this->serviceByServiceNameExchangeMap.at(serviceName);
    auto exchange = request.getExchange();
    if (serviceByExchangeMap.find(exchange) == serviceByExchangeMap.end()) {
      self->onError(Event::Type::FIX_STATUS, Message::Type::FIX_FAILURE, "please enable exchange: " + exchange);
      return;
    }
    std::shared_ptr<Service> servicePtr = serviceByExchangeMap.at(exchange);
    auto now = UtilTime::now();
    servicePtr->sendRequestByFix(fixOrderEntrySubscriptionCorrelationId, request, now);
    CCAPI_LOGGER_FUNCTION_EXIT;
  }

  void sendRequestByWebsocket(Session* self, const std::string& websocketOrderEntrySubscriptionCorrelationId, Request& request) {
    CCAPI_LOGGER_FUNCTION_ENTER;
    const auto& serviceName = request.getServiceName();
    CCAPI_LOGGER_DEBUG("serviceName = " + serviceName);
    if (this->serviceByServiceNameExchangeMap.find(serviceName) == this->serviceByServiceNameExchangeMap.end()) {
      self->onError(Event::Type::REQUEST_STATUS, Message::Type::REQUEST_FAILURE,
                    "please enable service: " + serviceName + ", and the exchanges that you want for websocketOrderEntrySubscriptionCorrelationId " +
                        toString(websocketOrderEntrySubscriptionCorrelationId) + ", request = " + toString(request));
      return;
    }
    const std::map<std::string, std::shared_ptr<Service>>& serviceByExchangeMap = this->serviceByServiceNameExchangeMap.at(serviceName);
    const auto& exchange = request.getExchange();
    if (serviceByExchangeMap.find(exchange) == serviceByExchangeMap.end()) {
      self->onError(Event::Type::REQUEST_STATUS, Message::Type::REQUEST_FAILURE, "please enable exchange: " + exchange);
      return;
    }
    std::shared_ptr<Service> servicePtr = serviceByExchangeMap.at(exchange);
    const auto& now = UtilTime::now();
    servicePtr->sendRequestByWebsocket(websocketOrderEntrySubscriptionCorrelationId, request, now);
    CCAPI_LOGGER_FUNCTION_EXIT;
  }

  void sendRequest(Session* self, std::vector<Request>& requestList, Queue<Event>* eventQueuePtr, long delayMilliseconds) {
    CCAPI_LOGGER_FUNCTION_ENTER;
    std::vector<std::shared_ptr<std::future<void>>> futurePtrList;
    int i = 0;
    for (auto& request : requestList) {
      request.setIndex(i);
      const auto& serviceName = request.getServiceName();
      CCAPI_LOGGER_DEBUG("serviceName = " + serviceName);
      if (this->serviceByServiceNameExchangeMap.find(serviceName) == this->serviceByServiceNameExchangeMap.end()) {
        self->onError(Event::Type::REQUEST_STATUS, Message::Type::REQUEST_FAILURE,
                      "please enable service: " + serviceName + ", and the exchanges that you want for request " + toString(request), eventQueuePtr);
        return;
      }
      std::map<std::string, std::shared_ptr<Service>>& serviceByExchangeMap = this->serviceByServiceNameExchangeMap.at(serviceName);
      const auto& exchange = request.getExchange();
      if (serviceByExchangeMap.find(exchange) == serviceByExchangeMap.end()) {
        self->onError(Event::Type::REQUEST_STATUS, Message::Type::REQUEST_FAILURE, "please enable exchange: " + exchange + " for request " + toString(request),
                      eventQueuePtr);
        return;
      }
      std::shared_ptr<Service> servicePtr = serviceByExchangeMap.at(exchange);
      const auto& now = UtilTime::now();
      auto futurePtr = servicePtr->sendRequest(request, !!eventQueuePtr, now, delayMilliseconds, eventQueuePtr);
      if (eventQueuePtr) {
        futurePtrList.push_back(futurePtr);
      }
      ++i;
    }
    if (eventQueuePtr) {
      for (auto& futurePtr : futurePtrList) {
        CCAPI_LOGGER_TRACE("before future wait");
        futurePtr->wait();
        CCAPI_LOGGER_TRACE("after future wait");
      }
    }
    CCAPI_LOGGER_FUNCTION_EXIT;
  }

  void onError(Session* self, const Event::Type eventType, const Message::Type messageType, const std::string& errorMessage, Queue<Event>* eventQueuePtr) {
    CCAPI_LOGGER_ERROR("errorMessage = " + errorMessage);
    Event event;
    event.setType(eventType);
    Message message;
    auto now = UtilTime::now();
    message.setTimeReceived(now);
    message.setTime(now);
    message.setType(messageType);
    Element element;
    element.insert(CCAPI_ERROR_MESSAGE, errorMessage);
    message.setElementList({element});
    event.setMessageList({message});
    self->onEvent(event, eventQueuePtr);
  }

  void setImmediate(std::function<void()> successHandler) {
    boost::asio::post(*this->serviceContextPtr->ioContextPtr, [this, successHandler]() {
      if (this->eventHandler) {
        if (!this->eventDispatcher) {
          successHandler();
        } else {
          this->eventDispatcher->dispatch([successHandler] { successHandler(); });
        }
      }
    });
  }

  void setTimer(const std::string& id, long delayMilliseconds, std::function<void(const boost::system::error_code&)> errorHandler,
                std::function<void()> successHandler) {
    boost::asio::post(*this->serviceContextPtr->ioContextPtr, [this, id, delayMilliseconds, errorHandler, successHandler]() {
      auto timerPtr = std::make_shared<boost::asio::steady_timer>(*this->serviceContextPtr->ioContextPtr, boost::asio::chrono::milliseconds(delayMilliseconds));
      timerPtr->async_wait([this, id, errorHandler, successHandler](const boost::system::error_code& ec) {
        if (this->eventHandler) {
          if (!this->eventDispatcher) {
            if (ec) {
              if (errorHandler) {
                errorHandler(ec);
              }
            } else {
              if (successHandler) {
                successHandler();
              }
            }
          } else {
            this->eventDispatcher->dispatch([ec, errorHandler, successHandler] {
              if (ec) {
                if (errorHandler) {
                  errorHandler(ec);
                }
              } else {
                if (successHandler) {
                  successHandler();
                }
              }
            });
          }
        }
        this->delayTimerByIdMap.erase(id);
      });
      this->delayTimerByIdMap[id] = timerPtr;
    });
  }

  void cancelTimer(const std::string& id) {
    boost::asio::post(*this->serviceContextPtr->ioContextPtr, [this, id]() {
      if (this->delayTimerByIdMap.find(id) != this->delayTimerByIdMap.end()) {
        this->delayTimerByIdMap[id]->cancel();
        this->delayTimerByIdMap.erase(id);
      }
    });
  }

  void purgeHttpConnectionPool(const std::string& serviceName, const std::string& exchangeName) {
    for (const auto& x : this->serviceByServiceNameExchangeMap) {
      if (serviceName.empty() || serviceName == x.first) {
        for (const auto& y : x.second) {
          if (exchangeName.empty() || exchangeName == y.first) {
            y.second->purgeHttpConnectionPool();
          }
        }
      }
    }
  }

  void forceCloseWebsocketConnections(const std::string& serviceName, const std::string& exchangeName) {
    for (const auto& x : this->serviceByServiceNameExchangeMap) {
      if (serviceName.empty() || serviceName == x.first) {
        for (const auto& y : x.second) {
          if (exchangeName.empty() || exchangeName == y.first) {
            y.second->forceCloseWebsocketConnections();
          }
        }
      }
    }
  }
};

// ================= SharedContext thin wrapper =================

struct SharedContext::Impl {
  // Owns a real ccapi::ServiceContext; lives only in this TU so asio types
  // never leak into the public header.
  ServiceContext context;
};

SharedContext::SharedContext() : pImpl_(std::make_unique<Impl>()) {}
SharedContext::~SharedContext() = default;

void SharedContext::start() { pImpl_->context.start(); }
void SharedContext::stop() { pImpl_->context.stop(); }

// ================= Session thin forwarders =================

Session::Session(const SessionOptions& so, const SessionConfigs& sc, EventHandler* eh, EventDispatcher* ed
#ifndef SWIG
                 ,
                 SharedContext* sharedContext
#endif
                 )
    : pImpl_(std::make_unique<Impl>(so, sc, eh, ed,
#ifndef SWIG
                                    sharedContext ? &sharedContext->pImpl_->context : nullptr
#else
                                    nullptr
#endif
                                    )) {
  CCAPI_LOGGER_FUNCTION_ENTER;
  if (!pImpl_->eventHandler && pImpl_->eventDispatcher) {
    throw std::runtime_error("eventHandler is needed when eventDispatcher is provided");
  }
  this->start();
  CCAPI_LOGGER_FUNCTION_EXIT;
}

Session::~Session() = default;

void Session::start() { pImpl_->start(this); }
void Session::stop() { pImpl_->stop(); }
void Session::setHeartbeatTimer(long ms) { pImpl_->setHeartbeatTimer(this, ms); }

void Session::subscribe(Subscription& s) { pImpl_->subscribe(this, s); }
void Session::subscribe(std::vector<Subscription>& list) { pImpl_->subscribe(this, list); }

void Session::onEvent(Event& event, Queue<Event>* eventQueue) { pImpl_->onEvent(this, event, eventQueue); }

void Session::sendRequestByFix(const std::string& cid, Request& req) { pImpl_->sendRequestByFix(this, cid, req); }
void Session::sendRequestByFix(const std::string& cid, std::vector<Request>& list) {
  for (auto& x : list) this->sendRequestByFix(cid, x);
}
void Session::sendRequestByWebsocket(const std::string& cid, Request& req) { pImpl_->sendRequestByWebsocket(this, cid, req); }
void Session::sendRequestByWebsocket(const std::string& cid, std::vector<Request>& list) {
  for (auto& x : list) this->sendRequestByWebsocket(cid, x);
}
void Session::sendRequest(Request& req, Queue<Event>* q, long d) {
  std::vector<Request> list({req});
  this->sendRequest(list, q, d);
}
void Session::sendRequest(std::vector<Request>& list, Queue<Event>* q, long d) { pImpl_->sendRequest(this, list, q, d); }

Queue<Event>& Session::getEventQueue() { return pImpl_->eventQueue; }

void Session::onError(const Event::Type et, const Message::Type mt, const std::string& msg, Queue<Event>* q) { pImpl_->onError(this, et, mt, msg, q); }

#ifndef SWIG
void Session::setImmediate(std::function<void()> h) { pImpl_->setImmediate(h); }
void Session::setTimer(const std::string& id, long ms, std::function<void(const boost::system::error_code&)> eh, std::function<void()> sh) {
  pImpl_->setTimer(id, ms, eh, sh);
}
void Session::cancelTimer(const std::string& id) { pImpl_->cancelTimer(id); }
void Session::purgeHttpConnectionPool(const std::string& s, const std::string& e) { pImpl_->purgeHttpConnectionPool(s, e); }
void Session::forceCloseWebsocketConnections(const std::string& s, const std::string& e) { pImpl_->forceCloseWebsocketConnections(s, e); }
#endif

}  // namespace ccapi
