# 构建说明

- CMake 默认从 `~/boost_subset` 读取 boost(bcp 抽取的 asio+beast 子集),
  **不会**下载完整 boost 压缩包。子集缺失时 cmake 会直接报错,
  并给出重新生成的 `bcp` 命令。可用 `-DBOOST_INCLUDE_DIR=<path>` 覆盖默认路径。
- 自动启用 `ccache` + `mold` + `ninja`(CMake 会检测,不必手动传参)。
  常规开发命令:`cmake -B build -G Ninja && ninja -C build`。

# 关注范围

- `binding/`(SWIG 生成的 Python/Java/C#/Go/JS 包装层)不是当前关注点,
  不需要修改 / 构建 / 调试;README 中 `### non-C++` 章节同理。

# 工程结构(2026-04-21 重构后)

工程已从纯 header-only 重构为"**薄公共头 + 静态库**"模式:

```
include/ccapi_cpp/
├── ccapi_session.h          ← 用户入口(Pimpl,不含 asio)
├── ccapi_event*.h / subscription.h / request.h / session_options.h ...
│                            ← 公共头(15 个,属于 src/CMakeLists.txt 里的
│                               CCAPI_PUBLIC_HEADERS 列表,会 install)
├── ccapi_util.h             ← 轻量工具,公共
├── ccapi_util_internal.h    ← 重量工具(OpenSSL/regex),仅 ccapi 内部 include
└── service/                 ← 全部内部,仅 src/ccapi_session.cpp include

src/
├── ccapi_session.cpp        ← Session / SharedContext 的 Impl 定义,
│                               产出 libccapi_core.a
├── CMakeLists.txt           ← 构建库 + install 规则
└── ccapiConfig.cmake.in     ← find_package(ccapi) 支持
```

**构建方式**:`example/CMakeLists.txt` 通过 `add_subdirectory(../src)` 连入
`ccapi_core` target,20 个 example 全部 `target_link_libraries(... ccapi::ccapi_core)`。
用户 TU 不再看到 service/*.h 与 asio/beast,编译时间从 80s → 5s(~16× 加速)。

**外发布**:`cmake --install build/ccapi_core_build --prefix /opt/ccapi` 导出
`libccapi_core.a` + 15 个公共头 + `ccapiConfig.cmake`,用户项目用
`find_package(ccapi)` + `target_link_libraries(... ccapi::ccapi_core)` 一行集成。

# API 稳定性

**尚未到 v1.0,公共头仍在随重构变动**。破坏性改动(函数签名、类布局)会发生,
建议下游应用**锁定 commit 或 tag**。未来考虑:
- 稳定分支(`stable-v*`) vs 开发主线(`develop`)
- 破坏性改动写进 `CHANGELOG.md`
- 进一步精简公共头(`ccapi_url.h`、`ccapi_market_data_message.h` 等内部暴露可收缩)

# 工程目标

本工程正在进行重构升级,按优先级排列:

1. **性能优化(最高优先级)**:面向低延迟行情 / 执行场景优化热点路径 ——
   减少拷贝与堆分配、优化锁与原子、提升缓存亲和、启用 `TCP_NODELAY` /
   busy-poll、引入 SPSC 队列等;建立可复现的 latency 基准(HdrHistogram),
   任何改动都以 p50/p99/p999 为准,而非主观判断。
2. **现代化写法**:统一按 C++20 及以上标准改写(coroutines、concepts、ranges、
   `std::string_view`、`std::format`、`std::optional`、`std::source_location` 等),
   老式 boost/STL 用法逐步替换;前提是不损害性能目标。
3. **功能扩展**:参考 `/home/shilf/work/nautilus_trader`(Rust/Python 量化交易平台)
   的架构与能力,把对应功能(组合行情、事件总线、策略/执行框架、回测等)
   在本工程中以 C++ 形式扩展实现。修改前先阅读 nautilus_trader 对应模块以对齐设计。

# 已完成的里程碑(供 Claude 了解历史)

- 基准测量:`benchmark/market_data_latency/` 用 HdrHistogram 测 Binance USDS
  合约行情到达间隔。任何性能改动前后跑这个基线对比 p50/p99/p99.9。
- 编译加速:ccache + mold + 工具头拆分 + Session Pimpl,用户 TU 80s → 5s。
- `SharedContext`:公共 API 新增,支持多 Session 共享 io_context + 单线程
  (HFT 常用模式:pin 到 core + isolcpus)。
- install / find_package:生成可发布的 `libccapi_core.a` + 15 公共头。
