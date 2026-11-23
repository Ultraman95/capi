# Subscription failure reporting

When an exchange rejects a subscription, the program receives a `SUBSCRIPTION_STATUS` event whose message type is `SUBSCRIPTION_FAILURE` and whose `ERROR_MESSAGE` holds the exchange's raw error payload, instead of silently receiving nothing.

## Sub-features

- `failure-okx-bad-symbol` — okx rejects an unknown instrument with code `60018`.
- `failure-binance-malformed-symbol` — Binance USDⓈ-M rejects a malformed stream name (`NOPE/USDT`) with `{"error":{"code":2,...}}`.
- `binance-unknown-symbol-is-silent` — Binance USDⓈ-M acks a well-formed but non-existent symbol (`NOPEUSDT`) as a success; documented exchange behavior, recorded so a change on Binance's side is noticed.

## How to get to it (user POV)

- Subscribe with a symbol or channel the exchange rejects; inspect `Event::Type::SUBSCRIPTION_STATUS` messages for `Message::Type::SUBSCRIPTION_FAILURE`.
- `trade/market_data_subscriber <config.ini>` prints these as `[STATUS]` blocks.

## Driving it with ccapi-verify

Preconditions:

- Baseline preconditions from the README hold.
- `$V doctor okx binance-usds-futures` reports both `net ...: OK` (otherwise a "no data" result proves nothing).

- **OKX unknown instrument.** Run `$V drive subscription-failure exchange=okx symbol=NOPE-USDT field=MARKET_DEPTH market_depth_max=5 duration_seconds=6 --expect failure`. `verdict=PASS`, `subscription_failure=1`, `data_rows=0`, `first_error=` contains `"code":"60018"` and `instId:NOPE-USDT doesn't exist`.
- **Binance malformed symbol.** Run `$V drive subscription-failure exchange=binance-usds-futures symbol=NOPE/USDT field=MARKET_DEPTH market_depth_max=5 duration_seconds=8 --expect failure`. `verdict=PASS`, `subscription_failure` ≥ 1, `data_rows=0`, `first_error=` contains `"code":2` and `Invalid request: invalid stream`. Before the fix (2026-11-23): `subscription_failure=0`, `subscription_failure_due_to_connection_failure=4`, `verdict=FAIL`.
- **Binance unknown symbol (exchange behavior).** Run `$V drive subscription-failure exchange=binance-usds-futures symbol=NOPEUSDT field=MARKET_DEPTH market_depth_max=5 duration_seconds=6 --expect failure`. Observed and expected today: `verdict=FAIL`, `subscription_started=1` with `INFO_MESSAGE = {"result":null,"id":0}`, `data_rows=0`. Report it as "exchange limitation", not as a ccapi regression. If it ever turns PASS, Binance started validating symbols; update this file.
- **Proof.** Keep the `run_dir=` and quote the `ERROR_MESSAGE` line from `stdout.txt`.

## Gotchas

- Binance validates only the stream-name format, never the symbol. Docs: "if the `result` received is `null` this means the request sent was a success" (https://raw.githubusercontent.com/binance/binance-spot-api-docs/master/web-socket-streams.md, Live Subscribing/Unsubscribing; the USDⓈ-M page at developers.binance.com says the same and lists no symbol error). A raw client confirmed `LIST_SUBSCRIPTIONS` returns `nopeusdt@depth5@100ms` as active. Detecting an unknown symbol needs REST (`GET /fapi/v1/exchangeInfo`, or `GET /fapi/v1/depth?symbol=` → `-1121 Invalid symbol.`), which ccapi does not do on subscribe; a user can pre-check with `Request::Operation::GET_INSTRUMENT`.
- After Binance's error frame the server closes the socket and ccapi reconnects and resends the same bad SUBSCRIBE about every 2 s, indefinitely (shared reconnect logic in `ccapi_service.h` `onClose`). Expect repeated `SUBSCRIPTION_FAILURE` and `SUBSCRIPTION_FAILURE_DUE_TO_CONNECTION_FAILURE` events in long runs; that loop is a known open issue, not part of this journey.
- `subscription_failure` counts only `SUBSCRIPTION_FAILURE`; connection failures are counted separately in `subscription_failure_due_to_connection_failure` and never satisfy `--expect failure`.
- The OKX journey guards the fix in commit `cc4e3b60` (error events without an `arg` field used to be dropped). A regression there shows as `subscription_failure=0` with `data_rows=0`.
- `--expect failure` still requires a reachable exchange: a network outage also yields zero data, so always run doctor first.
