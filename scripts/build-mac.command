#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
if ! command -v cmake >/dev/null; then
  echo 'Instale o CMake: https://cmake.org/download/ (ou brew install cmake)'
  exit 1
fi
cmake -S . -B build-mac -DCMAKE_BUILD_TYPE=Release '-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64' -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
cmake --build build-mac --config Release --parallel 2
ctest --test-dir build-mac -C Release --output-on-failure
echo 'Compilado em build-mac/WONKA_artefacts/Release. Consulte README.md para instalar no FL Studio.'
