# Trade stream subscription

A program subscribes to `TRADE` and receives one data element per public trade, each with `LAST_PRICE`, `LAST_SIZE`, `IS_BUYER_MAKER`, and `TRADE_ID`.

## Sub-features

- `trade-okx` — okx spot trades.
- `trade-binance-usds-futures` — Binance USDⓈ-M futures trades.

## How to get to it (user POV)

- `ccapi::Subscription(exchange, symbol, "TRADE")`, then `Session::subscribe`.
- `trade/market_data_subscriber <config.ini>` with `field = TRADE`.

## Driving it with ccapi-verify

Preconditions:

- Baseline preconditions from the README hold.
- `$V doctor okx binance-usds-futures` reports both `net ...: OK`.

- **Binance USDⓈ-M trades.** Run `$V drive trade-stream exchange=binance-usds-futures symbol=BTCUSDT field=TRADE duration_seconds=8`. `verdict=PASS`, `data_rows` in the hundreds (255 on 2026-11-23).
- **OKX trades.** Run `$V drive trade-stream exchange=okx symbol=BTC-USDT field=TRADE duration_seconds=8`. `verdict=PASS`; `stdout.txt` lines look like `2026-11-23T12:04:46.356000000Z {IS_BUYER_MAKER=1, LAST_PRICE=61349.7, LAST_SIZE=0.00120699, TRADE_ID=1089444883}`.
- **Proof.** Keep the `run_dir=`; confirm in `stdout.txt` that `TRADE_ID` values increase and every line has all four fields.

## Gotchas

- OKX spot BTC-USDT trades are sparse compared with Binance futures (10 rows in 8 s on 2026-11-23). Quiet periods can drop below 3 distinct timestamps; retry once, then use `duration_seconds=20` rather than lowering `--min-updates`.
- Depth checks in `summary.txt` (levels, crossed book) only apply to `MARKET_DEPTH`; they are absent for `TRADE`.
