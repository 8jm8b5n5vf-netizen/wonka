#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
if [ "$(uname -s)" != Darwin ]; then echo 'Execute este script no macOS.'; exit 1; fi
if ! command -v cmake >/dev/null && [ -x /Applications/CMake.app/Contents/bin/cmake ]; then
  export PATH="/Applications/CMake.app/Contents/bin:$PATH"
fi
if ! command -v cmake >/dev/null; then echo 'Instale CMake: https://cmake.org/download/'; exit 1; fi
wonka_arch="$(uname -m)"
if [ "$(sysctl -in hw.optional.arm64 2>/dev/null || echo 0)" = 1 ]; then wonka_arch=arm64; fi
case "$wonka_arch" in arm64|x86_64) ;; *) echo "Arquitetura nao suportada: $wonka_arch"; exit 1;; esac
echo "WONKA 0.2.1 - Mac detectado: $wonka_arch. Build universal arm64 + x86_64 para FL nativo ou Rosetta."
cmake -S . -B build-mac -DCMAKE_BUILD_TYPE=Release '-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64' -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
cmake --build build-mac --config Release --parallel 2
ctest --test-dir build-mac -C Release --output-on-failure
echo 'Compilado. Feche FL Studio e execute scripts/install-mac.command para instalar VST3.'
