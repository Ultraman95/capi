# Market depth subscription

A program subscribes to `MARKET_DEPTH` for an exchange and symbol, receives `SUBSCRIPTION_STARTED`, then a stream of order-book updates; each update carries at most `MARKET_DEPTH_MAX` bid levels and ask levels, and the best bid stays below the best ask.

## Sub-features

- `depth-okx-5` — okx top 5 levels without credentials (public `books5` channel).
- `depth-okx-20` — okx more than 5 levels without credentials (public `books` channel).
- `depth-okx-1` — okx single level (`bbo-tbt` channel).
- `depth-binance-usds-futures` — Binance USDⓈ-M futures top 5.
- `depth-kucoin` — KuCoin spot top 5 via the dynamic-URL token handshake.
- `depth-bybit-linear` — Bybit linear perpetual top 5 with `instrument_type`.
- `depth-hyperliquid` — Hyperliquid perp top 5.

## How to get to it (user POV)

- `ccapi::Subscription(exchange, symbol, "MARKET_DEPTH", "MARKET_DEPTH_MAX=<n>&CONFLATE_INTERVAL_MILLISECONDS=<ms>")`, optionally `setInstrumentType(...)`, then `Session::subscribe`.
- `trade/market_data_subscriber <config.ini>` with `field = MARKET_DEPTH`.

## Driving it with ccapi-verify

Preconditions:

- Baseline preconditions from the README hold.
- `$V doctor okx binance-usds-futures kucoin bybit hyperliquid` reports `net ...: OK` for each exchange you drive.

- **OKX 5 levels.** Run `$V drive market-depth exchange=okx symbol=BTC-USDT field=MARKET_DEPTH market_depth_max=5 duration_seconds=8`. `verdict=PASS`, `subscription_started=1`, `max_bid_levels_per_update=5`, `max_ask_levels_per_update=5`, `crossed_book_updates=0`; `stdout.txt` has `INFO_MESSAGE = {"event":"subscribe","arg":{"channel":"books5","instId":"BTC-USDT"}...`.
- **OKX 20 levels.** Run `$V drive market-depth exchange=okx symbol=BTC-USDT field=MARKET_DEPTH market_depth_max=20 duration_seconds=8`. `verdict=PASS`, `max_bid_levels_per_update=20`, `max_ask_levels_per_update=20`; the `[STATUS]` payload names channel `books`.
- **OKX 1 level.** Run `$V drive market-depth exchange=okx symbol=BTC-USDT field=MARKET_DEPTH market_depth_max=1 duration_seconds=8`. `verdict=PASS`, both level maxima `1`; the payload names channel `bbo-tbt`.
- **Binance USDⓈ-M.** Run `$V drive market-depth exchange=binance-usds-futures symbol=BTCUSDT field=MARKET_DEPTH market_depth_max=5 duration_seconds=8`. `verdict=PASS`, both level maxima `5`.
- **KuCoin.** Run `$V drive market-depth exchange=kucoin symbol=BTC-USDT field=MARKET_DEPTH market_depth_max=5 duration_seconds=8`. `verdict=PASS`, both level maxima `5`; the first update arrives within ~2 s of start (`first_update` vs `meta.txt` `started_utc`). A ~7 s gap means the first-connect TLS regression is back.
- **Bybit linear.** Run `$V drive market-depth exchange=bybit symbol=BTCUSDT instrument_type=linear field=MARKET_DEPTH market_depth_max=5 duration_seconds=8`. `verdict=PASS`, both level maxima `5`.
- **Hyperliquid.** Run `$V drive market-depth exchange=hyperliquid symbol=BTC field=MARKET_DEPTH market_depth_max=5 duration_seconds=20`. `verdict=PASS`, both level maxima `5`.
- **Proof.** For each drive, keep the printed `run_dir=`; its `summary.txt` and the `[STATUS]` payload in `stdout.txt` are the evidence.

## Gotchas

- OKX `books50-l2-tbt` / `books-l2-tbt` need a logged-in VIP4+ account. Without credentials ccapi picks `bbo-tbt` (1 level), `books5` (≤5), or `books` (>5). The comment in `trade/market_data_subscriber/config.example.ini` saying OKX needs `conflate_interval_milliseconds = 100` predates that fix and is no longer required.
- Hyperliquid delivered only 2–3 distinct updates per 8–20 s for BTC on 2026-11-23 (an 8 s drive failed `--min-updates 3` with 2 updates). Whether that is the exchange's push cadence or ccapi dropping changes is not yet investigated; drive it for 20 s and treat the low rate as an open question, not a pass criterion to loosen.
- Bybit without `instrument_type` also passed on 2026-11-23 (the URL ends in `/v5/public/`), contrary to the config file comment. Always pass `instrument_type` anyway; the unqualified form is undocumented behavior.
- Each element line is one price level; `distinct_update_timestamps` counts updates. Levels within one update share the exchange timestamp.
- Thin symbols can legitimately produce fewer than 3 updates in 8 s. Use BTC pairs for regression checks.
