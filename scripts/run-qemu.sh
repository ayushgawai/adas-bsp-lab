#!/bin/sh
# Boot kernel+DTB on QEMU virt when images exist; otherwise explain what's missing.
set -eu
K=${KERNEL:-build/Image}; D=${DTB:-build/adas-virt.dtb}; R=${ROOTFS:-build/adas-image.ext4}
command -v qemu-system-aarch64 >/dev/null || { echo "SKIP: qemu-system-aarch64 not installed"; exit 0; }
[ -f "$K" ] && [ -f "$D" ] || { echo "SKIP: kernel/DTB not found ($K $D) - build Yocto or: dtc -I dts -O dtb -o $D board/dts/adas-virt.dts"; exit 0; }
exec qemu-system-aarch64 -machine virt -cpu cortex-a57 -m 1024 -nographic -kernel "$K" -dtb "$D" -append "console=ttyAMA0 root=/dev/vda rw" ${R:+ -drive "file=$R,format=raw,if=virtio"} -netdev user,id=n0 -device virtio-net-device,netdev=n0
