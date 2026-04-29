#!/usr/bin/env bash
# Usage: 
#   ./render.sh <scene_name> [--open]
#   ./render.sh sphere_grid
#   ./render.sh three_spheres --open

set -euo pipefail

SCENE="${1:?usage: render.sh <scene_name> [--open]}"
OPEN_FLAG="${2:-}"

cmake -B build -S . > /dev/null
cmake --build build --target "${SCENE}" -j
"./build/scenes/${SCENE}"

OUTPUT="renders/${SCENE}.png"
echo "Rendered ${OUTPUT}"

if [ "${OPEN_FLAG}" = "--open" ]; then
    case "$(uname -s)" in
        Darwin) open "${OUTPUT}" ;;
        Linux)  xdg-open "${OUTPUT}" ;;
        *)      echo "Don't know how to open on this platform" ;;
    esac
fi