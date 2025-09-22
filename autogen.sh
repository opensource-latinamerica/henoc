#!/usr/bin/env bash
set -euo pipefail

if ! command -v autoreconf >/dev/null 2>&1; then
  echo "autoreconf not found; please install autoconf/automake/libtool" >&2
  exit 1
fi

mkdir -p m4 build-aux
exec autoreconf -i

