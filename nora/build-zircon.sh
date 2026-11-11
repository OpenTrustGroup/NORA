#!/bin/bash
# Copyright 2026 Open Trust Group. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# Build zircon, the hypervisor's kernel and user programs (venus-hee, debug
# and release), from zircon/'s source: OTG's zircon with GRT's interface.
# build.sh runs this first and goes on with what it makes. The TEE
# (venus-tee) is not built: its image comes with the assets.
#
#   nora/build-zircon.sh
set -eo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
source scripts/env.sh
# Zircon exports the April 2018 libraries and fidlc of its system/legacy,
# which garnet is written against, in place of its own. local.mk rather
# than an argument: the Fuchsia build's gn gen runs Zircon's make too.
grep -qx 'LEGACY_ULIB := 1' zircon/local.mk 2>/dev/null ||
  echo 'LEGACY_ULIB := 1' >> zircon/local.mk
# With clang, as GRT's release flow builds zircon: garnet's link needs what
# clang emits (GCC leaves fbl's placement new out of line).
Z=scripts/build-zircon.sh; A=(USE_CLANG=true WITH_SYZKALLER=0)
$Z -t venus-hee "${A[@]}"
$Z -r -t venus-hee "${A[@]}"
