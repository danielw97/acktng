#!/usr/bin/env python3
"""Simple web server for exposing WHO, help, and shelp webpages."""

from __future__ import annotations

from html import escape
from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path
from urllib.parse import unquote

HOST = "0.0.0.0"
PORT = 80
WHO_HTML_FILE = Path("/home/ftp/pub/webpage/soewholist.html")
WHO_COUNT_FILE = Path("/home/ftp/pub/webpage/whocount.html")
ROOT_DIR = Path(__file__).resolve().parent.parent
HELP_DIR = ROOT_DIR / "help"
SHELP_DIR = ROOT_DIR / "shelp"


class WhoRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self) -> None:  # noqa: N802 (BaseHTTPRequestHandler interface)
        route = unquote(self.path.split("?", 1)[0])

        if route in ("/", "/players", "/players/"):
            self._send_html(self._build_players_page(), title="ACK! Player List")
            return

        if route in ("/helps", "/helps/"):
            self._send_html(_build_topic_index_page("Help Topics", "helps", HELP_DIR), title="Help Topics")
            return

        if route in ("/shelps", "/shelps/"):
            self._send_html(_build_topic_index_page("Spell Help Topics", "shelps", SHELP_DIR), title="Spell Help Topics")
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
        page = (
            "<html><head>"
            f"<title>{escape(title)}</title>"
            "</head><body>"
            f"{body}"
            "</body></html>"
        )
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

        if who_html is None:
            return (
                "<h1>ACK! Player List</h1>"
                "<p>The WHO webpage output has not been generated yet.</p>"
            )

        content = ["<h1>ACK! Player List</h1>"]
        if who_count is not None:
            content.append(who_count)
        content.append(who_html)
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


def _build_topic_index_page(title: str, route_base: str, base_dir: Path) -> str:
    if not base_dir.exists() or not base_dir.is_dir():
        return f"<h1>{escape(title)}</h1><p>No topics available.</p>"

    links = [
        f"<li><a href='/{escape(route_base)}/{escape(path.name)}'>{escape(path.name)}</a></li>"
        for path in sorted(base_dir.iterdir(), key=lambda p: p.name)
        if path.is_file()
    ]

    if not links:
        return f"<h1>{escape(title)}</h1><p>No topics available.</p>"

    return f"<h1>{escape(title)}</h1><ul>{''.join(links)}</ul>"


def main() -> None:
    HTTPServer((HOST, PORT), WhoRequestHandler).serve_forever()


if __name__ == "__main__":
    main()
