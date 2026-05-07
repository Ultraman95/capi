#include "ccapi_cpp/ccapi_session.h"
#include <iostream>

namespace ccapi {

Logger* Logger::logger = nullptr;  // This line is needed.

class MyEventHandler : public EventHandler {
 public:
  void processEvent(const Event& event, Session* sessionPtr) override {
    if (event.getType() == Event::Type::SUBSCRIPTION_STATUS) {
      std::cout << "Received an event of type SUBSCRIPTION_STATUS:\n" + event.toPrettyString(2, 2) << std::endl;
    } else if (event.getType() == Event::Type::SUBSCRIPTION_DATA) {
      for (const auto& message : event.getMessageList()) {
        std::cout << std::string("Best bid and ask at ") + UtilTime::getISOTimestamp(message.getTime()) + " are:" << std::endl;
        for (const auto& element : message.getElementList()) {
          // They key std::string_view is created from a string literal and therefore is safe, because string
          // literals have static storage duration, meaning they live for the entire duration of the program.
          const std::map<std::string_view, std::string>& elementNameValueMap = element.getNameValueMap();
          std::cout << "  " + toString(elementNameValueMap) << std::endl;
        }
      }
    }
  }
};

} /* namespace ccapi */

using ::ccapi::MyEventHandler;
using ::ccapi::Session;
using ::ccapi::SessionConfigs;
using ::ccapi::SessionOptions;
using ::ccapi::Subscription;
using ::ccapi::toString;

int main(int argc, char** argv) {
  const std::string exchange = argc > 1 ? argv[1] : CCAPI_EXCHANGE_NAME_BINANCE_USDS_FUTURES;
  const std::string symbol = argc > 2 ? argv[2] : "BTCUSDT";
  const int seconds = argc > 3 ? std::stoi(argv[3]) : 10;

  SessionOptions sessionOptions;
  SessionConfigs sessionConfigs;
  MyEventHandler eventHandler;
  Session session(sessionOptions, sessionConfigs, &eventHandler);
  Subscription subscription(exchange, symbol, CCAPI_MARKET_DEPTH);
  session.subscribe(subscription);
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
  session.stop();
  std::cout << "Bye" << std::endl;
  return EXIT_SUCCESS;
}
