#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
if [ "$(uname -s)" != Darwin ]; then echo 'Execute este script no macOS.'; exit 1; fi
if pgrep -x 'FL Studio' >/dev/null; then echo 'Feche o FL Studio antes de substituir o plugin.'; exit 1; fi
wonka_source="$PWD/build-mac/WONKA_artefacts/Release/VST3/WONKA.vst3"
if [ ! -d "$wonka_source" ]; then echo 'Compile primeiro: bash scripts/build-mac.command'; exit 1; fi
wonka_destination="$HOME/Library/Audio/Plug-Ins/VST3"
wonka_backup="$HOME/Library/Application Support/WonkaAudio/Backups"
mkdir -p "$wonka_destination" "$wonka_backup"
if [ -e "$wonka_destination/WONKA.vst3" ]; then
 mv "$wonka_destination/WONKA.vst3" "$wonka_backup/WONKA-$(date +%Y%m%d-%H%M%S).vst3"
fi
ditto "$wonka_source" "$wonka_destination/WONKA.vst3"
echo 'WONKA instalado. No FL: Manage plugins > Verify plugins > Rescan previously verified plugins > Find installed plugins.'
echo 'Confirme v0.2.0 no rodape. A copia anterior foi preservada em Application Support/WonkaAudio/Backups.'
