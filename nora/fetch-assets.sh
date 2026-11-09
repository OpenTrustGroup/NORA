#!/bin/bash
# Copyright 2026 Open Trust Group. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# Fetch and unpack the binary parts that are not kept in git (toolchains,
# the prebuilt TEE image, bazel and its downloads), checked against
# nora/ASSETS.sha256. They are open-ddk's release assets, less its prebuilt
# zircon.
#
#   nora/fetch-assets.sh [--from DIR_OR_URL]
#
# By default they come from the GitHub releases of the repository this is
# checked out from: <origin>/releases/download/<release>/<file>, the release
# named in ASSETS.sha256 (ASSETS_RELEASE), or for a file an earlier release
# published and this one did not change, that release's ("# <file>:
# <release>"). --from takes a local directory or a base URL with every file.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LIST="$ROOT/nora/ASSETS.sha256"
RELEASE="$(sed -n 's/^# ASSETS_RELEASE=//p' "$LIST")"
FROM=""; BASE=""
[ "${1:-}" = --from ] && FROM="$2"
# (a GitHub release's URL: an earlier release's file from that release)
case $FROM in */releases/download/*) BASE="${FROM%/*}" ;; esac
if [ -z "$FROM" ]; then
  # (this repository's: garnet is a project of its own, its origin the garnet fork)
  origin=$(git -C "$ROOT" remote get-url origin 2>/dev/null || true)
  case "$origin" in
    git@github.com:*) origin="https://github.com/${origin#git@github.com:}" ;;
    ssh://git@github.com/*) origin="https://github.com/${origin#ssh://git@github.com/}" ;;
  esac
  case "$origin" in
    https://github.com/*) BASE="${origin%.git}/releases/download"; FROM="$BASE/$RELEASE" ;;
    *) echo "error: cannot tell where the release assets are (origin:" \
            "${origin:-none}); use --from <dir or URL>" >&2; exit 1 ;;
  esac
fi
echo "assets from $FROM"
DL="$ROOT/.assets"; mkdir -p "$DL"
grep -v '^#' "$LIST" | while read -r sum name; do
  # zircon/ is zircon's whole source, a project of the manifest, which the
  # builds build (nora/build-zircon.sh): its whole toolchains, and no
  # prebuilt zircon nor the part of the toolchains that one needs
  case $name in
    open-ddk-zircon-*) continue ;;
    open-ddk-toolchain-clang-gcc-full-*) ;;
    open-ddk-toolchain-clang-gcc-*) continue ;;
  esac
  f="$DL/$name"
  if [ ! -f "$f" ] || ! echo "$sum  $f" | sha256sum -c --status; then
    # (a file of an earlier release: that release's)
    rel=$(sed -n "s/^# $name: //p" "$LIST")
    if [ -d "$FROM" ]; then cp "$FROM/$name" "$f"
    elif [ -n "$BASE" ]; then curl -fL -o "$f" "$BASE/${rel:-$RELEASE}/$name"
    else curl -fL -o "$f" "$FROM/$name"; fi
    echo "$sum  $f" | sha256sum -c --quiet
  fi
  echo "unpacking $name"
  tar -xJf "$f" -C "$ROOT"
done
echo "assets in place"
