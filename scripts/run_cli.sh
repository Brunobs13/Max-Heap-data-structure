#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

if [[ ! -f "build/maxheap_cli" ]]; then
  ./scripts/build.sh
fi

./build/maxheap_cli
