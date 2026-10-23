#!/usr/bin/env bash
# Link this repo's Sweeq board definition into an upstream qmk_firmware clone
# and register this repo as the QMK external userspace. Safe to re-run.
set -euo pipefail

REPO="$(cd "$(dirname "$0")" && pwd)"
QMK_HOME="${QMK_HOME:-$HOME/Code/qmk_firmware}"

mkdir -p "$QMK_HOME/keyboards/holykeebs"
ln -sfn "$REPO/board/holykeebs/sweeq" "$QMK_HOME/keyboards/holykeebs/sweeq"

# Keep the upstream clone's `git status` clean.
exclude="$QMK_HOME/.git/info/exclude"
grep -qxF 'keyboards/holykeebs/' "$exclude" || echo 'keyboards/holykeebs/' >> "$exclude"

qmk config user.overlay_dir="$REPO"
