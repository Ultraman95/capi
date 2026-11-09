#!/usr/bin/env python3
"""审计 ccapi 各交易所 API 版本是否最新(实时探测)。

每家交易所发两个 HTTP 请求:
  - 「当前路径」探活:  hit ccapi 用的版本下一个已知端点,期望 200(确认还活着)
  - 「下版本」探活:   hit 把版本号 +1 后的同名端点,期望 4xx(若 200 = 有更新版本)

判定:
  ✅ 当前 200 且下版本 4xx           ccapi 路径就是最新,且仍在维护
  ⚠️  当前 200 且下版本 200           ccapi 落后,有更新版本可用
  ❌ 当前不是 200                    ccapi 路径已失效或被弃用
  · 未配置探测                       该交易所没人工建模(版本号非 /vN 形式或独立 API)

不带 /vN 的交易所(Coinbase Exchange、Kraken、Crypto.com 等)只做"当前路径
探活",落后/最新与否依赖人工注释(见 NOTES)。

用法:
    python3 scripts/audit_api_versions.py                 # 仅汇总
    python3 scripts/audit_api_versions.py --details       # 带每家具体 HTTP 状态码
"""

from __future__ import annotations

import argparse
import base64
import http.client
import os
import socket
import ssl
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass, field
from urllib.parse import urlparse

UA = "ccapi-version-audit/2.0"
TIMEOUT = 6  # 秒

# 状态
OK = "OK"           # current 200, next 4xx
BEHIND = "BEHIND"   # current 200, next 200(有新版本)
DEAD = "DEAD"       # current 不是 200(ccapi 路径已失效)
PARTIAL = "PARTIAL" # 单 endpoint 设计,有"独立 API 未接"的副产品(人工标记)
NO_PROBE = "NO_PROBE"  # 不适用版本号探测的设计(非 /vN 形式)

ICON = {OK: "✅", BEHIND: "⚠️", DEAD: "❌", PARTIAL: "⚠️", NO_PROBE: "·"}


@dataclass
class Check:
    exchange: str
    ccapi_ver: str            # 文档:ccapi 实际拼接的 REST 版本路径
    current_url: str = ""     # REST 当前路径探活(空 = 不探)
    next_url: str = ""        # REST 下版本路径探活(空 = 不探)
    ws_current_url: str = ""  # WS 当前路径探活(wss:// 全 URL,空 = 不探)
    ws_next_url: str = ""     # WS 下版本路径探活(空 = 不探)
    static_status: str = ""   # 强制静态状态(用于 NO_PROBE / PARTIAL)
    note: str = ""            # 人工备注


# ---------------------------------------------------------------------------
# Probe 表 —— 每家挑一个公开、轻量、不要鉴权的 endpoint
# ---------------------------------------------------------------------------
CHECKS: list[Check] = [
    # ===== /vN style: 实时探测 =====
    Check("binance",                   "api.binance.com/api/v3",
          "https://api.binance.com/api/v3/ping",
          "https://api.binance.com/api/v4/ping"),
    Check("binance-us",                "api.binance.us/api/v3",
          "https://api.binance.us/api/v3/ping",
          "https://api.binance.us/api/v4/ping"),
    Check("binance-usds-futures",      "fapi.binance.com/fapi/v1",
          "https://fapi.binance.com/fapi/v1/ping",
          "https://fapi.binance.com/fapi/v2/ping"),
    Check("binance-coin-futures",      "dapi.binance.com/dapi/v1",
          "https://dapi.binance.com/dapi/v1/ping",
          "https://dapi.binance.com/dapi/v2/ping"),
    Check("okx",                       "www.okx.com/api/v5",
          "https://www.okx.com/api/v5/public/time",
          "https://www.okx.com/api/v6/public/time",
          ws_current_url="wss://ws.okx.com:8443/ws/v5/public",
          ws_next_url="wss://ws.okx.com:8443/ws/v6/public"),
    Check("bybit",                     "api.bybit.com/v5",
          "https://api.bybit.com/v5/market/time",
          "https://api.bybit.com/v6/market/time",
          ws_current_url="wss://stream.bybit.com/v5/public/spot",
          ws_next_url="wss://stream.bybit.com/v6/public/spot"),
    Check("bitget",                    "api.bitget.com/api/v2",
          "https://api.bitget.com/api/v2/public/time",
          "https://api.bitget.com/api/v3/public/time",
          ws_current_url="wss://ws.bitget.com/v2/ws/public",
          ws_next_url="wss://ws.bitget.com/v3/ws/public"),
    Check("bitget-futures",            "api.bitget.com/api/v2/mix",
          "https://api.bitget.com/api/v2/mix/market/contracts?productType=USDT-FUTURES",
          "https://api.bitget.com/api/v3/mix/market/contracts?productType=USDT-FUTURES",
          ws_current_url="wss://ws.bitget.com/v2/ws/public",
          ws_next_url="wss://ws.bitget.com/v3/ws/public"),
    Check("kucoin",                    "api.kucoin.com/api/v1",
          "https://api.kucoin.com/api/v1/timestamp",
          "https://api.kucoin.com/api/v2/timestamp"),
    Check("kucoin-futures",            "api-futures.kucoin.com/api/v1",
          "https://api-futures.kucoin.com/api/v1/timestamp",
          "https://api-futures.kucoin.com/api/v2/timestamp"),
    Check("gateio",                    "api.gateio.ws/api/v4",
          "https://api.gateio.ws/api/v4/spot/time",
          "https://api.gateio.ws/api/v5/spot/time",
          ws_current_url="wss://api.gateio.ws/ws/v4/",
          ws_next_url="wss://api.gateio.ws/ws/v5/"),
    Check("gateio-perpetual-futures",  "fx-api.gateio.ws/api/v4",
          "https://fx-api.gateio.ws/api/v4/futures/usdt/contracts",
          "https://fx-api.gateio.ws/api/v5/futures/usdt/contracts",
          ws_current_url="wss://fx-ws.gateio.ws/v4/ws/usdt",
          ws_next_url="wss://fx-ws.gateio.ws/v5/ws/usdt"),
    Check("mexc",                      "api.mexc.com/api/v3",
          "https://api.mexc.com/api/v3/ping",
          "https://api.mexc.com/api/v4/ping"),
    Check("mexc-futures",              "contract.mexc.com/api/v1",
          "https://contract.mexc.com/api/v1/contract/ping",
          "https://contract.mexc.com/api/v2/contract/ping"),
    Check("kraken-futures",            "futures.kraken.com/derivatives/api/v3",
          "https://futures.kraken.com/derivatives/api/v3/instruments",
          "https://futures.kraken.com/derivatives/api/v4/instruments",
          ws_current_url="wss://futures.kraken.com/ws/v1",
          ws_next_url="wss://futures.kraken.com/ws/v2"),
    Check("bitfinex",                  "api-pub.bitfinex.com/v2",
          "https://api-pub.bitfinex.com/v2/platform/status",
          "https://api-pub.bitfinex.com/v3/platform/status",
          ws_current_url="wss://api-pub.bitfinex.com/ws/2",
          ws_next_url="wss://api-pub.bitfinex.com/ws/3"),
    Check("bitstamp",                  "www.bitstamp.net/api/v2",
          "https://www.bitstamp.net/api/v2/trading-pairs-info/",
          "https://www.bitstamp.net/api/v3/trading-pairs-info/"),
    Check("bitmex",                    "www.bitmex.com/api/v1",
          "https://www.bitmex.com/api/v1/instrument/active",
          "https://www.bitmex.com/api/v2/instrument/active"),
    Check("deribit",                   "www.deribit.com/api/v2",
          "https://www.deribit.com/api/v2/public/get_time",
          "https://www.deribit.com/api/v3/public/get_time",
          ws_current_url="wss://www.deribit.com/ws/api/v2",
          ws_next_url="wss://www.deribit.com/ws/api/v3"),
    Check("gemini",                    "api.gemini.com/v1",
          "https://api.gemini.com/v1/symbols",
          "https://api.gemini.com/v2/symbols",
          ws_current_url="wss://api.gemini.com/v1/marketdata/btcusd",
          ws_next_url="wss://api.gemini.com/v2/marketdata/btcusd"),
    Check("ascendex",                  "ascendex.com/api/pro/v1",
          "https://ascendex.com/api/pro/v1/cash/products",
          "https://ascendex.com/api/pro/v2/cash/products",
          ws_current_url="wss://ascendex.com/api/pro/v1/stream",
          ws_next_url="wss://ascendex.com/api/pro/v2/stream"),
    Check("bitmart",                   "api-cloud.bitmart.com/spot/v1",
          "https://api-cloud.bitmart.com/spot/v1/symbols",
          "https://api-cloud.bitmart.com/spot/v2/symbols"),
    Check("huobi",                     "api.huobi.pro/v1",
          "https://api.huobi.pro/v1/common/symbols",
          "https://api.huobi.pro/v2/common/symbols"),
    Check("huobi-usdt-swap",           "api.hbdm.com/linear-swap-api/v1",
          "https://api.hbdm.com/linear-swap-api/v1/swap_contract_info",
          "https://api.hbdm.com/linear-swap-api/v2/swap_contract_info"),
    Check("huobi-coin-swap",           "api.hbdm.com/swap-api/v1",
          "https://api.hbdm.com/swap-api/v1/swap_contract_info",
          "https://api.hbdm.com/swap-api/v2/swap_contract_info"),

    # ===== 非 /vN style:不适合自动探测,做静态注释 =====
    Check("kraken",                    "api.kraken.com/0",
          "https://api.kraken.com/0/public/Time", "",
          ws_current_url="wss://ws.kraken.com",
          ws_next_url="wss://ws.kraken.com/v2",
          static_status=NO_PROBE,
          note="Kraken REST 用 /0/(数字 0 不是版本号),没有 /v1/ 可探;WS V2 已上线"),
    Check("cryptocom",                 "api.crypto.com/exchange/v1",
          "https://api.crypto.com/exchange/v1/public/get-instruments", "",
          ws_current_url="wss://stream.crypto.com/exchange/v1/market",
          ws_next_url="wss://stream.crypto.com/exchange/v2/market",
          static_status=PARTIAL,
          note="V1 是 Crypto.com Exchange 当前主版本(非 'V1=旧版');统一 API,perp/spot/futures 共 endpoint"),
    Check("coinbase",                  "api.exchange.coinbase.com",
          "https://api.exchange.coinbase.com/products", "",
          static_status=PARTIAL,
          note="ccapi 接的是 Coinbase Exchange (旧 Pro);Advanced Trade(api.coinbase.com/api/v3) + International perp 是另两条独立 API,均未接"),
]


def probe(url: str) -> tuple[str, bool]:
    """对 url 发 GET,返回 ('HTTP NNN' 字符串 + 内容是否像 JSON API)。
    JSON 判定:Content-Type 含 json,或响应体首个非空白字符是 '{' 或 '['。
    用来过滤"返回 200 但实际是 HTML 404 SPA 页"的假阳性。
    """
    if not url:
        return "-", False
    u = urlparse(url)
    host = u.hostname or ""
    port = u.port or (443 if u.scheme == "https" else 80)
    path = u.path + (("?" + u.query) if u.query else "")
    if not path:
        path = "/"
    try:
        if u.scheme == "https":
            conn = http.client.HTTPSConnection(host, port, timeout=TIMEOUT)
        else:
            conn = http.client.HTTPConnection(host, port, timeout=TIMEOUT)
        conn.request("GET", path, headers={"User-Agent": UA, "Connection": "close"})
        resp = conn.getresponse()
        code = resp.status
        ctype = (resp.getheader("Content-Type") or "").lower()
        body_head = resp.read(256)
        conn.close()
        json_like = "json" in ctype or body_head.lstrip().startswith((b"{", b"["))
        return f"HTTP {code}", json_like
    except (socket.timeout, TimeoutError):
        return "timeout", False
    except Exception as e:
        return f"err:{type(e).__name__}", False


def ws_handshake(url: str) -> str:
    """对 wss:// URL 做 WebSocket Upgrade 握手,返回响应首行(如 'HTTP/1.1 101 ...')。
    成功 = 服务端接受 ws upgrade 即此 URL 是有效 WS endpoint。
    """
    if not url:
        return "-"
    u = urlparse(url)
    host = u.hostname or ""
    port = u.port or (443 if u.scheme == "wss" else 80)
    path = u.path or "/"
    if u.query:
        path += "?" + u.query
    key = base64.b64encode(os.urandom(16)).decode()
    req = (
        f"GET {path} HTTP/1.1\r\n"
        f"Host: {host}\r\n"
        f"Upgrade: websocket\r\n"
        f"Connection: Upgrade\r\n"
        f"Sec-WebSocket-Key: {key}\r\n"
        f"Sec-WebSocket-Version: 13\r\n"
        f"User-Agent: {UA}\r\n\r\n"
    )
    try:
        sock = socket.create_connection((host, port), timeout=TIMEOUT)
        if u.scheme == "wss":
            ctx = ssl.create_default_context()
            ctx.check_hostname = False
            ctx.verify_mode = ssl.CERT_NONE
            sock = ctx.wrap_socket(sock, server_hostname=host)
        sock.settimeout(TIMEOUT)
        sock.sendall(req.encode())
        data = b""
        while b"\r\n" not in data and len(data) < 4096:
            chunk = sock.recv(1024)
            if not chunk:
                break
            data += chunk
        sock.close()
        first = data.split(b"\r\n", 1)[0].decode("latin-1", errors="replace")
        # 截断噪音
        parts = first.split(" ", 2)
        if len(parts) >= 2 and parts[1].isdigit():
            return f"HTTP {parts[1]}"
        return first[:40] if first else "(empty)"
    except (socket.timeout, TimeoutError):
        return "timeout"
    except Exception as e:
        return f"err:{type(e).__name__}"


def ws_upgrade_ok(status: str) -> bool:
    """101 Switching Protocols = WS upgrade 被接受。"""
    return status == "HTTP 101"


def classify_rest(c: Check, cur: str, cur_json: bool, nxt: str, nxt_json: bool) -> str:
    """REST 单面判定。static_status(PARTIAL/NO_PROBE)优先生效。"""
    if c.static_status:
        return c.static_status
    if not c.current_url:
        return NO_PROBE
    if not (cur.startswith("HTTP 2") and cur_json):
        return DEAD
    if nxt.startswith("HTTP 2") and nxt_json:
        return BEHIND
    return OK


def classify_ws(c: Check, cur: str, nxt: str) -> str:
    """WS 单面判定。不沿用 REST 的 static_status,独立机判。"""
    if not c.ws_current_url:
        return NO_PROBE
    if not ws_upgrade_ok(cur):
        return DEAD
    if ws_upgrade_ok(nxt):
        return BEHIND
    return OK


@dataclass
class CheckResult:
    c: Check
    rest_cur: str
    rest_nxt: str
    rest_status: str
    ws_cur: str
    ws_nxt: str
    ws_status: str


def run_check(c: Check) -> CheckResult:
    rest_cur, rest_cur_json = probe(c.current_url)
    rest_nxt, rest_nxt_json = probe(c.next_url)
    ws_cur = ws_handshake(c.ws_current_url)
    ws_nxt = ws_handshake(c.ws_next_url)
    rest_status = classify_rest(c, rest_cur, rest_cur_json, rest_nxt, rest_nxt_json)
    ws_status = classify_ws(c, ws_cur, ws_nxt)
    rest_cur_disp = rest_cur if rest_cur == "-" else (
        rest_cur if rest_cur_json or not rest_cur.startswith("HTTP 2") else f"{rest_cur} (HTML)"
    )
    rest_nxt_disp = rest_nxt if rest_nxt == "-" else (
        rest_nxt if rest_nxt_json or not rest_nxt.startswith("HTTP 2") else f"{rest_nxt} (HTML)"
    )
    return CheckResult(c, rest_cur_disp, rest_nxt_disp, rest_status, ws_cur, ws_nxt, ws_status)


STATUS_LABEL = {
    OK:       "最新",
    BEHIND:   "落后",
    DEAD:     "失效",
    PARTIAL:  "部分",
    NO_PROBE: "未测",
}


def _next_ver_token(url: str) -> str:
    """从探测 URL 里抽出 'v6' / 'v2' 等版本字面值。"""
    if not url:
        return ""
    import re
    m = re.search(r"/(api/)?(pro/)?(exchange/)?(ws/)?v?(\d+)", url)
    return ("v" + m.group(5)) if m else ""


def visual_width(s: str) -> int:
    """终端显示宽度估算。"""
    import unicodedata
    w = 0
    for ch in s:
        cp = ord(ch)
        if 0xFE00 <= cp <= 0xFE0F or cp == 0x200D:
            continue
        cat = unicodedata.category(ch)
        if cat.startswith("M"):
            continue
        if unicodedata.east_asian_width(ch) in ("F", "W"):
            w += 2
        elif cp >= 0x2300 and cat.startswith("S"):
            w += 2
        else:
            w += 1
    return w


def pad(s: str, target: int) -> str:
    return s + " " * max(0, target - visual_width(s))


def render_box_table(headers: list[str], body: list[list[str]]) -> str:
    """通用框线表格。"""
    if not body:
        return "(无数据)"
    widths = [
        max(visual_width(h), max((visual_width(row[i]) for row in body), default=0))
        for i, h in enumerate(headers)
    ]

    def hborder(left: str, mid: str, right: str) -> str:
        return left + mid.join("─" * (widths[i] + 2) for i in range(len(widths))) + right

    def datarow(cells: list[str]) -> str:
        return "│ " + " │ ".join(pad(cells[i], widths[i]) for i in range(len(cells))) + " │"

    out = [hborder("┌", "┬", "┐"),
           datarow(headers),
           hborder("├", "┼", "┤")]
    for row in body:
        out.append(datarow(row))
    out.append(hborder("└", "┴", "┘"))
    return "\n".join(out)


def short_action(plane: str, c: Check, status: str, next_url: str) -> str:
    """plane: 'rest' / 'ws'。"""
    if status == OK:
        return "—"
    if status == BEHIND:
        nv = _next_ver_token(next_url)
        return f"升级到 {nv}" if nv else "升级到下一版本"
    if status == DEAD:
        return "立即修"
    if status == PARTIAL:
        return "见详情"
    if status == NO_PROBE:
        return "人工核对" if plane == "rest" else "—"
    return ""


def render_rest_table(results) -> str:
    headers = ["结果", "Exchange ID", "ccapi REST 路径", "当前路径", "+1 版本", "操作"]
    body = []
    for r in results:
        body.append([
            STATUS_LABEL[r.rest_status],
            r.c.exchange,
            r.c.ccapi_ver,
            r.rest_cur,
            r.rest_nxt,
            short_action("rest", r.c, r.rest_status, r.c.next_url),
        ])
    return render_box_table(headers, body)


def render_ws_table(results) -> str:
    """只列出有 ws_current_url 的项;无 WS 探测的 17 项一笔带过。"""
    rows_with_ws = [r for r in results if r.c.ws_current_url]
    rows_without = [r for r in results if not r.c.ws_current_url]
    headers = ["结果", "Exchange ID", "ccapi WS 路径", "当前 WS", "+1 版本 WS", "操作"]
    body = []
    for r in rows_with_ws:
        ws_path = urlparse(r.c.ws_current_url).hostname or ""
        path_part = urlparse(r.c.ws_current_url).path
        if path_part:
            ws_path += path_part
        body.append([
            STATUS_LABEL[r.ws_status],
            r.c.exchange,
            ws_path,
            r.ws_cur,
            r.ws_nxt,
            short_action("ws", r.c, r.ws_status, r.c.ws_next_url),
        ])
    out = [render_box_table(headers, body)]
    if rows_without:
        skipped = ", ".join(r.c.exchange for r in rows_without)
        out.append("")
        out.append(f"未探 WS 的 {len(rows_without)} 项(WS URL 无 /vN 体系或动态 URL,自动机检不适用):")
        out.append(f"  {skipped}")
    return "\n".join(out)


def render_details(results) -> str:
    out = ["", "详情(REST + WS 两面综合;只列需要关注的项):", "─" * 60]
    any_ = False
    for r in results:
        rest, ws = r.rest_status, r.ws_status
        if rest == OK and ws in (OK, NO_PROBE):
            continue
        any_ = True
        out.append("")
        out.append(f"  {r.c.exchange}")
        # REST 面
        if rest == BEHIND:
            nv = _next_ver_token(r.c.next_url) or "下一版本"
            out.append(f"    REST  落后→{nv}  | ccapi 在用 {r.c.ccapi_ver},实测 {nv} 返回真实 JSON")
            out.append(f"          操作:把 service 的 REST 路径升级到 {nv}")
        elif rest == DEAD:
            out.append(f"    REST  失效   | ccapi 路径 {r.rest_cur},立即修")
        elif rest == PARTIAL:
            out.append(f"    REST  部分   | {r.c.note}")
        elif rest == NO_PROBE:
            out.append(f"    REST  未测   | {r.c.note or '版本号非 /vN 形式,需人工核对'}")
        # WS 面
        if ws == BEHIND:
            nv = _next_ver_token(r.c.ws_next_url) or "下一版本"
            out.append(f"    WS    落后→{nv}  | ccapi 在用 {r.c.ws_current_url},实测 {nv} URL 接受 WS upgrade")
            out.append(f"          操作:把 service 的 WS 路径升级到 {nv}")
        elif ws == DEAD:
            out.append(f"    WS    失效   | ccapi WS {r.c.ws_current_url},握手返回 {r.ws_cur}")
    if not any_:
        out.append("(REST 与 WS 全部对齐,无需后续操作)")
    return "\n".join(out)


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.parse_args()

    print(f"探测中 (REST + WS 双面,并发,超时 {TIMEOUT}s) ...", file=sys.stderr)
    results: list[CheckResult] = []
    with ThreadPoolExecutor(max_workers=16) as ex:
        futs = {ex.submit(run_check, c): c for c in CHECKS}
        for fut in as_completed(futs):
            results.append(fut.result())
    order = {c.exchange: i for i, c in enumerate(CHECKS)}
    results.sort(key=lambda r: order[r.c.exchange])

    rest_counts: dict[str, int] = {}
    ws_counts: dict[str, int] = {}
    for r in results:
        rest_counts[r.rest_status] = rest_counts.get(r.rest_status, 0) + 1
        ws_counts[r.ws_status] = ws_counts.get(r.ws_status, 0) + 1

    print()
    print("=" * 60)
    print("REST 探测(28 项)")
    print("=" * 60)
    print(render_rest_table(results))
    rest_parts = [f"{ICON[s]} {STATUS_LABEL[s]}={rest_counts[s]}" for s in (OK, BEHIND, DEAD, PARTIAL, NO_PROBE) if s in rest_counts]
    print(f"REST 汇总:" + "  ".join(rest_parts))

    print()
    print("=" * 60)
    print("WebSocket 探测(只列有 /vN 版本号可比对的)")
    print("=" * 60)
    print(render_ws_table(results))
    # WS 汇总只统计有 ws_current_url 的项
    ws_with: dict[str, int] = {}
    for r in results:
        if r.c.ws_current_url:
            ws_with[r.ws_status] = ws_with.get(r.ws_status, 0) + 1
    ws_parts = [f"{ICON[s]} {STATUS_LABEL[s]}={ws_with[s]}" for s in (OK, BEHIND, DEAD, PARTIAL, NO_PROBE) if s in ws_with]
    print(f"WS 汇总(仅有 WS 探测的项):" + "  ".join(ws_parts))

    print(render_details(results))
    return 0


if __name__ == "__main__":
    sys.exit(main())
