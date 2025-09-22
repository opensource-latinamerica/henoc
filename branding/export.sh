#!/usr/bin/env bash
set -euo pipefail

# Export all SVGs in this folder to PNG at common sizes.
# Requires one of: inkscape, rsvg-convert (librsvg), or ImageMagick's convert.

SIZES=(16 32 64 128 256)
SRC_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT_DIR="$SRC_DIR/png"
mkdir -p "$OUT_DIR"

run_inkscape() {
  local svg="$1" size="$2" out="$3"
  inkscape "$svg" --export-type=png --export-filename="$out" -w "$size" -h "$size" >/dev/null 2>&1
}

run_rsvg() {
  local svg="$1" size="$2" out="$3"
  rsvg-convert -w "$size" -h "$size" "$svg" -o "$out"
}

run_convert() {
  local svg="$1" size="$2" out="$3"
  convert -background none -resize ${size}x${size} "$svg" "$out"
}

have() { command -v "$1" >/dev/null 2>&1; }

export_one() {
  local svg="$1" base size out
  base="$(basename "$svg" .svg)"
  for size in "${SIZES[@]}"; do
    out="$OUT_DIR/${base}-${size}.png"
    if have inkscape; then
      run_inkscape "$svg" "$size" "$out"
    elif have rsvg-convert; then
      run_rsvg "$svg" "$size" "$out"
    elif have convert; then
      run_convert "$svg" "$size" "$out"
    else
      echo "Error: need inkscape or rsvg-convert or convert (ImageMagick)." >&2
      exit 1
    fi
    echo "Exported $out"
  done
}

shopt -s nullglob
cd "$SRC_DIR"
for svg in *.svg; do
  export_one "$svg"
done
