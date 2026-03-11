#!/usr/bin/env python3
"""Simple web server for exposing ACKMUD project and game reference webpages."""

from __future__ import annotations

import os
from html import escape
from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path
from urllib.parse import parse_qs, unquote, urlparse

HOST = "0.0.0.0"
PORT = int(os.environ.get("ACK_WEB_PORT", "80"))
WEB_DIR = Path(__file__).resolve().parent
ROOT_DIR = WEB_DIR.parent
WHO_HTML_FILE = WEB_DIR / "soewholist.html"
WHO_COUNT_FILE = WEB_DIR / "whocount.html"
HELP_DIR = ROOT_DIR / "help"
SHELP_DIR = ROOT_DIR / "shelp"
TEMPLATE_DIR = WEB_DIR / "templates"
WORLD_TARGETS = [
    {"id": "acktng", "name": "ACK!TNG", "host": "ackmud.com", "port": 8890},
    {"id": "ack431", "name": "ACK! 4.3.1", "host": "ackmud.com", "port": 8891},
    {"id": "ack42", "name": "ACK! 4.2", "host": "ackmud.com", "port": 8892},
]


class WhoRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self) -> None:  # noqa: N802 (BaseHTTPRequestHandler interface)
        parsed_url = urlparse(self.path)
        route = unquote(parsed_url.path)
        query = parse_qs(parsed_url.query)
        help_query = query.get("q", [""])[0].strip()

        if route in ("/home", "/home/"):
            self._redirect_to("/")
            return

        if route in ("/",):
            self._send_html(_build_home_page(), title="ACKMUD Historical Archive")
            return

        if route in ("/players", "/players/", "/who", "/who/"):
            self._send_html(self._build_players_page(), title="ACKMUD Player List")
            return

        if route in ("/mud", "/mud/"):
            self._send_html(_build_mud_client_page(), title="ACKMUD Web Client")
            return

        if route in ("/help", "/help/"):
            self._send_html(_build_topic_index_page("Help Topics", "helps", HELP_DIR, help_query), title="Help Topics")
            return

        if route in ("/shelp", "/shelp/"):
            self._send_html(
                _build_topic_index_page("Spell Help Topics", "shelps", SHELP_DIR, help_query), title="Spell Help Topics"
            )
            return

        if route in ("/helps", "/helps/"):
            self._redirect_to("/help/")
            return

        if route in ("/shelps", "/shelps/"):
            self._redirect_to("/shelp/")
            return

        if route.startswith("/helps/"):
            topic = route[len("/helps/") :]
            self._send_topic_page("Help", HELP_DIR, topic, "helps")
            return

        if route.startswith("/shelps/"):
            topic = route[len("/shelps/") :]
            self._send_topic_page("Spell Help", SHELP_DIR, topic, "shelps")
            return

        self.send_error(404, "Not Found")

    def do_POST(self) -> None:  # noqa: N802 (BaseHTTPRequestHandler interface)
        self.send_error(404, "Not Found")

    def _redirect_to(self, location: str) -> None:
        self.send_response(302)
        self.send_header("Location", location)
        self.end_headers()

    def _send_topic_page(self, page_name: str, base_dir: Path, topic: str, base_route: str) -> None:
        topic_path = _safe_topic_path(base_dir, topic)
        if topic_path is None:
            self.send_error(404, "Not Found")
            return

        body = (
            f"<h1>{escape(page_name)}: {escape(topic_path.name)}</h1>"
            f"<p><a href='/{escape(base_route)}/'>Back to {escape(page_name)} index</a></p>"
            f"<pre>{escape(topic_path.read_text(encoding='utf-8', errors='replace'))}</pre>"
        )
        self._send_html(body, title=f"{page_name}: {topic_path.name}")

    def _send_html(self, body: str, title: str) -> None:
        page = _build_full_page(title=title, body=body)
        body_bytes = page.encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body_bytes)))
        self.end_headers()
        self.wfile.write(body_bytes)

    def log_message(self, fmt: str, *args: object) -> None:
        return

    def _build_players_page(self) -> str:
        who_html = _read_file_if_present(WHO_HTML_FILE)
        who_count = _read_file_if_present(WHO_COUNT_FILE)

        content = ["<h1>ACKMUD Player Activity</h1>", "<p class='muted'>Live snapshot from in-game WHO output.</p>"]
        if who_count is not None:
            content.append(who_count)
        else:
            content.append("<p>Players online: 0</p>")

        if who_html is not None:
            content.append(who_html)
        else:
            content.append("<h2>Players Online</h2>\n<ul>\n</ul>")

        return "\n".join(content)


def _read_file_if_present(path: Path) -> str | None:
    if not path.exists() or not path.is_file():
        return None
    return path.read_text(encoding="utf-8", errors="replace")


def _safe_topic_path(base_dir: Path, topic: str) -> Path | None:
    cleaned_topic = topic.strip().strip("/")
    if not cleaned_topic:
        return None

    candidate = (base_dir / cleaned_topic).resolve()
    if not candidate.is_file():
        return None

    if base_dir.resolve() not in candidate.parents:
        return None

    return candidate


def _build_topic_index_page(title: str, route_base: str, base_dir: Path, query: str = "") -> str:
    if not base_dir.exists() or not base_dir.is_dir():
        return f"<h1>{escape(title)}</h1><p>No topics available.</p>"

    normalized_query = query.strip().lower()
    links = [
        f"<li><a href='/{escape(route_base)}/{escape(path.name)}'>{escape(path.name)}</a></li>"
        for path in sorted(base_dir.iterdir(), key=lambda p: p.name)
        if path.is_file() and (not normalized_query or normalized_query in path.name.lower())
    ]

    if not links:
        if normalized_query:
            return f"<h1>{escape(title)}</h1><p>No topics match <strong>{escape(query)}</strong>.</p>"
        return f"<h1>{escape(title)}</h1><p>No topics available.</p>"

    query_blurb = ""
    if normalized_query:
        query_blurb = f"<p>Filtered by <strong>{escape(query)}</strong>.</p>"

    return f"<h1>{escape(title)}</h1>{query_blurb}<ul>{''.join(links)}</ul>"


def _build_home_page() -> str:
    return _load_template("home.html")


def _build_mud_client_page() -> str:
    world_options = "".join(
        (
            f"<option value='{world['id']}' data-host='{world['host']}' data-port='{world['port']}' data-ws='ws://{world['host']}:{world['port']}/'>{world['name']} ({world['host']}:{world['port']})</option>"
        )
        for world in WORLD_TARGETS
    )
    return _load_template("mud_client.html").replace("__WORLD_OPTIONS__", world_options)


def _load_template(name: str) -> str:
    return (TEMPLATE_DIR / name).read_text(encoding="utf-8", errors="replace")


def _build_full_page(title: str, body: str) -> str:
    template = _load_template("base.html")
    return template.replace("__TITLE__", escape(title)).replace("__BODY__", body)


def main() -> None:
    HTTPServer((HOST, PORT), WhoRequestHandler).serve_forever()


if __name__ == "__main__":
    main()
