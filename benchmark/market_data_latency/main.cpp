#include "ccapi_cpp/ccapi_session.h"

#include <hdr/hdr_histogram.h>

#include <chrono>
#include <cstdio>
#include <string_view>
#include <thread>

namespace ccapi {

Logger* Logger::logger = nullptr;

class LatencyHandler : public EventHandler {
 public:
  LatencyHandler() {
    // 100 纳秒 ~ 10 秒,3 位有效数字
    hdr_init(100, 10'000'000'000LL, 3, &histo_);
  }

  ~LatencyHandler() override { hdr_close(histo_); }

  void processEvent(const Event& event, Session* /*sessionPtr*/) override {
    if (event.getType() != Event::Type::SUBSCRIPTION_DATA) {
      if (event.getType() == Event::Type::SUBSCRIPTION_STATUS) {
        std::cout << event.toPrettyString(2, 2) << std::endl;
      }
      return;
    }
    const auto now = std::chrono::steady_clock::now();
    if (count_++ > 0) {
      const auto delta_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_).count();
      hdr_record_value(histo_, delta_ns);
    }
    last_ = now;
  }

  void report(std::string_view label) const {
    std::printf("\n==================================================\n");
    std::printf("%.*s inter-arrival time\n", static_cast<int>(label.size()), label.data());
    std::printf("Messages seen:     %lu\n", static_cast<unsigned long>(count_));
    std::printf("Samples recorded:  %ld\n", histo_->total_count);
    if (histo_->total_count == 0) {
      std::printf("(no samples)\n");
      return;
    }
    std::printf("Mean:   %.3f ms\n", hdr_mean(histo_) / 1e6);
    std::printf("Stddev: %.3f ms\n", hdr_stddev(histo_) / 1e6);
    std::printf("Min:    %.3f ms\n", static_cast<double>(hdr_min(histo_)) / 1e6);
    std::printf("Max:    %.3f ms\n", static_cast<double>(hdr_max(histo_)) / 1e6);
    std::printf("--- Percentiles (ms) ---\n");
    for (double p : {50.0, 90.0, 99.0, 99.9, 99.99}) {
      const int64_t v = hdr_value_at_percentile(histo_, p);
      std::printf("  p%-6.3f  %.3f\n", p, static_cast<double>(v) / 1e6);
    }
  }

 private:
  hdr_histogram* histo_{nullptr};
  std::chrono::steady_clock::time_point last_{};
  uint64_t count_{0};
};

}  // namespace ccapi

int main(int argc, char** argv) {
  using namespace ccapi;
  const int duration_seconds = (argc > 1) ? std::atoi(argv[1]) : 60;

  SessionOptions sessionOptions;
  SessionConfigs sessionConfigs;
  LatencyHandler handler;
  Session session(sessionOptions, sessionConfigs, &handler);

  Subscription futures(CCAPI_EXCHANGE_NAME_BINANCE_USDS_FUTURES, "BTCUSDT", CCAPI_MARKET_DEPTH, "", "futures");
  session.subscribe(futures);

  std::printf("Recording for %d seconds...\n", duration_seconds);
  std::this_thread::sleep_for(std::chrono::seconds(duration_seconds));
  session.stop();

  handler.report("BINANCE_USDS_FUTURES BTCUSDT");
  return 0;
}
