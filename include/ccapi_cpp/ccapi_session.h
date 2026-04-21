#pragma once

#include "ccapi_cpp/ccapi_macro.h"

#include "ccapi_cpp/ccapi_event.h"
#include "ccapi_cpp/ccapi_event_dispatcher.h"
#include "ccapi_cpp/ccapi_event_handler.h"
#include "ccapi_cpp/ccapi_queue.h"
#include "ccapi_cpp/ccapi_request.h"
#include "ccapi_cpp/ccapi_session_configs.h"
#include "ccapi_cpp/ccapi_session_options.h"
#include "ccapi_cpp/ccapi_subscription.h"

#include <boost/system/error_code.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ccapi {

/**
 * Opaque handle for a shared asio io_context + thread pool. Pass one instance
 * to multiple Sessions to make them share a single event loop (useful for
 * HFT / colo deployments where thread pinning + single-core cache residency
 * matter more than per-session parallelism).
 *
 * Internally wraps ccapi::ServiceContext, keeping boost.asio types out of
 * the public include chain.
 */
class SharedContext {
 public:
  SharedContext();
  ~SharedContext();
  SharedContext(const SharedContext&) = delete;
  SharedContext& operator=(const SharedContext&) = delete;

  void start();
  void stop();

 private:
  struct Impl;
  std::unique_ptr<Impl> pImpl_;
  friend class Session;  // so Session::Session can read the internal context
};

/**
 * Pimpl'd session. All asio / service details are hidden behind pImpl_ so
 * user TUs don't transitively instantiate boost.asio / boost.beast templates.
 * Public API signatures are preserved from the original header-only class.
 */
class Session {
 public:
  Session(const Session&) = delete;
  Session& operator=(const Session&) = delete;

  Session(const SessionOptions& sessionOptions = SessionOptions(), const SessionConfigs& sessionConfigs = SessionConfigs(), EventHandler* eventHandler = nullptr,
          EventDispatcher* eventDispatcher = nullptr
#ifndef SWIG
          ,
          SharedContext* sharedContext = nullptr
#endif
  );

  virtual ~Session();

  virtual void start();
  virtual void stop();
  virtual void setHeartbeatTimer(long heartbeatIntervalMilliseconds);

  virtual void subscribe(Subscription& subscription);
  virtual void subscribe(std::vector<Subscription>& subscriptionList);

  virtual void onEvent(Event& event, Queue<Event>* eventQueue);

  virtual void sendRequestByFix(const std::string& fixOrderEntrySubscriptionCorrelationId, Request& request);
  virtual void sendRequestByFix(const std::string& fixOrderEntrySubscriptionCorrelationId, std::vector<Request>& requestList);
  virtual void sendRequestByWebsocket(const std::string& websocketOrderEntrySubscriptionCorrelationId, Request& request);
  virtual void sendRequestByWebsocket(const std::string& websocketOrderEntrySubscriptionCorrelationId, std::vector<Request>& requestList);
  virtual void sendRequest(Request& request, Queue<Event>* eventQueuePtr = nullptr, long delayMilliseconds = 0);
  virtual void sendRequest(std::vector<Request>& requestList, Queue<Event>* eventQueuePtr = nullptr, long delayMilliseconds = 0);

  virtual Queue<Event>& getEventQueue();

  virtual void onError(const Event::Type eventType, const Message::Type messageType, const std::string& errorMessage, Queue<Event>* eventQueuePtr = nullptr);

#ifndef SWIG
  virtual void setImmediate(std::function<void()> successHandler);
  virtual void setTimer(const std::string& id, long delayMilliseconds, std::function<void(const boost::system::error_code&)> errorHandler,
                        std::function<void()> successHandler);
  virtual void cancelTimer(const std::string& id);
  void purgeHttpConnectionPool(const std::string& serviceName = "", const std::string& exchangeName = "");
  void forceCloseWebsocketConnections(const std::string& serviceName = "", const std::string& exchangeName = "");
#endif

 private:
  struct Impl;
  std::unique_ptr<Impl> pImpl_;
};

}  // namespace ccapi
