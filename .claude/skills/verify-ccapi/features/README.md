# ccapi verification map

This directory is the maintained source for verifying the user-facing behavior of ccapi. A "user" is a C++ program linked against `libccapi_core.a`. Read this index, then use the matching feature file as the recipe.

## Baseline preconditions

- `V=.claude/skills/verify-ccapi/ccapi-verify`, commands run from the repo root.
- `$V build` ends with `READY subscriber=...`.
- `$V doctor <exchanges you will drive>` ends with `DOCTOR OK`; in particular `lib: up to date` so the drive exercises your current sources.
- Outbound HTTPS/WSS to the exchanges is available. No API keys are needed or used.

## Driving conventions

- Every journey is one `$V drive <feature-id> ...` or `$V consumer ...` call. Use the feature id from the file name as `<feature-id>` so run dirs are self-describing.
- Treat every command as literal; symbols are exchange-specific spellings passed through verbatim.
- Use `duration_seconds` ≥ 8 (KuCoin needs ~1.5 s for its token handshake before the socket opens).
- Live markets vary. One FAIL from a thin market or a network blip is retried once; a second FAIL is reported.

## Proof and skip reporting

- Cite the run dir, the drive command, and the `summary.txt` verdict lines.
- For exchange-behavior claims, quote the raw `[STATUS]` payload line from `stdout.txt`.
- An exchange that doctor reports UNREACHABLE is "not verified (network)".
- Do not report an exchange as verified because a different exchange passed.

## Feature entry contract

Each feature file has an H1 title and one paragraph, then exactly four H2 sections in this order: `Sub-features`, `How to get to it (user POV)`, `Driving it with ccapi-verify` (starts with `Preconditions:`, then labeled bullets pairing action, exact command, observable result), `Gotchas`.

## Features

- [Market depth subscription](./market-depth.md) — `MARKET_DEPTH` on okx (1/5/20 levels), binance-usds-futures, kucoin, bybit, hyperliquid; level cap and uncrossed-book checks.
- [Trade stream subscription](./trade-stream.md) — `TRADE` ticks with price, size, side, trade id.
- [Subscription failure reporting](./subscription-failure.md) — a bad subscription surfaces as a `SUBSCRIPTION_FAILURE` event instead of silence.
- [Library integration](./library-integration.md) — an external CMake project consumes the lib with `find_package(ccapi)` and never sees boost.

## Not yet mapped

- REST market-data requests (`Request::Operation::GET_RECENT_TRADES`, `GENERIC_PUBLIC_REQUEST`): only hard-coded examples under `example/` exist; no configurable driver.
- Execution management and FIX: require credentials and send real orders. Do not drive from this skill.
- Multi-session `SharedContext`: only `example/use_multiple_sessions`.
