#!/bin/bash
set -E
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";
PROJECT_DIR="$(realpath "$SCRIPT_DIR/../..")"

function fatal() { echo -ne "\x1b[31mFATAL\x1b[0m: "; echo $@; exit 1; }
function info()  { echo -ne "\x1b[32mINFO\x1b[0m: " ; echo $@; }
function debug() { echo -ne "\x1b[34mDEBUG\x1b[0m: "; echo $@; }
function warn()  { echo -ne "\x1b[33mWARN\x1b[0m: " ; echo $@; }
function trim_path() { echo "$(echo "$1" | sed -e "s@${2}@@g" | sed -e 's@^/@@g')"; }

help_text="
Compiles shaders
Usage: $(basename "$0") <variable>=<value>
Available variables:
    INPUTDIR
    GLSLANGVALIDATOR_BIN        : default: glslangValidator
    OUTDIR                      : default: <project_root>/build/shaders"

for arg in "$@"; do
  [[ -z $2 && -z $inputlist ]] && inputurl="$1"
	case "$arg" in
	-h|--help)
		echo "$help_text"
		exit 0
		;;&
	*) 
        if [[ $1 == "--"* ]]; then 
            declare ${1/--/}="$2";
        elif [[ $1 =~ ([^=]*)=(.*) ]]; then 
            declare ${BASH_REMATCH[1]}="${BASH_REMATCH[2]}";
        fi
        ;;
	esac
	shift
done

# defaults for options
[[ -z $GLSLANGVALIDATOR_BIN ]] && GLSLANGVALIDATOR_BIN="glslangValidator"
[[ -z $OUTDIR ]] && OUTDIR="${PROJECT_DIR}/build/shaders"

INPUTDIR=$(realpath "$INPUTDIR")

# Sanity checks
! command -v "$GLSLANGVALIDATOR_BIN" >/dev/null 2>&1 && fatal "$GLSLANGVALIDATOR_BIN not found"
[[ ! -d "$OUTDIR" ]] && mkdir -p "$OUTDIR"

# List of possible extensions for shader files
SHADER_EXT=(".vert" ".vs" ".frag" ".fs" ".gs" ".geom" ".comp" ".tesc" ".tese" ".rgen" ".rint" ".rahit" ".rchit" ".rmiss" ".rcall" ".mesh" ".task")

info "GLSLANGVALIDATOR_BIN: $GLSLANGVALIDATOR_BIN"
info "OUTDIR: $OUTDIR"

##############################################
# OpenGL 
SHADER_OPENGL_SRC_DIR="$INPUTDIR/opengl"
OUTPUT_OPENGL_DIR="$OUTDIR/opengl"
mkdir -p "${OUTPUT_OPENGL_DIR}"
find ${SHADER_OPENGL_SRC_DIR} -type f | while read f; do
    # Skip if extension does not match
    echo ${SHADER_EXT[@]} | grep "${f##*.}" &> /dev/null || continue
    f_shader_path="$(trim_path "$f" "$SHADER_OPENGL_SRC_DIR")" # $(echo "$f" | sed -e "s@${SHADER_OPENGL_SRC_DIR}@@g" | sed -e 's@^/@@g')
    f_output_path="${OUTPUT_OPENGL_DIR}/$f_shader_path"
    # make sure subdir is created
    mkdir -p "$(dirname "$f_output_path")"
    echo -n "Found: $f_shader_path"
    cp "$f" "${f_output_path}"
    echo " - compiled to $(trim_path "$f_output_path" "$OUTDIR")"
done


# SHADER_SRC_DIR="shaders"
# OUTPUT_DIR="build/out/shaders"
# mkdir -p "${OUTPUT_DIR}"
# find ${SHADER_SRC_DIR} -type f | while read f; do
#     # Skip if extension does not match
#     echo ${SHADER_EXT[@]} | grep "${f##*.}" &> /dev/null || continue
#     f_shader_path=$(echo "$f" | sed -e "s@${SHADER_SRC_DIR}@@g" | sed -e 's@^/@@g')
#     f_output_path="${OUTPUT_DIR}/$f_shader_path"
#     # make sure subdir is created
#     mkdir -p "$(dirname "$f_output_path")"
#     echo -n "Found: $f_shader_path"
#     COMPILE_LOG="$(${GLSLANGVALIDATOR_BIN} -V --target-env vulkan1.0 -o "${f_output_path}" "$f" 2>1)"
#     if [[ $? != 0 ]]; then
#         echo " - failed to compile $f_shader_path"
#         echo "${COMPILE_LOG[@]}"
#         exit 1
#     fi
#     echo " - compiled to $f_output_path"
# done
