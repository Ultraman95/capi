#include "ccapi_cpp/ccapi_session.h"

namespace ccapi {

Logger* Logger::logger = nullptr;  // This line is needed.

class MyEventHandler : public EventHandler {
 public:
  void processEvent(const Event& event, Session* sessionPtr) override {
    std::cout << "Received an event from session " << this->sessionPtrs.at(sessionPtr) << " on thread " << std::this_thread::get_id() << std::endl;
  }

  void setSessionPtrs(const std::map<Session*, std::string>& sessionPtrs) { this->sessionPtrs = sessionPtrs; }

 private:
  std::map<Session*, std::string> sessionPtrs;
};

} /* namespace ccapi */

using ::ccapi::MyEventHandler;
using ::ccapi::Session;
using ::ccapi::SessionConfigs;
using ::ccapi::SessionOptions;
using ::ccapi::Subscription;
using ::ccapi::toString;

// NOTE: previously this example created one shared ccapi::ServiceContext and
// passed it to both Sessions. After the Pimpl refactor, ServiceContext is no
// longer a public type (its asio internals are hidden). The "multiple sessions"
// demonstration still works here - each Session owns its own internal
// ServiceContext. If you need a shared io_context across sessions in the
// future, expose a ServiceContextHandle in the public API.
int main(int argc, char** argv) {
  SessionOptions sessionOptions_1;
  SessionOptions sessionOptions_2;
  SessionConfigs sessionConfigs_1;
  SessionConfigs sessionConfigs_2;
  MyEventHandler eventHandler;

  Session session_1(sessionOptions_1, sessionConfigs_1, &eventHandler);
  Session session_2(sessionOptions_2, sessionConfigs_2, &eventHandler);
  eventHandler.setSessionPtrs({
      {&session_1, "1"},
      {&session_2, "2"},
  });
  Subscription subscription("bybit", "BTCUSDT", "MARKET_DEPTH");
  session_1.subscribe(subscription);
  session_2.subscribe(subscription);

  std::this_thread::sleep_for(std::chrono::seconds(10));
  session_1.stop();
  session_2.stop();
  std::cout << "Bye" << std::endl;
  return EXIT_SUCCESS;
}
