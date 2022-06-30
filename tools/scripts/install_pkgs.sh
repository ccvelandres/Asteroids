#!/bin/bash
set -E
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";

function fatal() { echo -ne "\x1b[31mFATAL\x1b[0m: "; echo $@; exit 1; }
function info()  { echo -ne "\x1b[32mINFO\x1b[0m: " ; echo $@; }
function debug() { echo -ne "\x1b[34mDEBUG\x1b[0m: "; echo $@; }
function warn()  { echo -ne "\x1b[33mWARN\x1b[0m: " ; echo $@; }
# trap '[ "$?" -ne 77 ] || exit 77' ERR # For exiting parent shell from subshell

help_text="
Installs dependencies for building the core project
Usage: $(basename "$0") <variable>=<value>
Available variables:
    PLATFORM        : (mingw-w64, linux)
    INSTALL_PREFIX  : defaults: mingw-w64 (32-bit) =/usr/i686-w64-mingw32
                                          (64-bit) =/usr/x86_64-w64-mingw32
    LOG_DIR         : default: SCRIPT_DIR/logs/<PLATFORM>
    TEMP_DIR        : default: /tmp
    JOBS            : default: 1"

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

PLATFORMS=("mingw-w64", "linux")
[[ -z $PLATFORM ]] && fatal "PLATFORM unspecified"
if echo "${PLATFORMS[@]}" | grep "$PLATFORM" >/dev/null 2>&1; then
    info "PLATFORM: $PLATFORM"
else
    fatal "Platform: $PLATFORM is unsupported. Available platform are (${PLATFORMS[@]})"
fi

if [[ -z $INSTALL_PREFIX ]]; then
    [[ "$PLATFORM" == "mingw-w64" ]] && INSTALL_PREFIX="/usr"
    [[ "$PLATFORM" == "linux" ]] && INSTALL_PREFIX="/usr/local"
fi
info "INSTALL_PREFIX: $INSTALL_PREFIX"
# Set defaults
[[ -z $TEMP_DIR ]] && TEMP_DIR="/tmp"
[[ -z $LOG_DIR ]] && LOG_DIR="$SCRIPT_DIR/logs/$PLATFORM"
[[ -z $JOBS ]] && JOBS=1

# Source helper methods
source $SCRIPT_DIR/methods.sh

# Source manifest for target
MANIFEST_FILE="$SCRIPT_DIR/$PLATFORM/manifest.sh"
[[ ! -f $MANIFEST_FILE ]] && fatal "Could not find manifest file: $MANIFEST_FILE"
source "$MANIFEST_FILE"

# Source package list
PACKAGE_FILE="$SCRIPT_DIR/packages.sh"
[[ ! -f $PACKAGE_FILE ]] && fatal "Could not find package file: $PACKAGE_FILE"
source "$PACKAGE_FILE"

# Create log directory
mkdir -p "$LOG_DIR"

info "Temporary files: $TEMP_DIR"
info "Log Directory: $LOG_DIR"
info "Install Dir: $INSTALL_PREFIX"
info "Installing packages: ${PACKAGES[@]}"


set -o pipefail
for PACKAGE in "${PACKAGES[@]}"; do
    # Do everything in a subshell
    (
        PP_DIR="$SCRIPT_DIR/$PLATFORM/$PACKAGE"
        PP_FILE="$PP_DIR/pkg"
        info "Processing $PACKAGE for $PLATFORM (${PP_FILE#$SCRIPT_DIR/})"
        
        # Source the pkg file
        [[ ! -f $PP_FILE ]] && fatal "Could not find pkg file"
        source $PP_FILE

        # Check pkg variables
        check_pkg_var 0 pkgname
        check_pkg_var 1 pkgver
        check_pkg_var 1 sources
        check_pkg_var 1 sha256sums

        # Create pkg dir and define SRCDIR and PKGDIR
        create_src_dir pkgname TEMP_DIR
        info "SRCDIR: $SRCDIR"

        # Retrieve sources
        files=()
        get_sources PP_DIR SRCDIR sources files sha256sums

        # Clean src dir for artifacts
        clean_src_dir SRCDIR files

        # Process sources (extract, etc)
        process_sources SRCDIR files

        # Create pkgdir
        create_pkg_dir SRCDIR pkgname

        # Call prepare
        debug "Calling prepare"
        [[ $(type -t prepare) == function ]] && { 
            ( cd "$SRCDIR" && prepare )
            [[ $? != 0 ]] && fatal "Error during prepare"
        }

        # Call build
        debug "Calling build"
        [[ $(type -t build) == function ]] && {
            ( cd "$SRCDIR" && build )
            [[ $? != 0 ]] && fatal "Error during build"
        }

        # Call package
        debug "Calling package"
        [[ $(type -t package) == function ]] && {
            ( cd "$SRCDIR" && package )
            [[ $? != 0 ]] && fatal "Error during package"
        }

        # Install
        install_pkg PKGDIR INSTALL_PREFIX
     exit 0
    ) | tee "$LOG_DIR/$PACKAGE.log"
    [[ $? != 0 ]] && fatal "Install failed for $PACKAGE"
done