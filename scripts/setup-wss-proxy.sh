#!/bin/bash
# setup-wss-proxy.sh — Configure nginx TLS proxy and firewall for WSS support.
#
# Run this on the ackmud.com server host as root (or with sudo).
# Safe to re-run: all steps are idempotent.
#
# What this script does:
#   1. Writes /etc/nginx/conf.d/ackmud-wss.conf  (wss://:9890 → ws://127.0.0.1:18890)
#   2. Tests and reloads nginx
#   3. Adds iptables rule to block port 18890 from external traffic
#   4. Persists iptables rules (Debian/Ubuntu: iptables-save)
#   5. Writes certbot post-renewal hook to reload nginx after cert renewal

set -euo pipefail

# ── Configuration ────────────────────────────────────────────────────────────

DOMAIN="ackmud.com"
PUBLIC_WS_PORT="9890"
LOOPBACK_WS_PORT="18890"
CERT_DIR="/etc/letsencrypt/live/${DOMAIN}"
NGINX_CONF="/etc/nginx/conf.d/ackmud-wss.conf"
RENEWAL_HOOK="/etc/letsencrypt/renewal-hooks/deploy/reload-ackmud-wss.sh"

# ── Helpers ───────────────────────────────────────────────────────────────────

info()  { echo "[INFO]  $*"; }
ok()    { echo "[OK]    $*"; }
warn()  { echo "[WARN]  $*"; }
die()   { echo "[ERROR] $*" >&2; exit 1; }

require_root() {
    [ "$(id -u)" -eq 0 ] || die "This script must be run as root (or with sudo)."
}

require_cmd() {
    command -v "$1" >/dev/null 2>&1 || die "'$1' is not installed. Please install it first."
}

# ── Preflight checks ─────────────────────────────────────────────────────────

require_root
require_cmd nginx
require_cmd iptables

info "Checking TLS certificate for ${DOMAIN}..."
if [ ! -f "${CERT_DIR}/fullchain.pem" ] || [ ! -f "${CERT_DIR}/privkey.pem" ]; then
    die "TLS certificate not found at ${CERT_DIR}/. Run 'certbot certonly --nginx -d ${DOMAIN}' first."
fi
ok "Certificate found at ${CERT_DIR}/"

# ── Step 1: nginx proxy configuration ────────────────────────────────────────

info "Writing nginx config: ${NGINX_CONF}"
cat > "${NGINX_CONF}" <<NGINX
# ACK!TNG WSS proxy — managed by setup-wss-proxy.sh
# Accepts wss://${DOMAIN}:${PUBLIC_WS_PORT} (TLS), forwards to ws://127.0.0.1:${LOOPBACK_WS_PORT}

server {
    listen      ${PUBLIC_WS_PORT} ssl;
    server_name ${DOMAIN};

    ssl_certificate     ${CERT_DIR}/fullchain.pem;
    ssl_certificate_key ${CERT_DIR}/privkey.pem;
    ssl_protocols       TLSv1.2 TLSv1.3;
    ssl_ciphers         HIGH:!aNULL:!MD5;

    location / {
        proxy_pass          http://127.0.0.1:${LOOPBACK_WS_PORT};
        proxy_http_version  1.1;
        proxy_set_header    Upgrade    \$http_upgrade;
        proxy_set_header    Connection "upgrade";
        proxy_set_header    Host       \$host;
        # MUD sessions are long-lived; prevent nginx dropping idle connections.
        proxy_read_timeout  3600s;
        proxy_send_timeout  3600s;
    }
}
NGINX
ok "nginx config written."

info "Testing nginx configuration..."
nginx -t || die "nginx config test failed. Fix the errors above and re-run."
ok "nginx config OK."

info "Reloading nginx..."
systemctl reload nginx
ok "nginx reloaded."

# ── Step 2: iptables — block inner port from the internet ────────────────────

info "Checking iptables rule for port ${LOOPBACK_WS_PORT}..."

# Check if the rule already exists to stay idempotent.
if iptables -C INPUT -p tcp --dport "${LOOPBACK_WS_PORT}" ! -s 127.0.0.1 -j DROP 2>/dev/null; then
    ok "iptables rule already present — skipping."
else
    iptables -A INPUT -p tcp --dport "${LOOPBACK_WS_PORT}" ! -s 127.0.0.1 -j DROP
    ok "iptables rule added: DROP tcp:${LOOPBACK_WS_PORT} from non-loopback."
fi

info "Persisting iptables rules..."
if command -v iptables-save >/dev/null 2>&1; then
    RULES_FILE="/etc/iptables/rules.v4"
    mkdir -p "$(dirname "${RULES_FILE}")"
    iptables-save > "${RULES_FILE}"
    ok "iptables rules saved to ${RULES_FILE}."
else
    warn "iptables-save not found. Rules will not survive a reboot. Install 'iptables-persistent' to fix this."
fi

# ── Step 3: certbot post-renewal hook ────────────────────────────────────────

info "Writing certbot renewal hook: ${RENEWAL_HOOK}..."
mkdir -p "$(dirname "${RENEWAL_HOOK}")"
cat > "${RENEWAL_HOOK}" <<'HOOK'
#!/bin/bash
# Reload nginx after Let's Encrypt renews the certificate so the new cert
# is served immediately without a full service restart.
systemctl reload nginx
HOOK
chmod +x "${RENEWAL_HOOK}"
ok "Renewal hook written and made executable."

info "Running certbot renewal dry-run to confirm hook fires..."
if command -v certbot >/dev/null 2>&1; then
    certbot renew --dry-run 2>&1 | grep -E "Congratulations|No renewals|hook|Simulating|error" || true
    ok "Dry-run complete. Check output above for any errors."
else
    warn "'certbot' not found in PATH. Skipping dry-run. Verify the renewal hook manually."
fi

# ── Summary ───────────────────────────────────────────────────────────────────

echo ""
echo "════════════════════════════════════════════════════════════════"
echo " WSS proxy setup complete."
echo ""
echo " Public (WSS):   wss://${DOMAIN}:${PUBLIC_WS_PORT}/"
echo " Internal (WS):  ws://127.0.0.1:${LOOPBACK_WS_PORT}/"
echo " Firewall:       port ${LOOPBACK_WS_PORT} blocked from external traffic"
echo " Renewal hook:   ${RENEWAL_HOOK}"
echo ""
echo " Verify with:"
echo "   openssl s_client -connect ${DOMAIN}:${PUBLIC_WS_PORT} -quiet 2>/dev/null <<'EOF'"
echo "   GET / HTTP/1.1"
echo "   Host: ${DOMAIN}"
echo "   Upgrade: websocket"
echo "   Connection: Upgrade"
echo "   Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ=="
echo "   Sec-WebSocket-Version: 13"
echo ""
echo "   EOF"
echo " (Expect: HTTP/1.1 101 Switching Protocols)"
echo ""
echo " Ensure the MUD is running with:"
echo "   cd area && ../src/ack 4000 --ws-loopback ${LOOPBACK_WS_PORT}"
echo "════════════════════════════════════════════════════════════════"
