#ifdef CCAPI_ENABLE_SERVICE_MARKET_DATA
#ifdef CCAPI_ENABLE_EXCHANGE_BINANCE_USDS_FUTURES
#include "ccapi_cpp/service/ccapi_market_data_service_binance_usds_futures.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::ElementsAre;

namespace ccapi {

class MarketDataServiceBinanceUsdsFuturesTest : public ::testing::Test {
 public:
  void SetUp() override {
    this->service = std::make_shared<MarketDataServiceBinanceUsdsFutures>([this](Event& event, Queue<Event>*) { this->eventList.push_back(event); },
                                                                          SessionOptions(), SessionConfigs(), &this->serviceContext);
    Subscription subscription(CCAPI_EXCHANGE_NAME_BINANCE_USDS_FUTURES, "NOPE/USDT", CCAPI_MARKET_DEPTH, "MARKET_DEPTH_MAX=5", "cid");
    this->wsConnectionPtr = std::make_shared<WsConnection>("wss://fstream.binance.com/stream", "", std::vector<Subscription>{subscription},
                                                           std::map<std::string, std::string>{});
    this->service->prepareSubscription(this->wsConnectionPtr, subscription);
    this->service->createSendStringList(this->wsConnectionPtr);
  }

  Message receiveOneStatusMessage(const std::string& textMessage) {
    this->service->onTextMessage(this->wsConnectionPtr, textMessage, UtilTime::now());
    EXPECT_EQ(this->eventList.size(), 1);
    const auto& event = this->eventList.at(0);
    EXPECT_EQ(event.getType(), Event::Type::SUBSCRIPTION_STATUS);
    EXPECT_EQ(event.getMessageList().size(), 1);
    return event.getMessageList().at(0);
  }

  ServiceContext serviceContext;
  std::shared_ptr<MarketDataServiceBinanceUsdsFutures> service{nullptr};
  std::shared_ptr<WsConnection> wsConnectionPtr{nullptr};
  std::vector<Event> eventList;
};

TEST_F(MarketDataServiceBinanceUsdsFuturesTest, subscribeAckIsSubscriptionStarted) {
  auto message = this->receiveOneStatusMessage(R"({"result":null,"id":0})");
  EXPECT_EQ(message.getType(), Message::Type::SUBSCRIPTION_STARTED);
  EXPECT_THAT(message.getCorrelationIdList(), ElementsAre("cid"));
}

TEST_F(MarketDataServiceBinanceUsdsFuturesTest, subscribeErrorIsSubscriptionFailure) {
  std::string textMessage = R"({"error":{"code":2,"msg":"Invalid request: invalid stream"},"id":0})";
  auto message = this->receiveOneStatusMessage(textMessage);
  EXPECT_EQ(message.getType(), Message::Type::SUBSCRIPTION_FAILURE);
  EXPECT_THAT(message.getCorrelationIdList(), ElementsAre("cid"));
  EXPECT_EQ(message.getElementList().at(0).getValue(CCAPI_ERROR_MESSAGE), textMessage);
}

TEST_F(MarketDataServiceBinanceUsdsFuturesTest, subscribeErrorWithNullIdFailsTheConnectionSubscriptions) {
  auto message = this->receiveOneStatusMessage(R"({"error":{"code":3,"msg":"Invalid JSON: expected value at line 1 column 1"},"id":null})");
  EXPECT_EQ(message.getType(), Message::Type::SUBSCRIPTION_FAILURE);
  EXPECT_THAT(message.getCorrelationIdList(), ElementsAre("cid"));
}

} /* namespace ccapi */
#endif
#endif
