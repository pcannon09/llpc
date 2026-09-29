#!/usr/bin/env bash

if [[ ! -f ".root_dir" ]]; then
	echo "Need a \`.root_dir\` file for indication"
	echo "Execute \`gen.sh\` at the repo root"
	exit 1
fi

set -e

echo "[CREATE] Creating 'efi.img' file"

EFI_IMAGE="$PWD/build/img/efi.img"
EFI_DIR="$PWD/build/bin"

if [[ ! -d "$(dirname "$EFI_IMAGE")" ]]; then
	mkdir -p "$(dirname "$EFI_IMAGE")"
fi

rm -f "$EFI_IMAGE"

dd if=/dev/zero \
	of="$EFI_IMAGE" \
	bs=1M \
	count=64 \
	status=progress

mkfs.fat -F 32 -v "$EFI_IMAGE"

export MTOOLS_VERBOSE=1

mmd -i "$EFI_IMAGE" ::/EFI
mmd -i "$EFI_IMAGE" ::/EFI/BOOT

mcopy -i "$EFI_IMAGE" \
	"$EFI_DIR/EFI/BOOT/BOOTX64.EFI" \
	::/EFI/BOOT/BOOTX64.EFI

