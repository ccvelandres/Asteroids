#!/bin/bash

DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";
PROJECT_DIR="$DIR/.."
cd "$PROJECT_DIR"

SHADER_SRC_DIR="shaders"
OUTPUT_DIR="build/out/shaders"
GLSLANGVALIDATOR_BIN="glslangValidator"

mkdir -p "${OUTPUT_DIR}"

# List of possible extensions for shader files
SHADER_EXT=(".vert" ".vs" ".frag" ".fs" ".gs" ".geom" ".comp" ".tesc" ".tese" ".rgen" ".rint" ".rahit" ".rchit" ".rmiss" ".rcall" ".mesh" ".task")

find ${SHADER_SRC_DIR} -type f | while read f; do
    # Skip if extension does not match
    echo ${SHADER_EXT[@]} | grep "${f##*.}" &> /dev/null || continue
    f_shader_path=$(echo "$f" | sed -e "s@${SHADER_SRC_DIR}@@g" | sed -e 's@^/@@g')
    f_output_path="${OUTPUT_DIR}/$f_shader_path"
    # make sure subdir is created
    mkdir -p "$(dirname "$f_output_path")"
    echo -n "Found: $f_shader_path"
    COMPILE_LOG="$(${GLSLANGVALIDATOR_BIN} -V --target-env vulkan1.0 -o "${f_output_path}" "$f" 2>1)"
    if [[ $? != 0 ]]; then
        echo " - failed to compile $f_shader_path"
        echo "${COMPILE_LOG[@]}"
        exit 1
    fi
    echo " - compiled to $f_output_path"
done
