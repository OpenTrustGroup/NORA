#!/bin/bash
# Copyright 2026 Open Trust Group. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# Build what `fx run-venus` boots in QEMU: the hypervisor image (venus-hee) on
# zircon built from source, linux 6.6 and its root file system, TF-A, and QEMU
# itself; the TEE image (venus-tee) comes prebuilt with the assets. (At this
# tag `--tee none` does not boot.) No network: bazel and what its workspaces
# download come from the release assets (out/open-ddk-bazel/).
#
#   nora/build.sh
#
# The image's packages are garnet's with NORA's over them
# (nora/product/default): NORA's package, garnet/packages/nora.
#
# Then, in a shell that sourced scripts/env.sh:  fx run-venus -z -9 -d
# (and -x in tmux: the serial consoles open as tmux windows).
set -eo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
# A jiri checkout of manifest/NORA (README.md), with garnet and zircon's
# source among its projects.
for d in garnet zircon; do
  [ -d "$d" ] || { echo "error: no $d/ here: check NORA out with jiri (README.md)" >&2; exit 1; }
done

# GN looks for a BUILD.gn that is not in its place in one secondary tree,
# which the build's dotfile names (build/secondary/). NORA's builds run GN
# with a copy of that dotfile naming nora/gn-stubs/ instead, which holds
# stubs for what a GN file refers to and NORA does not carry (the nebula
# SDK); gn-stubs/third_party leads back to build/secondary/third_party.
# The copy is written for each build, so that it follows the build's.
mkdir -p out/nora
sed 's|^secondary_source = .*|secondary_source = "//nora/gn-stubs/"|' \
  build/gn/dotfile.gn > out/nora/dotfile.gn
grep -qx 'secondary_source = "//nora/gn-stubs/"' out/nora/dotfile.gn ||
  { echo "error: build/gn/dotfile.gn names no secondary_source" >&2; exit 1; }

source scripts/env.sh
bash nora/build-zircon.sh

# The QEMU board's package set: garnet's packages with NORA's over them,
# less trim.txt's, resolved from NORA's package, and for both images their
# boot images (bootdata-combo-qemu-virt), nothing else of //build/bootfs. An
# import or a package GN cannot find stops the build here, naming it.
P="$ROOT/out/nora/pkgroot"; Q="$ROOT/out/nora/packages"
rm -rf "$P" "$Q"; mkdir -p "$P/garnet" "$Q"
cp -r garnet/packages "$P/garnet/"
cp -r nora/product/default/buildroot_packages/. "$P/"
python3 nora/trim-packages.py "$P" "$ROOT/out/nora/trimmed"
python2 nora/product-packages.py --root "$P" --package garnet/packages/nora --out "$Q/nora"
echo '{ "labels": [ "//build/bootfs:bootdata-combo-qemu-virt" ] }' > "$Q/bootfs"
# The TEE image is prebuilt (out/venus-tee/arm64/bootdata-combo-qemu-virt.bin,
# from the assets): only the hypervisor is built.
[ -f out/venus-tee/arm64/bootdata-combo-qemu-virt.bin ] ||
  { echo "error: no TEE image (out/venus-tee/arm64/); run nora/fetch-assets.sh" >&2; exit 1; }
# `fx set-venus venus-hee`, with these package files in place of the
# project's own: set-venus's `gn gen` loads every package of
# garnet/packages/venus-hee and build/packages/bootfs, and with them sources
# NORA does not publish. The same args.gn and .config otherwise (VENUS_TEE:
# set-venus's --tee, nebula by default).
d=out/venus-hee/arm64
mkdir -p "$d"; rm -rf out/arm64; ln -s "$ROOT/$d" out/arm64
{
  echo 'target_cpu = "arm64"'
  echo 'zircon_project = "venus-hee"'
  echo 'bootfs_packages = true'
  echo 'with_syzkaller = 0'
  echo 'nebula_tee_services = false'
  echo 'zircon_nebula_api = true'
  if [ -n "${CCACHE_DIR:-}" ] && [ -d "$CCACHE_DIR" ]; then echo 'use_ccache = true'; fi
  echo 'fuchsia_packages = [ "out/nora/packages/nora", "out/nora/packages/bootfs" ]'
} > "$d/args.gn"
buildtools/gn gen "$d" --root=. --dotfile="$ROOT/out/nora/dotfile.gn" --check
(source scripts/devshell/lib/vars.sh && zircon_project=venus-hee variant= &&
 fx-config-write out/arm64 "" linux_6.6 2.10 "${VENUS_TEE:-nebula}" 0)
# The combo images' rules do not list the boot shim as an input: rebuild
# them each time, so that a changed shim reaches them.
rm -f "$d"/bootdata-combo-qemu-virt*.bin
fx build

source scripts/devshell/lib/vars.sh
source buildtools/vars.sh
fx-config-read
FUCHSIA_DIR="$ROOT" source build/bazel_symlink.sh
python3 nora/qemu/adapt-bazel.py "$ROOT"

# bazel itself and every file its workspaces download, from the assets
B="$ROOT/out/open-ddk-bazel"
export BAZELISK_HOME="$B/bazelisk"
CACHE=(--repository_cache="$B/repos")
# no bazel server left behind
trap '(cd bazel/ree && bazelisk shutdown) 2>/dev/null; (cd bazel/host && bazelisk shutdown) 2>/dev/null' EXIT

# the version string the REE carries, written where `fx bazel-ree` writes it
(cd third_party/ree-trusty && cat > version.bzl <<EOF2
"""Nebula Version Bazel Helper"""

NEBULA_VERSION_STR = "\"$(git describe --always --tags)\""

# The following provides "NEBULA_VERSION_STR" to genrule
def _nebula_version_str_impl(ctx):
  return [
    platform_common.TemplateVariableInfo({
      "NEBULA_VERSION_STR": NEBULA_VERSION_STR
    })
  ]

nebula_version_str = rule(
  implementation = _nebula_version_str_impl,
)
EOF2
)

# REE: linux 6.6 and the root file system (what `fx bazel-ree` builds for
# QEMU, with its toolchain settings)
CLANG_TOOLCHAIN_PREFIX="${BUILDTOOLS_CLANG_DIR}/bin"
GCC_TOOLCHAIN_PREFIX="${LINARO_AARCH64_TOOLCHAIN_HOME}/bin"
GCC_TOOLCHAIN_SYSROOT="${LINARO_AARCH64_TOOLCHAIN_HOME}/aarch64-linux-gnu/libc"
(cd bazel/ree && bazelisk build "${CACHE[@]}" --config=linux_6.6 \
   //:linux-image-6.6 //:linux-rootfs \
   --action_env=CLANG_TOOLCHAIN_PREFIX="$CLANG_TOOLCHAIN_PREFIX" \
   --action_env=CLANG_TOOLCHAIN_SYSROOT="$GCC_TOOLCHAIN_SYSROOT" \
   --action_env=GCC_TOOLCHAIN_PREFIX="$GCC_TOOLCHAIN_PREFIX" \
   --action_env=GCC_TOOLCHAIN_SYSROOT="$GCC_TOOLCHAIN_SYSROOT" \
   --action_env=CCACHE_DIR="$CCACHE_DIR" \
   --action_env=TEE_SPD="$TEE_SPD")
mkdir -p out/arm64/linux
ln -sf "$BAZEL_REE_OUT/linux/arch/arm64/boot/Image" out/arm64/linux/Image
ln -sfn "$BAZEL_REE_OUT/linux" out/build-linux
ln -sfn "$BAZEL_REE_OUT/rootfs" out/build-linux-rootfs

# QEMU with GRT's machine options, and its serial terminal
(cd bazel/host && bazelisk build "${CACHE[@]}" //:qemu //:soc_term)

fx build-atf
echo "built; to run:  source scripts/env.sh && fx run-venus -z -9 -d"
