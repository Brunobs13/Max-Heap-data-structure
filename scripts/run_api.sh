#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

if [[ ! -f "build/libmaxheap.dylib" && ! -f "build/libmaxheap.so" && ! -f "build/Release/maxheap.dll" ]]; then
  ./scripts/build.sh
fi

if [[ -z "${MAX_HEAP_LIB_PATH:-}" ]]; then
  if [[ -f "build/libmaxheap.dylib" ]]; then
    export MAX_HEAP_LIB_PATH="build/libmaxheap.dylib"
  elif [[ -f "build/libmaxheap.so" ]]; then
    export MAX_HEAP_LIB_PATH="build/libmaxheap.so"
  elif [[ -f "build/Release/maxheap.dll" ]]; then
    export MAX_HEAP_LIB_PATH="build/Release/maxheap.dll"
  fi
fi

uvicorn src.api.app:app --host 0.0.0.0 --port 8080
