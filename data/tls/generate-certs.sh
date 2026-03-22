#!/bin/sh
# Generate a self-signed TLS certificate and private key for ACK!TNG.
# Safe to re-run: skips generation if key.pem already exists.
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
KEY="$SCRIPT_DIR/key.pem"
CERT="$SCRIPT_DIR/cert.pem"

if [ -f "$KEY" ]; then
    echo "TLS key already exists at $KEY — skipping generation."
    exit 0
fi

echo "Generating self-signed TLS certificate..."
openssl req -x509 -newkey rsa:4096 \
    -keyout "$KEY" \
    -out "$CERT" \
    -days 3650 \
    -nodes \
    -subj "/CN=acktng/O=ACK!MUD TNG/C=US" \
    -addext "subjectAltName=DNS:localhost,IP:127.0.0.1" \
    2>/dev/null

chmod 600 "$KEY"
echo "Generated $CERT and $KEY (valid 10 years)."
