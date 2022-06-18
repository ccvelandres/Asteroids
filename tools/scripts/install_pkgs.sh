#!/bin/bash
DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";

function get_sources()
{
    local -n _sources=$1
    local -n _files=$2
    local -n _sha256sums=$3
    echo "Retrieving sources..."
    for source in ${_sources[@]}; do
        [[ $source == file://* ]] && source=$(echo "$source" | sed -e "s@file://@file://${sourcedir}/@")
        _files+=("$(curl -LO "$source" -sw "%{filename_effective}")")
        echo "  -> Found ${source}" 
    done
    for file in ${_files[@]}; do
        [[ $file == *.tar.gz ]] && { tar -xf "${file}"; continue; }
        [[ $file == *.tgz ]] && { tar -xf "${file}"; continue; }
    done
}

function install_pkg() {
    echo "Installing $pkgdir"
    # Move /usr/local to /usr
    [[ -d $pkgdir/usr/local ]] && { mv -t $pkgdir/usr/ $pkgdir/usr/local/*; rm -rf $pkgdir/usr/local; }

    find $pkgdir -type f | while read file; do
        sourcepath="$file"
        installpath="${file#*$pkgdir}"
        installdir="$(dirname "$installpath")"
        echo "'$sourcepath' -> '$installpath'"
        install -d "$installdir"
        install "$sourcepath" "$installpath"
    done
}

infile=$1
[[ ! -f $infile ]] && {
    echo "Could not find input file"
}
source $infile

echo "Installing for $target"

for pkg in "${pkgs[@]}"; do
    {
        sourcedir="${DIR}/${target}/$pkg"
        pkgfile="${sourcedir}/pkg"
        pkglog="$LOGPATH$(basename "${pkg}.log")"
        echo "Processing ${pkg} -> logs($pkglog)"
        exec 3>$pkglog
        {
            source $pkgfile
            # Create pkg directory
            [[ -d "${pkgname}" ]] && rm -rf "${pkgname}"
            mkdir "${pkgname}" && cd "${pkgname}"
            srcdir="$(pwd)"
            pkgdir="${srcdir}/pkg-${pkgname}"
            mkdir -p "$pkgdir"

            # Download sources
            [[ -z "$sources" ]] && { echo "sources undefined"; }
            [[ -z "$sha256sums" ]] && { echo "sha256sums undefined"; }
            files=()
            get_sources sources files sha256sums

            # Call prepare
            cd "$srcdir"
            [[ $(type -t prepare) == function ]] && prepare

            # Call build
            cd "$srcdir"
            [[ $(type -t build) == function ]] && build

            # Call package
            cd "$srcdir"
            [[ $(type -t package) == function ]] && package

            # Install
            install_pkg "${pkgdir}"
        } >&3 2>&3
        exec 3<&-
    }
done