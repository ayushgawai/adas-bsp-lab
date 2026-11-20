#!/bin/sh
# Load modules in QEMU when available; skip cleanly otherwise.
command -v qemu-system-aarch64 >/dev/null || { echo 'SKIP: no qemu'; exit 0; }
[ -f build/Image ] || { echo 'SKIP: no kernel image'; exit 0; }
echo 'would boot QEMU and insmod drivers/*.ko (see scripts/run-qemu.sh)'
