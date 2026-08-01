#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
BUILD_TYPE="${BUILD_TYPE:-Release}"
EXAMPLES_DIR="${SCRIPT_DIR}/examples"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

echo "Building"
cmake -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" ..
cmake --build . --parallel
echo "Build complete: ${BUILD_DIR}"

if ! command -v slangc >/dev/null 2>&1; then
    echo "ERROR: slangc not found on PATH, cannot compile shaders"
    exit 1
fi

echo ""
echo "Compiling shaders and linking assets" 

for EXAMPLE_PATH in "${EXAMPLES_DIR}"/*/; do
    EXAMPLE_NAME="$(basename "${EXAMPLE_PATH}")"
    SHADER_SRC="${EXAMPLE_PATH}assets/shaders/${EXAMPLE_NAME}.slang"
    EXAMPLE_BUILD_DIR="${BUILD_DIR}/examples/${EXAMPLE_NAME}"

    if [ ! -d "${EXAMPLE_BUILD_DIR}" ]; then
        echo "  [${EXAMPLE_NAME}] skip: no build output at ${EXAMPLE_BUILD_DIR}"
        continue
    fi

    # Shaders
    if [ -f "${SHADER_SRC}" ]; then
        SHADER_OUT_DIR="${EXAMPLE_BUILD_DIR}/shaders"
        mkdir -p "${SHADER_OUT_DIR}"

        echo "  [${EXAMPLE_NAME}] compiling ${EXAMPLE_NAME}.slang"

        slangc "${SHADER_SRC}" \
            -profile glsl_450 \
            -target spirv \
            -entry vertexMain \
            -stage vertex \
            -o "${SHADER_OUT_DIR}/${EXAMPLE_NAME}.vert.spv"

        slangc "${SHADER_SRC}" \
            -profile glsl_450 \
            -target spirv \
            -entry fragmentMain \
            -stage fragment \
            -o "${SHADER_OUT_DIR}/${EXAMPLE_NAME}.frag.spv"
    else
        echo "  [${EXAMPLE_NAME}] no shader found at ${SHADER_SRC}, skipping"
    fi

    # Textures (optional)
    TEXTURES_SRC="${EXAMPLE_PATH}assets/textures"
    if [ -d "${TEXTURES_SRC}" ]; then
        TEXTURES_LINK="${EXAMPLE_BUILD_DIR}/textures"
        rm -rf "${TEXTURES_LINK}"
        ln -s "${TEXTURES_SRC}" "${TEXTURES_LINK}"
        echo "  [${EXAMPLE_NAME}] symlinked textures"
    fi

    # Models (optional) 
    MODELS_SRC="${EXAMPLE_PATH}assets/models"
    if [ -d "${MODELS_SRC}" ]; then
        MODELS_LINK="${EXAMPLE_BUILD_DIR}/models"
        rm -rf "${MODELS_LINK}"
        ln -s "${MODELS_SRC}" "${MODELS_LINK}"
        echo "  [${EXAMPLE_NAME}] symlinked models"
    fi
done

echo ""
echo "Done"
