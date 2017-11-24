cfg () 
{ 
    case $(uname -o) in
        MSys|MSYS|Msys) SYSTEM="MSYS" ;;
        *) SYSTEM="Unix" ;;
    esac


    : ${prefix=/usr}
    : ${builddir=build/cmake}

    ( mkdir -p $builddir;
    cd $builddir;
    set -x
    ${CMAKE-cmake} -Wno-dev -DCMAKE_INSTALL_PREFIX="$prefix" \
    -G "${SYSTEM:-MSYS} Makefiles" \
    -DCMAKE_VERBOSE_MAKEFILE=TRUE \
    -DCMAKE_BUILD_TYPE="${TYPE:-RelWithDebInfo}" \
    "$@" \
     ../.. )
}
