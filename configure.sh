#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<USAGE
configure.sh --check|--build

Checks required build dependencies (Qt5, g++, make, autotools). If --build is
given and all checks pass, runs the build in src/.

Environment:
  QMAKE   Override qmake binary (default: qmake-qt5 or qmake)
USAGE
}

have() { command -v "$1" >/dev/null 2>&1; }

pick_qmake() {
  if [[ -n "${QMAKE:-}" && -x "$(command -v "$QMAKE" 2>/dev/null || true)" ]]; then
    echo "$QMAKE"
    return 0
  fi
  if have qmake-qt5; then echo qmake-qt5; return 0; fi
  if have qmake; then echo qmake; return 0; fi
  echo ""; return 1
}

check_qmake5() {
  local qm; qm=$(pick_qmake || true)
  if [[ -z "$qm" ]]; then
    echo "[X] qmake (Qt5) not found. Install Qt5 (qt5-qmake)." >&2
    return 1
  fi
  local ver; ver=$($qm -v 2>/dev/null | tr -d '\r') || true
  if echo "$ver" | grep -qi 'Qt version 5'; then
    echo "[✔] $qm ($ver)"
  else
    echo "[X] $qm is not Qt5: $ver" >&2
    return 1
  fi
}

check_cmd() {
  local c=$1 msg=${2:-}
  if have "$c"; then echo "[✔] $c"; else echo "[X] $c $msg" >&2; return 1; fi
}

do_check() {
  local ok=0
  check_qmake5 || ok=1
  check_cmd g++ "(C++11 compiler required)" || ok=1
  check_cmd make || ok=1
  # autotools for ODE bootstrap
  check_cmd autoconf || ok=1
  check_cmd automake || ok=1
  check_cmd libtool || ok=1
  # optional: OpenGL presence is platform-specific; assume dev packages installed per README
  # Deeper header/link checks
  # Qt headers
  local qm qt_hdr
  qm=$(pick_qmake || true)
  if [[ -n "$qm" ]]; then
    qt_hdr=$($qm -query QT_INSTALL_HEADERS 2>/dev/null | tr -d '\r' || true)
    if [[ -n "$qt_hdr" && -d "$qt_hdr" ]]; then
      if [[ -f "$qt_hdr/QtWidgets/QApplication"* || -f "$qt_hdr/QtWidgets/qapplication.h" ]]; then
        echo "[✔] QtWidgets headers"
      else
        echo "[!] QtWidgets headers not found under $qt_hdr (continuing)" >&2
      fi
      if [[ -f "$qt_hdr/QtGui/QOpenGLFunctions"* || -f "$qt_hdr/QtGui/qopenglfunctions.h" ]]; then
        echo "[✔] QtGui OpenGL headers"
      else
        echo "[!] QtGui OpenGL headers not found under $qt_hdr (continuing)" >&2
      fi
      if [[ -f "$qt_hdr/QtXml/qdomdocument.h" ]]; then
        echo "[✔] QtXml headers"
      else
        echo "[!] QtXml headers not found under $qt_hdr (continuing)" >&2
      fi
    fi
  fi
  # GL headers
  if [[ -f /usr/include/GL/gl.h ]]; then echo "[✔] GL header"; else echo "[!] Missing /usr/include/GL/gl.h (continuing)" >&2; fi
  if [[ -f /usr/include/GL/glu.h ]]; then echo "[✔] GLU header"; else echo "[!] Missing /usr/include/GL/glu.h (continuing)" >&2; fi
  # tiny link test for -lGL if possible
  if have g++; then
    tmpc="./henoc_gltest_$$.cpp"
    tmpe="./henoc_gltest_$$"
    cat > "$tmpc" <<'CPP'
#include <GL/gl.h>
int main(){ (void)glGetError(); return 0; }
CPP
    if g++ "$tmpc" -lGL -o "$tmpe" >/dev/null 2>&1; then echo "[✔] Link test: -lGL"; else echo "[!] Link test for -lGL failed (continuing)" >&2; fi
    rm -f "$tmpc" "$tmpe"
  fi
  if [[ $ok -ne 0 ]]; then
    echo "\nOne or more dependencies are missing. See README.md Troubleshooting." >&2
    return 1
  fi
}

do_build() {
  local qm; qm=$(pick_qmake)
  echo "Building with $qm"
  QMAKE="$qm" make -C src
}

case "${1:-}" in
  --check) do_check ;;
  --build) do_check && do_build ;;
  -h|--help|*) usage; exit 1 ;;
esac
