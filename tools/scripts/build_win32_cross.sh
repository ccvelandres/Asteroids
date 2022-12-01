#!/bin/sh

SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
PROJECT_DIR="${SCRIPT_DIR}/../.."

rm -rf ${PROJECT_DIR}/build/win32
mkdir -p ${PROJECT_DIR}/build/win32
# Configure
cmake -DCMAKE_TOOLCHAIN_FILE=${PROJECT_DIR}/tools/toolchain/mingw64.cmake \
    -B${PROJECT_DIR}/build/win32 \
    ${PROJECT_DIR}
# Build
cmake --build ${PROJECT_DIR}/build/win32 \
    --parallel