# Library integration

A downstream CMake project adds `find_package(ccapi)` and `target_link_libraries(<target> PRIVATE ccapi::ccapi_core)`, includes only `ccapi_cpp/ccapi_session.h`, compiles without pulling in asio/beast or any `service/*.h` header, and its program receives live market data.

## Sub-features

- `integration-build-tree` — consume the build tree `build/lib` via `ccapi_DIR`.
- `integration-thin-headers` — the consumer TU includes no asio/beast and no `ccapi_cpp/service/` header.
- `integration-runtime` — the linked consumer receives market-depth rows with no failures.

## How to get to it (user POV)

- In a user project: `find_package(ccapi REQUIRED)` with `-Dccapi_DIR=<repo>/build/lib` (build tree) or `CMAKE_PREFIX_PATH=<install prefix>` after `cmake --install build/lib --prefix <prefix>`.
- `trade/CMakeLists.txt` is the in-repo example of the same pattern.

## Driving it with ccapi-verify

Preconditions:

- Baseline preconditions from the README hold (`lib: up to date`).
- `$V doctor okx` reports `net okx: OK`.

- **Build and run consumer.** Run `$V consumer okx BTC-USDT`. `build_exit_code=0`, `included_asio_beast_headers=0`, `included_ccapi_service_headers=0`, `run_exit_code=0`, `consumer_rows=<positive> consumer_failures=0`, `verdict=PASS`.
- **Header weight.** Read `included_headers_total` (437 on 2026-11-23, of which 48 boost) and `build_seconds` (about 4 s for configure plus a cold compile and link of one TU). A jump to thousands of headers or tens of seconds means heavy headers leaked into the public headers.
- **Proof.** Keep the `run_dir=`; it holds the generated `CMakeLists.txt`, `main.cpp`, `build.log`, `compile_commands.json`, and `included_headers.txt`.

## Gotchas

- The public API still exposes `boost::system::error_code` (`Session::setTimer`'s error handler, `include/ccapi_cpp/ccapi_session.h:12`), so a consumer TU includes ~48 boost.system/config headers. The build tree passes `~/boost_subset` to consumers as a PUBLIC include dir, which hides this. The install tree (`INSTALL_INTERFACE`) does not ship boost, so an installed-tree consumer compiles only if boost headers are found elsewhere; this machine has `/usr/include/boost`, which masks the gap. Treat it as a known open issue, not a pass condition to tighten silently.
- This drives the build tree only. The install-tree path (`cmake --install build/lib --prefix <dir>` then `CMAKE_PREFIX_PATH=<dir>`) is not yet scripted; verify it by hand when touching install rules in `src/CMakeLists.txt` or `src/ccapiConfig.cmake.in`.
- `CCAPI_LOG_LEVEL` is a cache variable of `build/lib`; a consumer cannot change the library's log level.
- The consumer must define `ccapi::Logger::logger` itself (`Logger* Logger::logger = nullptr;`), like every example does; omitting it is a link error, not a harness bug.
