#!/usr/bin/env bash
set -euo pipefail

source /etc/os-release
[[ "$ID" == ubuntu && "$VERSION_ID" == 24.04 && "${VERSION_CODENAME:-}" == noble ]] || {
    echo 'FAIL: requires Ubuntu Desktop 24.04 (noble)' >&2; exit 1;
}
[[ "$(uname -m)" == x86_64 ]] || { echo 'FAIL: requires x86_64' >&2; exit 1; }
check_version() {
    if [[ "$2" != "$3" ]]; then
        printf 'FAIL: %s expected %s, got %s\n' "$1" "$3" "$2" >&2
        exit 1
    fi
    printf 'PASS: %s %s\n' "$1" "$2"
}
check_version Qt "$(qmake6 -query QT_VERSION)" 6.4.2
check_version GCC "$(gcc-13 -dumpfullversion)" 13.3.0
check_version G++ "$(g++-13 -dumpfullversion)" 13.3.0
check_version CMake "$(cmake --version | sed -n '1s/cmake version //p')" 3.28.3
printf 'PASS: Ubuntu %s / %s\n' "$VERSION_ID" "$(uname -m)"
echo 'This checks versions only; desktop, device and PAL validation are separate.'
