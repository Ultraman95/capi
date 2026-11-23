# Subscription failure reporting

When an exchange rejects a subscription, the program receives a `SUBSCRIPTION_STATUS` event whose message type is `SUBSCRIPTION_FAILURE` and whose `ERROR_MESSAGE` holds the exchange's raw error payload, instead of silently receiving nothing.

## Sub-features

- `failure-okx-bad-symbol` — okx rejects an unknown instrument with code `60018`.
- `failure-binance-bad-symbol` — Binance USDⓈ-M with an unknown symbol (currently silent; see Gotchas).

## How to get to it (user POV)

- Subscribe with a symbol or channel the exchange rejects; inspect `Event::Type::SUBSCRIPTION_STATUS` messages for `Message::Type::SUBSCRIPTION_FAILURE`.
- `trade/market_data_subscriber <config.ini>` prints these as `[STATUS]` blocks.

## Driving it with ccapi-verify

Preconditions:

- Baseline preconditions from the README hold.
- `$V doctor okx binance-usds-futures` reports both `net ...: OK` (otherwise a "no data" result proves nothing).

- **OKX unknown instrument.** Run `$V drive subscription-failure exchange=okx symbol=NOPE-USDT field=MARKET_DEPTH market_depth_max=5 duration_seconds=6 --expect failure`. `verdict=PASS`, `subscription_failure=1`, `data_rows=0`, `first_error=` contains `"code":"60018"` and `instId:NOPE-USDT doesn't exist`.
- **Binance unknown symbol.** Run `$V drive subscription-failure exchange=binance-usds-futures symbol=NOPEUSDT field=MARKET_DEPTH market_depth_max=5 duration_seconds=6 --expect failure`. Expected by the feature: `verdict=PASS`. Observed on 2026-11-23: `verdict=FAIL`, `reason=no SUBSCRIPTION_FAILURE event`, `data_rows=0`.
- **Proof.** Keep the `run_dir=` and quote the `ERROR_MESSAGE` line from `stdout.txt`.

## Gotchas

- The Binance journey fails today: the program gets neither data nor a failure event. Whether Binance returns any error for a non-existent stream, and whether ccapi drops it, is not yet investigated. Report it as a FAIL with that reason; do not remove the journey.
- The OKX journey guards the fix in commit `cc4e3b60` (error events without an `arg` field used to be dropped). A regression there shows as `subscription_failure=0` with `data_rows=0`.
- `--expect failure` still requires a reachable exchange: a network outage also yields zero data, so always run doctor first.
