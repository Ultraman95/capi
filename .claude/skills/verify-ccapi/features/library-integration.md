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
- **Header weight.** Read `included_headers_total` (386 on 2026-11-23, none from boost) and `build_seconds` (about 4 s on an idle machine for configure plus a cold compile and link of one TU; parallel builds inflate it). A jump to thousands of headers or tens of seconds means heavy headers leaked into the public headers.
- **Proof.** Keep the `run_dir=`; it holds the generated `CMakeLists.txt`, `main.cpp`, `build.log`, `compile_commands.json`, and `included_headers.txt`.

## Gotchas

- Public headers include no boost since `Session::setTimer` switched to `std::error_code` and `src/ccapiConfig.cmake.in` stopped requiring `~/boost_subset` (2026-11-23). `included_boost_headers` should be `0`; a non-zero value is a regression of the "consumers need no boost" contract. The build tree still lists `~/boost_subset` among `ccapi_core`'s PUBLIC include dirs (`boost_dir_on_consumer_include_path=yes`); whether in-repo tests that include internal headers rely on it has not been checked, so it was left alone. That path is unused by a public-header-only consumer.
- This drives the build tree only. The install-tree path is not yet scripted; verify it by hand when touching install rules in `src/CMakeLists.txt` or `src/ccapiConfig.cmake.in`: `cmake --install build/lib --prefix <dir>`, then configure a consumer with `HOME=/nonexistent cmake ... -DCMAKE_PREFIX_PATH=<dir>` (proves no `~/boost_subset` lookup), build it, and check `ninja -t deps` lists no `/boost/` header.
- `CCAPI_LOG_LEVEL` is a cache variable of `build/lib`; a consumer cannot change the library's log level.
- The consumer must define `ccapi::Logger::logger` itself (`Logger* Logger::logger = nullptr;`), like every example does; omitting it is a link error, not a harness bug.
