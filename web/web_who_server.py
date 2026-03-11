#!/usr/bin/env python3
"""Simple web server for exposing ACKMUD project and game reference webpages."""

from __future__ import annotations

import json
import os
import socket
import threading
import uuid
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
WORLD_TARGETS = [
    {"id": "acktng", "name": "ACK!TNG", "host": "ackmud.com", "port": 8890},
    {"id": "ack431", "name": "ACK! 4.3.1", "host": "ackmud.com", "port": 8891},
    {"id": "ack42", "name": "ACK! 4.2", "host": "ackmud.com", "port": 8892},
]


class MudSession:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port
        self.socket = socket.create_connection((host, port), timeout=10)
        self.socket.settimeout(0.5)
        self._chunks: list[str] = []
        self._chunks_lock = threading.Lock()
        self.connected = True
        self.error = ""
        self._reader = threading.Thread(target=self._reader_loop, daemon=True)
        self._reader.start()

    def _reader_loop(self) -> None:
        while self.connected:
            try:
                data = self.socket.recv(4096)
            except socket.timeout:
                continue
            except OSError as exc:
                self.error = f"Connection error: {exc}"
                break

            if not data:
                break

            decoded = data.decode("utf-8", errors="replace")
            with self._chunks_lock:
                self._chunks.append(decoded)

        self.connected = False
        self.close()

    def drain_output(self) -> str:
        with self._chunks_lock:
            output = "".join(self._chunks)
            self._chunks.clear()
        return output

    def send_line(self, line: str) -> None:
        if not self.connected:
            return
        payload = f"{line}\n".encode("utf-8")
        self.socket.sendall(payload)

    def close(self) -> None:
        if not self.connected:
            try:
                self.socket.close()
            except OSError:
                return
            return

        self.connected = False
        try:
            self.socket.shutdown(socket.SHUT_RDWR)
        except OSError:
            pass
        try:
            self.socket.close()
        except OSError:
            pass


MUD_SESSIONS: dict[str, MudSession] = {}
MUD_SESSIONS_LOCK = threading.Lock()


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

        if route in ("/mud/poll",):
            self._handle_mud_poll(query)
            return

        self.send_error(404, "Not Found")

    def do_POST(self) -> None:  # noqa: N802 (BaseHTTPRequestHandler interface)
        parsed_url = urlparse(self.path)
        route = unquote(parsed_url.path)
        payload = self._parse_json_body()

        if route == "/mud/connect":
            self._handle_mud_connect(payload)
            return

        if route == "/mud/send":
            self._handle_mud_send(payload)
            return

        if route == "/mud/disconnect":
            self._handle_mud_disconnect(payload)
            return

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
        nav = (
            "<nav>"
            "<a href='/'>Home</a>"
            "<a href='/who/'>Who</a>"
            "<a href='/help/'>Help</a>"
            "<a href='/shelp/'>SHelp</a>"
            "<a href='/mud/'>MUD Client</a>"
            "<a href='https://discord.gg/T24UQV8h' target='_blank' rel='noopener noreferrer'>Discord</a>"
            "<a href='https://github.com/ackmudhistoricalarchive/acktng/tree/main/area' target='_blank' rel='noopener noreferrer'>Github</a>"
            "</nav>"
        )
        help_forms = (
            "<section class='help-forms'>"
            "<form method='get' action='/help/'>"
            "<label for='help-q'>Help:</label>"
            "<input id='help-q' name='q' placeholder='topic'>"
            "<button type='submit'>Open help</button>"
            "</form>"
            "<form method='get' action='/shelp/'>"
            "<label for='shelp-q'>SHelp:</label>"
            "<input id='shelp-q' name='q' placeholder='spell/skill'>"
            "<button type='submit'>Open shelp</button>"
            "</form>"
            "</section>"
        )
        page = (
            "<!doctype html><html><head>"
            f"<title>{escape(title)}</title>"
            "<meta name='viewport' content='width=device-width, initial-scale=1'>"
            "<style>"
            ":root{color-scheme:dark;--bg:#05080f;--bg-soft:#0f1729;--card:#121c31;--line:#253457;--text:#e5ecff;--muted:#9eb0df;--accent:#79b7ff;--accent-soft:#163866;}"
            "*{box-sizing:border-box;}"
            "body{font-family:Inter,Segoe UI,Roboto,sans-serif;background:radial-gradient(circle at top,#18284a,#05080f 55%);color:var(--text);max-width:1080px;margin:0 auto;padding:1.4rem 1rem 2rem;line-height:1.6;}"
            "nav{display:flex;flex-wrap:wrap;gap:.6rem;margin-bottom:1rem;}"
            "nav a{padding:.5rem .85rem;background:rgba(121,183,255,.12);border:1px solid var(--line);border-radius:999px;text-decoration:none;color:var(--text);font-weight:600;letter-spacing:.02em;}"
            "nav a:hover{background:rgba(121,183,255,.25);border-color:#4d72b8;}"
            ".help-forms{display:flex;flex-wrap:wrap;gap:.8rem;margin:0 0 1.2rem;padding:1rem;background:rgba(18,28,49,.72);border:1px solid var(--line);border-radius:14px;backdrop-filter:blur(4px);}"
            ".help-forms form{display:flex;gap:.55rem;align-items:center;flex-wrap:wrap;}"
            "label{color:var(--muted);font-weight:600;}"
            "input{background:var(--bg-soft);border:1px solid var(--line);color:var(--text);padding:.45rem .65rem;border-radius:10px;min-width:190px;}"
            "button{background:linear-gradient(135deg,#5ea8ff,#72cbff);color:#061126;border:none;padding:.5rem .85rem;border-radius:10px;font-weight:700;cursor:pointer;}"
            "button:hover{filter:brightness(1.05);}"
            "main{background:rgba(18,28,49,.78);border:1px solid var(--line);padding:1.1rem 1.15rem;border-radius:16px;box-shadow:0 12px 30px rgba(0,0,0,.32);}"
            "h1,h2,h3{line-height:1.2;margin-top:0;color:#f4f7ff;}"
            "a{color:var(--accent);}"
            "p,li{color:var(--text);}"
            ".muted{color:var(--muted);}"
            "pre{white-space:pre-wrap;background:#0a1222;padding:0.9rem;border-radius:12px;border:1px solid var(--line);overflow:auto;}"
            ".mud-output{height:52vh;overflow:auto;font-family:ui-monospace,SFMono-Regular,Menlo,monospace;}"
            ".mud-controls{display:flex;flex-wrap:wrap;gap:.55rem;margin:.75rem 0;}"
            ".mud-controls select{background:var(--bg-soft);border:1px solid var(--line);color:var(--text);padding:.45rem .65rem;border-radius:10px;}"
            "ul{padding-left:1.2rem;}"
            ".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(240px,1fr));gap:.8rem;margin:.8rem 0 1rem;}"
            ".card{background:rgba(8,15,29,.72);border:1px solid var(--line);padding:.8rem .9rem;border-radius:12px;}"
            "</style>"
            "</head><body>"
            f"{nav}"
            f"{help_forms}"
            f"<main>{body}</main>"
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

    def _parse_json_body(self) -> dict[str, object]:
        content_length = int(self.headers.get("Content-Length", "0"))
        raw_payload = self.rfile.read(content_length) if content_length > 0 else b"{}"
        try:
            parsed = json.loads(raw_payload.decode("utf-8"))
        except json.JSONDecodeError:
            return {}
        return parsed if isinstance(parsed, dict) else {}

    def _send_json(self, payload: dict[str, object], status: int = 200) -> None:
        response = json.dumps(payload).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)

    def _handle_mud_connect(self, payload: dict[str, object]) -> None:
        world_id = str(payload.get("world", "")).strip()
        world = next((item for item in WORLD_TARGETS if item["id"] == world_id), None)
        if world is None:
            self._send_json({"ok": False, "error": "Unknown world."}, status=400)
            return

        try:
            session = MudSession(host=world["host"], port=world["port"])
        except OSError as exc:
            self._send_json({"ok": False, "error": f"Unable to connect: {exc}"}, status=502)
            return

        session_id = uuid.uuid4().hex
        with MUD_SESSIONS_LOCK:
            MUD_SESSIONS[session_id] = session

        self._send_json({"ok": True, "session": session_id, "world": world["name"]})

    def _handle_mud_send(self, payload: dict[str, object]) -> None:
        session_id = str(payload.get("session", ""))
        command = str(payload.get("command", ""))
        with MUD_SESSIONS_LOCK:
            session = MUD_SESSIONS.get(session_id)

        if session is None:
            self._send_json({"ok": False, "error": "Session not found."}, status=404)
            return

        session.send_line(command)
        self._send_json({"ok": True})

    def _handle_mud_poll(self, query: dict[str, list[str]]) -> None:
        session_id = query.get("session", [""])[0]
        with MUD_SESSIONS_LOCK:
            session = MUD_SESSIONS.get(session_id)

        if session is None:
            self._send_json({"ok": False, "error": "Session not found."}, status=404)
            return

        output = session.drain_output()
        payload = {"ok": True, "output": output, "connected": session.connected, "error": session.error}

        if not session.connected:
            with MUD_SESSIONS_LOCK:
                MUD_SESSIONS.pop(session_id, None)

        self._send_json(payload)

    def _handle_mud_disconnect(self, payload: dict[str, object]) -> None:
        session_id = str(payload.get("session", ""))
        with MUD_SESSIONS_LOCK:
            session = MUD_SESSIONS.pop(session_id, None)

        if session is not None:
            session.close()

        self._send_json({"ok": True})


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
    return """
<h1>ACKMUD Historical Archive Project</h1>
<p>
  The ACKMUD Historical Archive is a long-horizon preservation and interpretation effort for one of the enduring
  text-world traditions: the ACK code lineage and the living worlds that grew from it. This project is not just a file dump;
  it is a curated record of worldbuilding decisions, game-system evolution, social history, and technical craft spanning
  years of iterative development.
</p>


<h2>Active ACK! Worlds</h2>
<ul>
  <li><strong>ACK!TNG (The Next Generation):</strong> Running at <code>ackmud.com 8890</code>.</li>
  <li><strong>ACK! 4.3.1:</strong> Running at <code>ackmud.com 8891</code>.</li>
  <li><strong>ACK! 4.2:</strong> Running at <code>ackmud.com 8892</code>.</li>
</ul>

<div class='grid'>
  <section class='card'>
    <h2>Mission</h2>
    <p>
      Preserve game assets, system logic, and reference text in a format that remains readable and useful to future builders,
      maintainers, and players. The archive balances authenticity with accessibility: original content is retained while
      navigational surfaces (help/shelp indexes and web pages) make discovery practical.
    </p>
  </section>
  <section class='card'>
    <h2>Scope</h2>
    <p>
      The collection spans areas, NPC definitions, help libraries, spell references, logs, and supporting data files that
      describe both gameplay and operational culture. Together these materials document how classes, encounters, and
      progression loops changed over time, including both polished systems and historically significant rough edges.
    </p>
  </section>
  <section class='card'>
    <h2>Research Value</h2>
    <p>
      Beyond gameplay nostalgia, the archive is useful for software archaeology. It captures architecture decisions in
      long-lived C/C++ MUD codebases, balancing performance constraints, maintainability, and community-driven feature growth.
      For designers, it offers a practical catalog of pacing, reward, and social-system patterns proven in persistent worlds.
    </p>
  </section>
</div>

<h2>Historical Context</h2>
<p>
  ACK-based MUDs embody a period where online worlds were built collaboratively and operated continuously, often by small teams
  with deep domain knowledge. Every command, help topic, and area file becomes part of a running chronicle: player behavior
  informs balance updates; builder style informs narrative texture; operational incidents inform infrastructure hardening.
  The archive preserves these strata as evidence of how live service design emerged from grassroots craftsmanship.
</p>

<h2>What This Web Interface Provides</h2>
<ul>
  <li><strong>Home:</strong> A narrative overview of the archive's purpose and historical significance.</li>
  <li><strong>Who:</strong> A dedicated, live player activity view separated from archival content for cleaner discovery paths.</li>
  <li><strong>Help / SHelp:</strong> Searchable indexes into game documentation and spell/skill references for historians,
      implementers, and returning players.</li>
  <li><strong>Community / Source:</strong> Quick links to the Discord server and the canonical GitHub area-file tree.</li>
</ul>
<h2>Preservation Principles</h2>
<ul>
  <li><strong>Fidelity first:</strong> Source materials are retained as primary artifacts.</li>
  <li><strong>Interpretation second:</strong> Supplemental explanations are additive and clearly separated from originals.</li>
  <li><strong>Traceability:</strong> Structural changes are made so future maintainers can map web output back to canonical files.</li>
  <li><strong>Longevity:</strong> Lightweight tooling and minimal dependencies reduce maintenance risk over time.</li>
</ul>

<p class='muted'>
  This archive is intended to remain useful decades from now: to support restoration, scholarly study, emulator efforts,
  and renewed play. It treats ACKMUD not only as software, but as a cultural artifact shaped by its builders and community.
</p>

<p>
  Want to connect immediately from your browser? Use the <a href="/mud/">MUD Client</a> page and select a world.
</p>
"""


def _build_mud_client_page() -> str:
    worlds_json = json.dumps(WORLD_TARGETS)
    return f"""
<h1>ACKMUD Web Client</h1>
<p class='muted'>Select a world and connect directly from your browser. Add future worlds in <code>WORLD_TARGETS</code>.</p>
<div class='mud-controls'>
  <label for='world-select'>World</label>
  <select id='world-select'></select>
  <button id='connect-btn' type='button'>Connect</button>
  <button id='disconnect-btn' type='button'>Disconnect</button>
</div>
<div class='mud-controls'>
  <input id='mud-command' placeholder='Type command and press Enter' style='flex:1;min-width:280px;'>
  <button id='send-btn' type='button'>Send</button>
</div>
<pre id='mud-output' class='mud-output'>Ready.</pre>
<script>
(() => {{
  const worlds = {worlds_json};
  const worldSelect = document.getElementById('world-select');
  const connectBtn = document.getElementById('connect-btn');
  const disconnectBtn = document.getElementById('disconnect-btn');
  const sendBtn = document.getElementById('send-btn');
  const commandInput = document.getElementById('mud-command');
  const output = document.getElementById('mud-output');
  let session = null;
  let pollTimer = null;

  worlds.forEach((world) => {{
    const option = document.createElement('option');
    option.value = world.id;
    option.textContent = `${{world.name}} (${{world.host}}:${{world.port}})`;
    worldSelect.appendChild(option);
  }});

  const appendOutput = (text) => {{
    output.textContent += text;
    output.scrollTop = output.scrollHeight;
  }};

  const stopPolling = () => {{
    if (pollTimer) {{
      clearInterval(pollTimer);
      pollTimer = null;
    }}
  }};

  const startPolling = () => {{
    stopPolling();
    pollTimer = setInterval(async () => {{
      if (!session) return;
      const response = await fetch(`/mud/poll?session=${{encodeURIComponent(session)}}`);
      const data = await response.json();
      if (!data.ok) {{
        appendOutput(`\n[Error] ${{data.error}}\n`);
        stopPolling();
        session = null;
        return;
      }}
      if (data.output) appendOutput(data.output);
      if (!data.connected) {{
        if (data.error) appendOutput(`\n[Disconnected] ${{data.error}}\n`);
        else appendOutput('\n[Disconnected]\n');
        stopPolling();
        session = null;
      }}
    }}, 350);
  }};

  connectBtn.addEventListener('click', async () => {{
    if (session) {{
      appendOutput('\n[Info] Already connected.\n');
      return;
    }}
    output.textContent = '';
    const response = await fetch('/mud/connect', {{
      method: 'POST',
      headers: {{ 'Content-Type': 'application/json' }},
      body: JSON.stringify({{ world: worldSelect.value }}),
    }});
    const data = await response.json();
    if (!data.ok) {{
      appendOutput(`[Error] ${{data.error}}\n`);
      return;
    }}
    session = data.session;
    appendOutput(`[Connected] ${{data.world}}\n`);
    startPolling();
    commandInput.focus();
  }});

  disconnectBtn.addEventListener('click', async () => {{
    if (!session) return;
    await fetch('/mud/disconnect', {{
      method: 'POST',
      headers: {{ 'Content-Type': 'application/json' }},
      body: JSON.stringify({{ session }}),
    }});
    appendOutput('\n[Disconnected]\n');
    stopPolling();
    session = null;
  }});

  const sendCommand = async () => {{
    const command = commandInput.value;
    if (!session || !command.trim()) return;
    await fetch('/mud/send', {{
      method: 'POST',
      headers: {{ 'Content-Type': 'application/json' }},
      body: JSON.stringify({{ session, command }}),
    }});
    appendOutput(`\n> ${{command}}\n`);
    commandInput.value = '';
  }};

  sendBtn.addEventListener('click', sendCommand);
  commandInput.addEventListener('keydown', (event) => {{
    if (event.key === 'Enter') sendCommand();
  }});
}})();
</script>
"""


def main() -> None:
    HTTPServer((HOST, PORT), WhoRequestHandler).serve_forever()


if __name__ == "__main__":
    main()
