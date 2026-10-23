#!/usr/bin/env bash
# Link this repo's Sweeq board definition into an upstream qmk_firmware clone
# and register this repo as the QMK external userspace. Safe to re-run.
set -euo pipefail

die() { echo "setup.sh: $*" >&2; exit 1; }

REPO="$(cd "$(dirname "$0")" && pwd)"
# Prefer an explicit QMK_HOME, then the clone `qmk setup` recorded.
QMK_HOME="${QMK_HOME:-$(qmk config -ro user.qmk_home 2>/dev/null | sed -n 's/^user\.qmk_home=//p')}"
QMK_HOME="${QMK_HOME:-$HOME/Code/qmk_firmware}"

[ -d "$QMK_HOME/keyboards" ] || die "not a qmk_firmware clone: $QMK_HOME (set QMK_HOME)"

target="$QMK_HOME/keyboards/holykeebs/sweeq"
if [ -e "$target" ] && [ ! -L "$target" ]; then
    die "$target exists and is not a symlink; move it aside first"
fi
mkdir -p "$(dirname "$target")"
ln -sfn "$REPO/board/holykeebs/sweeq" "$target"

# Keep the upstream clone's `git status` clean. Resolve the path through git so
# worktrees (where .git is a file) work too.
exclude="$(git -C "$QMK_HOME" rev-parse --path-format=absolute --git-path info/exclude)"
mkdir -p "$(dirname "$exclude")"
touch "$exclude"
if ! grep -qxF 'keyboards/holykeebs/' "$exclude"; then
    # Don't glue the pattern onto a last line that lacks a newline.
    [ -s "$exclude" ] && [ "$(tail -c 1 "$exclude")" != "" ] && echo >> "$exclude"
    echo 'keyboards/holykeebs/' >> "$exclude"
fi

qmk config user.overlay_dir="$REPO"
