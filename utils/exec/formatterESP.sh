#!/usr/bin/env bash

set -euo pipefail

EFI_FILE="${1:?Usage: $0 <efi-file> <output-vdi>}"
OUTPUT_VDI="${2:?Usage: $0 <efi-file> <output-vdi>}"

IMAGE_DIR="$(dirname "$OUTPUT_VDI")"
IMAGE_NAME="$(basename "$OUTPUT_VDI" .vdi)"
RAW_IMAGE="${IMAGE_DIR}/${IMAGE_NAME}.img"
MOUNT_DIR="$(mktemp -d)"

IMAGE_SIZE_MB=64

cleanup() {
	if mountpoint -q "$MOUNT_DIR"; then
		sudo umount "$MOUNT_DIR"
	fi

	if [[ -n "${LOOP_DEVICE:-}" ]]; then
		sudo losetup -d "$LOOP_DEVICE" 2>/dev/null || true
	fi

	rmdir "$MOUNT_DIR" 2>/dev/null || true
}

trap cleanup EXIT

if [[ ! -f "$EFI_FILE" ]]; then
	echo "error: EFI file does not exist: $EFI_FILE" >&2
	exit 1
fi

mkdir -p "$IMAGE_DIR"

rm -f "$RAW_IMAGE" "$OUTPUT_VDI"

echo "Creating ${IMAGE_SIZE_MB} MiB disk image..."
truncate -s "${IMAGE_SIZE_MB}M" "$RAW_IMAGE"

echo "Creating GPT partition table..."
sudo parted "$RAW_IMAGE" --script \
	mklabel gpt \
	mkpart ESP fat32 1MiB 100% \
	set 1 esp on

echo "Attaching image..."
LOOP_DEVICE="$(sudo losetup --find --show --partscan "$RAW_IMAGE")"

sudo udevadm settle

ESP="${LOOP_DEVICE}p1"

if [[ ! -b "$ESP" ]]; then
	echo "error: EFI partition was not created: $ESP" >&2
	exit 1
fi

echo "Formatting EFI System Partition..."
sudo mkfs.fat -F 32 -n EFI "$ESP"

echo "Mounting EFI System Partition..."
sudo mount "$ESP" "$MOUNT_DIR"

echo "Installing EFI application..."
sudo mkdir -p "$MOUNT_DIR/EFI/BOOT"
sudo cp "$EFI_FILE" "$MOUNT_DIR/EFI/BOOT/BOOTX64.EFI"

sync

echo "EFI image contents:"
sudo find "$MOUNT_DIR" -type f -printf '%P\n'

sudo umount "$MOUNT_DIR"
sudo losetup -d "$LOOP_DEVICE"
unset LOOP_DEVICE

echo "Converting to VDI..."
qemu-img convert \
	-f raw \
	-O vdi \
	"$RAW_IMAGE" \
	"$OUTPUT_VDI"

echo
echo "Created:"
echo "  Raw: $RAW_IMAGE"
echo "  VDI: $OUTPUT_VDI"

