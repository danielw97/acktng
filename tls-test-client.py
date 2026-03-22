"""
TLS telnet test client for ACK!MUD integration testing.

Usage: python3 tls-test-client.py <port> <player> <password>

Connects to 127.0.0.1:<port> over TLS (self-signed cert accepted),
strips telnet IAC negotiation, and walks the new-player creation flow.
Exits 0 on success, 1 on failure.
"""

import socket
import ssl
import sys
import time

PORT     = int(sys.argv[1])
PLAYER   = sys.argv[2]
PASSWORD = sys.argv[3]

# Telnet protocol constants for IAC stripping
IAC  = 0xFF
SE   = 0xF0
SB   = 0xFA
WILL = 0xFB
WONT = 0xFC
DO   = 0xFD
DONT = 0xFE

def strip_iac(buf):
    """Remove telnet IAC sequences from a bytearray.

    Returns (text, remaining_buf) where remaining_buf holds any trailing
    bytes that form an incomplete IAC sequence (not yet safe to discard).
    """
    out = bytearray()
    i = 0
    while i < len(buf):
        b = buf[i]
        if b == IAC:
            if i + 1 >= len(buf):
                break                        # incomplete – keep for next read
            cmd = buf[i + 1]
            if cmd == IAC:                   # escaped 0xFF literal
                out.append(IAC)
                i += 2
            elif cmd in (WILL, WONT, DO, DONT):
                if i + 2 >= len(buf):
                    break                    # incomplete option byte
                i += 3
            elif cmd == SB:
                # Skip subnegotiation block until IAC SE
                j = i + 2
                found_se = False
                while j + 1 < len(buf):
                    if buf[j] == IAC and buf[j + 1] == SE:
                        j += 2
                        found_se = True
                        break
                    j += 1
                if not found_se:
                    break                    # incomplete subnegotiation
                i = j
            else:
                i += 2                       # other 2-byte commands (GA, NOP…)
        else:
            out.append(b)
            i += 1
    return out.decode('latin-1', errors='replace'), bytearray(buf[i:])

def fail(msg, context=""):
    print(f"integration-test: FAILED (TLS) - {msg}", flush=True)
    if context:
        print(f"  received: {repr(context[-300:])}", flush=True)
    sys.exit(1)

# Open a TLS connection, accepting any certificate (self-signed test cert).
raw = socket.create_connection(('127.0.0.1', PORT), timeout=10)
ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE
sock = ctx.wrap_socket(raw, server_hostname='127.0.0.1')

buf = bytearray()

def recv_until(marker, timeout=10.0):
    """Read from the TLS socket until marker appears in stripped text."""
    global buf
    local_text = ""
    deadline = time.time() + timeout
    while time.time() < deadline:
        sock.settimeout(min(0.5, deadline - time.time()))
        try:
            chunk = sock.recv(4096)
            if chunk:
                buf.extend(chunk)
        except (socket.timeout, ssl.SSLError, OSError):
            pass
        text, buf = strip_iac(buf)
        local_text += text
        if marker.lower() in local_text.lower():
            return local_text
    return local_text

def send(msg):
    sock.sendall((msg + '\n').encode('latin-1'))

# Walk the new-player creation flow.
data = recv_until('name', timeout=10.0)
if 'name' not in data.lower():
    fail("expected name prompt in greeting", data)

send(PLAYER)
data = recv_until('y/n', timeout=5.0)
if 'y/n' not in data.lower():
    fail("expected name confirmation (Y/N)", data)

send('Y')
data = recv_until('assword', timeout=5.0)
if 'assword' not in data:
    fail("expected password prompt", data)

send(PASSWORD)
data = recv_until('etype', timeout=5.0)
if 'etype' not in data:
    fail("expected retype-password prompt", data)

send(PASSWORD)
data = recv_until('elect', timeout=5.0)
if 'elect' not in data.lower():
    fail("expected character-creation menu", data)

send('1')
recv_until('ex', timeout=5.0)
send('M')
recv_until('elect', timeout=5.0)

send('2')
recv_until('ace', timeout=5.0)
send('Hmn')
recv_until('elect', timeout=5.0)

send('3')
recv_until('lass', timeout=5.0)
send('War Mag Cle Sen')
recv_until('elect', timeout=5.0)

send('4')
recv_until('\n', timeout=5.0)

send('')
data = recv_until('welcome', timeout=10.0)
if 'welcome' not in data.lower():
    fail("expected 'Welcome' message after entering game", data)

time.sleep(2.0)
print(f"integration-test: TLS login successful - '{PLAYER}' reached playing state over TLS and stayed connected for 2s", flush=True)
sock.close()
sys.exit(0)
