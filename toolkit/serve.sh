#!/bin/bash
# Serve the official build (MiniDayZ+1.0 by default) on 127.0.0.1:8731 if nothing is serving it yet.
B=${1:-MiniDayZ+1.0}; P=${2:-8731}
curl -s -o /dev/null "http://127.0.0.1:$P/index.html" && exit 0
cd "$(dirname "$0")/../$B" && nohup python3 -m http.server $P --bind 127.0.0.1 > /dev/null 2>&1 &
sleep 1
