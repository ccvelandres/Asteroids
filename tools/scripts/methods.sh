
function check_sha() {
    local -n __file=$1
    local -n __shasums=$2
    if ! echo "${__shasums[@]}" | grep -q "$(sha256sum $__filepath | awk '{print $1}')"; then
        return 1
    fi
    return 0
}

function get_sources()
{
    local -n __ppdir=$1
    local -n __sourcedir=$2
    local -n __sources=$3
    local -n __files=$4
    local -n __sha256sums=$5

    info "Retrieving sources..."
    for source in ${__sources[@]}; do
        local __url
        local __filename
        local __filepath

        # Fix local sources
        source=$(echo "$source" | sed -e "s@file://@file://${__ppdir}/@")

        # Check if filename is specified
        if grep -q "::" <<< $source >/dev/null; then
            __filename="$(awk '{split($0,a,"::"); print a[1]}' <<< $source)"
            __url="$(awk '{split($0,a,"::"); print a[2]}' <<< $source)"

            [[ -z $__url ]] && fatal "Empty url from $source"
            [[ -z $__url ]] && fatal "Empty filename from $source"
        else
            __url="$source"

            # Retrieve filename from remote location
            __filename="$(curl -sIkL "$url" | sed -r '/filename=/!d;s/.*filename=(.*)$/\1/')"
            # Get the filename from url if no remote filename
            [[ -z $__filename ]] && __filename="${__url##*/}"
        fi

        # Sanity check that url and filename is set
        debug "source: $source"
        debug "__url: $__url"
        debug "__filename: $__filename"
        [[ -z $__url ]] && fatal "Could not resolve source url"
        [[ -z $__filename ]] && fatal "Could not resolve source filename"

        # Get the filepath on sourcedir
        __filepath="$(realpath "$__sourcedir/$__filename")"
        debug "__filepath: $__filepath"

        # Check if we already downloaded from source
        if [[ -f "$__filepath" ]]; then
            if check_sha __filepath __sha256sums; then
                info "Cached $__filename"
            else 
                # SHA mismatch, remove cached file
                rm "$__filepath"
            fi
        fi
        
        if [[ ! -f "$__filepath" ]]; then
            for retries in {0..3}; do
                info -e "Downloading $__url...\r"
                if curl -L "$__url" -o "$__filepath" >/dev/null 2>&1; then
                    # Check if sha matches, if not retry
                    if check_sha __filepath __sha256sums; then
                        info "Downloaded $__filename from $__url"
                        break
                    else
                        warn "Retry download: SHA mismatch"
                    fi
                else
                    if [[ $retries < 3 ]]; then
                        warn "Retrying to download $__filename"
                        continue
                    else
                        break
                    fi
                fi
            done
        fi

        [[ ! -f "$__filepath" ]] && fatal "Download failed for $__url"

        # # Check for sha
        # if ! echo "${__sha256sums[@]}" | grep -q "$(sha256sum $__filepath | awk '{print $1}')"; then
        #     fatal "SHA256 failed for $__filename ($(sha256sum $__filepath | awk '{print $1}'))"
        # fi
        __files+=("$__filepath")
    done
}

function process_sources() {
    local -n __sourcedir=$1
    local -n __files=$2

    # Post-process (extract archives etc)
    for file in ${__files[@]}; do
        local __filename="$(basename "$file")"
        [[ $__filename == *.tar.gz ]] && { tar -xf "$file" -C "$__sourcedir"; continue; }
        [[ $__filename == *.tgz ]] && { tar -xf "$file" -C "$__sourcedir"; continue; }
    done
}

function install_pkg() {
    local -n __pkgdir=$1
    local -n __installprefix=$2
    info "Installing $__pkgdir to $__installprefix"

    # Move /usr/local to /usr
    [[ -d $__pkgdir/usr/local ]] && { mv -t $__pkgdir/usr/ $__pkgdir/usr/local/*; rm -rf $__pkgdir/usr/local; }

    find "$__pkgdir"/usr/ -type f | while read file; do
        local __sourcepath="$file"
        local __installpath="$__installprefix/""${__sourcepath#*$__pkgdir/usr/}"
        local __installdir="$(dirname "$__installpath")"
        info -e "'$__sourcepath' -> '$__installpath'"
        install -d "$__installdir"
        install "$__sourcepath" "$__installpath"
    done
}

function check_pkg_var() {
    local __optional=$1
    local __varname=$2
    local -n __varvalue=$2
    if [[ -z "$__varvalue" ]]; then
        [[ $__optional == 0 ]] && fatal "$__varname was not defined" || warn "$__varname is undefined"
    fi
}

function create_src_dir() {
    local -n __pkgname=$1
    local -n __basedir=$2

    SRCDIR="$__basedir/${__pkgname}"
    mkdir -p "$SRCDIR"
}

function create_pkg_dir() {
    local -n __srcdir=$1
    local -n __pkgname=$2

    PKGDIR="$__srcdir/pkg-$__pkgname"
    [[ -d "$PKGDIR" ]] && rm -rf "$PKGDIR"
    mkdir -p "$PKGDIR"
}

function clean_src_dir() {
    local -n __srcdir=$1
    local -n __files=$2
    find "$__srcdir"/ -maxdepth 1 -mindepth 1 | while read f; do
        local artifact=1
        for _f in "${__files[@]}"; do 
            if [[ "$f" == "$_f" ]]; then
                artifact=0
                break
            fi
        done
        if [[ $artifact != 0 ]]; then
            info "Removing artifacts from previous build: $f"
            rm -r "$f"
        fi
    done
}