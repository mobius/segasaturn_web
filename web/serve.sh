#!/usr/bin/env bash
# Assembles build-web/dist (wasm module + JS glue + web shell) and starts a
# local static server. Usage: web/serve.sh [port]   (default 8080)
set -euo pipefail
cd "$(dirname "$0")/.."

DIST=build-web/dist
mkdir -p "$DIST"
cp -f build-web/ymir-web.js build-web/ymir-web.wasm "$DIST"/
cp -f web/shell/index.html web/shell/main.js web/shell/ymir-web-audio-worklet.js "$DIST"/

PORT="${1:-8080}"
echo "Serving $DIST at http://localhost:${PORT}"
cd "$DIST"
exec python3 -m http.server "$PORT"
