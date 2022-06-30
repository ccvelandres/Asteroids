#!/bin/bash

TARGETS=("i686-w64-mingw32" "x86_64-w64-mingw32")
TOOLCHAINS=("${SCRIPT_DIR}/../toolchain/mingw32.cmake" "${SCRIPT_DIR}/../toolchain/mingw64.cmake")

function check_thread_model() {
    local __cmd=$1
    local __threadmodel=$2
    [[ ! -z $3 ]] && local -n __model=$3 || local __model
    __model="$(${__cmd} -v 2>&1 | grep "^Thread model" | sed -r 's/^Thread model: (.*)$/\1/')"

    [[ "$__model" != "$__threadmodel" ]] && return 1 || return 0
}

function i686-w64-mingw32-cmake() { 
    cmake -DCMAKE_INSTALL_PREFIX=/usr/i686-w64-mingw32 -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAINS[0]} "$@"
}
function x86_64-w64-mingw32-cmake() {
    cmake -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAINS[1]} "$@"
}

function verify_thread_model() {
    # Check if thread model is posix
    for TARGET in "${TARGETS[@]}"; do
        local exts=("gcc" "g++")
        for ext in "${exts[@]}"; do 
            local model=""
            info "Checking ${TARGET}-${ext}..."
            if ! check_thread_model "${TARGET}-${ext}" "posix" model; then
                info "Updating alternatives for $TARGET-${ext}, using posix variant for thread model"
                # grab path for posix
                local posix=$(update-alternatives --list ${TARGET}-${ext} | grep posix)
                [[ -z $posix ]] && fatal "No alternatives for $TARGET"

                update-alternatives --set "${TARGET}-${ext}" "$posix"
                [[ $? != 0 ]] && fatal "Failed to set alternatives for ${TARGET}-${ext} ($posix)"
                info "${TARGET}-${ext} set to $posix"
            fi
        done
    done
}

verify_thread_model