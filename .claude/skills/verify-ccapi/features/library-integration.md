# Library integration

A downstream CMake project adds `find_package(ccapi)` and `target_link_libraries(<target> PRIVATE ccapi::ccapi_core)`, includes only `ccapi_cpp/ccapi_session.h`, compiles without pulling in asio/beast or any `service/*.h` header, and its program receives live market data.

## Sub-features

- `integration-build-tree` — consume the build tree `build/lib` via `ccapi_DIR`.
- `integration-thin-headers` — the consumer TU includes no asio/beast and no `ccapi_cpp/service/` header.
- `integration-runtime` — the linked consumer receives market-depth rows with no failures, and `setTimer` fires and reports cancellation through `std::error_code`.
- `integration-install-no-boost` — an installed copy is consumed via `CMAKE_PREFIX_PATH` on a toolchain that cannot see any boost.

## How to get to it (user POV)

- In a user project: `find_package(ccapi REQUIRED)` with `-Dccapi_DIR=<repo>/build/lib` (build tree) or `CMAKE_PREFIX_PATH=<install prefix>` after `cmake --install build/lib --prefix <prefix>`.
- `trade/CMakeLists.txt` is the in-repo example of the same pattern.

## Driving it with ccapi-verify

Preconditions:

- Baseline preconditions from the README hold (`lib: up to date`).
- `$V doctor okx` reports `net okx: OK`.

- **Build and run consumer.** Run `$V consumer okx BTC-USDT`. `build_exit_code=0`, `included_asio_beast_headers=0`, `included_ccapi_service_headers=0`, `run_exit_code=0`, `timer_cancel_error=125 (Operation canceled)`, `consumer_rows=<positive> consumer_failures=0 timer_fired=1 timer_canceled=1`, `verdict=PASS`.
- **Installed, boost hidden.** Run `$V consumer-install okx BTC-USDT`. `boost_hidden=yes`, `build_exit_code=0`, `included_boost_headers=0`, the same runtime lines, `verdict=PASS`; `canary.log` shows `fatal error: boost/version.hpp: No such file or directory`.
- **Header weight.** Read `included_headers_total` (386 on 2026-11-23, none from boost) and `build_seconds` (about 4 s on an idle machine for configure plus a cold compile and link of one TU; parallel builds inflate it). A jump to thousands of headers or tens of seconds means heavy headers leaked into the public headers.
- **Proof.** Keep the `run_dir=`; it holds the generated `CMakeLists.txt`, `main.cpp`, `build.log`, `compile_commands.json`, and `included_headers.txt`.

## Gotchas

- Public headers include no boost since `Session::setTimer` switched to `std::error_code` and `src/ccapiConfig.cmake.in` stopped requiring `~/boost_subset` (2026-11-23). `included_boost_headers` should be `0`; a non-zero value is a regression of the "consumers need no boost" contract. The build tree still lists `~/boost_subset` among `ccapi_core`'s PUBLIC include dirs (`boost_dir_on_consumer_include_path=yes`); whether in-repo tests that include internal headers rely on it has not been checked, so it was left alone. That path is unused by a public-header-only consumer.
- Negative control (2026-11-23): re-adding `#include <boost/system/error_code.hpp>` to `ccapi_session.h` makes `consumer-install` fail at compile time with `fatal error: boost/system/error_code.hpp: No such file or directory`, while plain `consumer` still builds (it sees `~/boost_subset` through the build tree). Only `consumer-install` proves the no-boost contract.
- `boost_hidden=no (leaked via include dir: ...)` means some imported target re-added a directory containing `boost/`. The first version of this check missed `/usr/include` coming from `OpenSSL::SSL`; the harness now redirects `OPENSSL_INCLUDE_DIR` and `ZLIB_INCLUDE_DIR` to the filtered copy.
- `CCAPI_LOG_LEVEL` is a cache variable of `build/lib`; a consumer cannot change the library's log level.
- The consumer must define `ccapi::Logger::logger` itself (`Logger* Logger::logger = nullptr;`), like every example does; omitting it is a link error, not a harness bug.
