# 构建说明

- CMake 默认从 `~/boost_subset` 读取 boost(bcp 抽取的 asio+beast 子集),
  **不会**下载完整 boost 压缩包。子集缺失时 cmake 会直接报错,
  并给出重新生成的 `bcp` 命令。可用 `-DBOOST_INCLUDE_DIR=<path>` 覆盖默认路径。

# 关注范围

- `binding/`(SWIG 生成的 Python/Java/C#/Go/JS 包装层)不是当前关注点,
  不需要修改 / 构建 / 调试;README 中 `### non-C++` 章节同理。

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
