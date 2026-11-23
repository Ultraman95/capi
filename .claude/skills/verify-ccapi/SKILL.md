---
name: verify-ccapi
description: Prove ccapi (C++ crypto exchange connectivity library, static lib libccapi_core.a) works the way a downstream program uses it — build the lib, subscribe to live public market data on real exchanges through trade/market_data_subscriber, and link an external find_package(ccapi) consumer. Use after changing anything under include/ccapi_cpp/ or src/, after merging upstream, or whenever a claim like "OKX depth works" or "the lib still links" needs evidence instead of unit tests.
---

# verify-ccapi

ccapi is a library. Its users are C++ programs that create a `ccapi::Session`, subscribe or send requests, and receive `Event`s in a handler. The real user path is therefore: build `build/lib/libccapi_core.a` → link a program against it → connect to a live exchange → observe events. Unit tests (`test/`) never touch the network and do not count as proof here; the OKX silent-subscription bug and the KuCoin first-connect TLS bug both passed all unit tests.

Primary surface: **live market-data subscription** via `trade/market_data_subscriber` (reads an ini, subscribes, prints `[STATUS]` events and one line per data element, exits after `duration_seconds`).
Secondary surface: **library integration** via an external `find_package(ccapi)` project.
Not covered (no harness yet): REST requests (`Request` / `sendRequest`), execution management (needs API keys and places real orders — never drive it from this skill), FIX, `binding/` (out of scope per CLAUDE.md).

All commands below run from the repo root. The harness is `.claude/skills/verify-ccapi/ccapi-verify`; set `V=.claude/skills/verify-ccapi/ccapi-verify` and run `$V help` for the full usage.

## Launch

There is no server. "Launch" means both build trees are current:

```bash
V=.claude/skills/verify-ccapi/ccapi-verify
$V build
```

Ready = last line `READY subscriber=<repo>/trade/build/market_data_subscriber/market_data_subscriber` and exit 0. It configures `build/lib` (from `src/`) and `trade/build` (from `trade/`) on first use, then runs `ninja` on both. A cold `build/lib` compile of `src/ccapi_session.cpp` takes several minutes (all 30 exchanges in one TU); an incremental one after a `service/*.h` change also recompiles that whole TU. Run it with a long timeout or in the background.

Build prerequisites (from CLAUDE.md): boost subset at `~/boost_subset`, OpenSSL, zlib, ninja; ccache and mold are picked up automatically. Library log level is a configure-time cache variable: `cmake -S src -B build/lib -DCCAPI_LOG_LEVEL=TRACE` then `$V build` — remember to reset it (`-DCCAPI_LOG_LEVEL=`) afterwards, because it changes the lib every later run links.

Teardown: nothing to stop. Drives are foreground processes that exit on their own.

## Doctor

```bash
$V doctor [exchange ...]      # default: okx binance-usds-futures kucoin bybit
```

Read-only. Prints `git_head`, `git_dirty_tracked_files`, lib/subscriber mtimes, then:

- `lib: up to date` / `STALE` — `ninja -n` on `build/lib`. STALE means the binary you would drive does not contain your source edits. Run `$V build` before trusting any drive.
- `subscriber: up to date` / `STALE` — the subscriber needs a relink against the current lib.
- `net <exchange>: OK` / `UNREACHABLE http=<code>` — that exchange's public REST host. UNREACHABLE (timeouts, 403/451 geo-blocks) means a drive failure on that exchange is environmental, not a code regression.

Last line `DOCTOR OK` (exit 0) or `DOCTOR FOUND PROBLEMS` (exit 1). Run it first, and again whenever a drive fails unexpectedly.

## Drive

```bash
$V drive <feature-id> key=value ... [--expect data|failure] [--min-updates N]
```

Keys are exactly the ini keys of `trade/market_data_subscriber` (documented in `trade/market_data_subscriber/config.example.ini`): `exchange`, `symbol`, `field` (`MARKET_DEPTH`, `TRADE`, `BBO`, …), `market_depth_max`, `conflate_interval_milliseconds`, `instrument_type`, `duration_seconds` (default 10; must be > 0). Symbols are passed to the exchange verbatim — use each exchange's own spelling (`BTC-USDT` for okx/kucoin, `BTCUSDT` for binance/bybit, `BTC` for hyperliquid).

Each drive is one subscriber process with its own config and run dir, so drives can run in parallel. Do not run `$V build` while another agent is mid-build on the same tree.

Verdict rules (exit 0 = PASS, 1 = FAIL, 2 = harness misuse):

- `--expect data` (default): subscriber exit 0, no `SUBSCRIPTION_FAILURE`, at least `--min-updates` (default 3) distinct update timestamps; for `MARKET_DEPTH` also no crossed book (best bid ≥ best ask within one update) and no update with more levels than `market_depth_max`.
- `--expect failure`: at least one `SUBSCRIPTION_FAILURE` event and zero data rows.

Feature recipes live in [features/README.md](features/README.md). Read the matching feature file before driving.

## Library integration

```bash
$V consumer [exchange symbol]   # default: okx BTC-USDT
```

Generates a fresh CMake project in the scratch dir that does `find_package(ccapi)` + `target_link_libraries(consumer PRIVATE ccapi::ccapi_core)` against `build/lib`, builds it, runs it for 8 s, and records the headers the consumer TU actually included (`ninja -t deps` → `included_headers.txt`). PASS needs a clean build, `included_asio_beast_headers=0` and `included_ccapi_service_headers=0` (the thin-public-header contract in CLAUDE.md: user TUs never see `service/*.h` or asio/beast), and `consumer_rows>0 consumer_failures=0`. `included_boost_headers` and `boost_dir_on_consumer_include_path` are informational; see features/library-integration.md.

## Evidence

Every drive and consumer run writes `.verify-artifacts/<UTC>-<feature-id>-<pid>/` (gitignored, override with `CCAPI_VERIFY_ARTIFACTS`):

- `config.ini` — the exact input (the action).
- `stdout.txt` / `stderr.txt` — every `[STATUS]` event with its raw exchange payload (`INFO_MESSAGE` / `ERROR_MESSAGE`) and every data element line (the resulting state).
- `meta.txt` — feature id, command, start/finish time, `git_head`, dirty count, lib/subscriber mtimes. Proof is only valid for the revision recorded here; if `git_dirty_tracked_files` > 0, say the proof covers uncommitted edits.
- `summary.txt` — counts, depth checks, `verdict=`, `reason=`.
- consumer runs add `CMakeLists.txt`, `main.cpp`, `build.log`, `compile_commands.json`, `included_headers.txt`.

Proof standards:

- Drive the real path: a linked program against a real exchange. No mocks, no unit tests, no calling service internals.
- Report the run dir, the `config.ini` keys, and the `summary.txt` lines. Quote one or two raw `stdout.txt` lines (a `[STATUS]` payload or data line) when the claim is about exchange behaviour (channel chosen, error code).
- A change to one exchange's service header is proven only on that exchange. A change to shared code (`ccapi_service.h`, `ccapi_market_data_service.h`, `ccapi_session.cpp`, connection classes) needs at least okx, binance-usds-futures, and kucoin drives.
- An exchange reported UNREACHABLE by doctor is "not verified (network)", never "verified".
- Never lower `--min-updates`, shorten the journey, or switch to an easier symbol to turn a FAIL into a PASS. Record the failure and its reason.

## Cleanup

```bash
$V cleanup
```

Kills any subscriber/consumer process whose recorded PID is still alive (only processes this harness started; it never kills by name), removes the scratch dir (`${TMPDIR:-/tmp}/ccapi-verify-scratch`, consumer sources and builds). It never deletes `.verify-artifacts/`. Delete old run dirs by hand when they are no longer needed.

## Helpers

- `.claude/skills/verify-ccapi/ccapi-verify` — the only helper; `build | doctor | drive | consumer | cleanup | help`. Bash, needs `cmake`, `ninja`, `curl`, `awk`, `bc`, `timeout`.
