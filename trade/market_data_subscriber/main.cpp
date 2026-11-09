#include "ccapi_cpp/ccapi_session.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <thread>

namespace {

std::string trim(std::string s) {
  const auto first = s.find_first_not_of(" \t\r\n");
  if (first == std::string::npos) return {};
  const auto last = s.find_last_not_of(" \t\r\n");
  return s.substr(first, last - first + 1);
}

std::map<std::string, std::string> loadIni(const std::string& path) {
  std::ifstream in(path);
  if (!in) throw std::runtime_error("cannot open config: " + path);
  std::map<std::string, std::string> kv;
  std::string line;
  while (std::getline(in, line)) {
    if (auto hash = line.find('#'); hash != std::string::npos) line.erase(hash);
    auto eq = line.find('=');
    if (eq == std::string::npos) continue;
    auto k = trim(line.substr(0, eq));
    auto v = trim(line.substr(eq + 1));
    if (!k.empty()) kv[k] = v;
  }
  return kv;
}

const std::string& require(const std::map<std::string, std::string>& kv, const std::string& key) {
  auto it = kv.find(key);
  if (it == kv.end() || it->second.empty()) throw std::runtime_error("missing required config key: " + key);
  return it->second;
}

std::atomic<bool> g_stop{false};

}  // namespace

namespace ccapi {

Logger* Logger::logger = nullptr;

class MarketDataSubscriberHandler : public EventHandler {
 public:
  void processEvent(const Event& event, Session* /*session*/) override {
    const auto type = event.getType();
    if (type == Event::Type::SUBSCRIPTION_STATUS) {
      std::cout << "[STATUS] " << event.toPrettyString(2, 2) << std::endl;
      return;
    }
    if (type != Event::Type::SUBSCRIPTION_DATA) return;
    for (const auto& message : event.getMessageList()) {
      const auto ts = UtilTime::getISOTimestamp(message.getTime());
      for (const auto& element : message.getElementList()) {
        std::cout << ts << ' ' << toString(element.getNameValueMap()) << std::endl;
      }
    }
  }
};

}  // namespace ccapi

int main(int argc, char** argv) {
  using namespace ccapi;
  try {
    const std::string configPath = argc > 1 ? argv[1] : "./config.ini";
    const auto cfg = loadIni(configPath);

    const auto& exchange = require(cfg, "exchange");
    const auto& symbol = require(cfg, "symbol");
    const std::string field = cfg.count("field") ? cfg.at("field") : std::string{CCAPI_MARKET_DEPTH};
    const std::string marketDepthMax = cfg.count("market_depth_max") ? cfg.at("market_depth_max") : std::string{"5"};
    const std::string conflateMs = cfg.count("conflate_interval_milliseconds") ? cfg.at("conflate_interval_milliseconds") : std::string{"0"};
    const std::string instrumentType = cfg.count("instrument_type") ? cfg.at("instrument_type") : std::string{};
    const int durationSeconds = cfg.count("duration_seconds") ? std::stoi(cfg.at("duration_seconds")) : 30;

    std::cout << "subscribing exchange=" << exchange << " symbol=" << symbol << " field=" << field
              << " duration=" << durationSeconds << "s" << std::endl;

    SessionOptions sessionOptions;
    SessionConfigs sessionConfigs;
    MarketDataSubscriberHandler handler;
    Session session(sessionOptions, sessionConfigs, &handler);

    const std::string options = std::string{CCAPI_MARKET_DEPTH_MAX} + "=" + marketDepthMax +
                                "&" + std::string{CCAPI_CONFLATE_INTERVAL_MILLISECONDS} + "=" + conflateMs;
    Subscription subscription(exchange, symbol, field, options);
    if (!instrumentType.empty()) subscription.setInstrumentType(instrumentType);
    session.subscribe(subscription);

    std::signal(SIGINT, [](int) { g_stop.store(true); });
    if (durationSeconds <= 0) {
      while (!g_stop.load()) std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } else {
      const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(durationSeconds);
      while (!g_stop.load() && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }
    }
    session.stop();
    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "fatal: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
