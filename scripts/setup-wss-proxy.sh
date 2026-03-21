#!/usr/bin/env bash
# This script has been superseded by scripts/install-nginx-certbot.sh in the
# web repo (~/web).  Run that instead:
#
#   sudo bash ~/web/scripts/install-nginx-certbot.sh
#
# That script configures nginx for HTTPS, the ACME webroot certbot uses,
# and the WSS proxy for acktng (wss://ackmud.com:9890 → 127.0.0.1:18890).
echo "Superseded. Run: sudo bash ~/web/scripts/install-nginx-certbot.sh" >&2
exit 1
