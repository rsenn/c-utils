cfg() {
 (if type gcc 2>/dev/null >/dev/null && type g++ 2>/dev/null >/dev/null; then
    : ${CC:=gcc} ${CXX:=g++}
  elif type clang 2>/dev/null >/dev/null && type clang++ 2>/dev/null >/dev/null; then
    : ${CC:=clang} ${CXX:=clang++}
  fi
  echo "builddir=$builddir" 1>&2

  : ${build:=`$CC -dumpmachine | sed 's|-pc-|-|g'`}

  if [ -z "$host" -a -z "$builddir" ]; then
    host=$build
    case "$host" in
      x86_64-w64-mingw32) host="$host"; : ${builddir=build/$host} ;;
      i686-w64-mingw32) host="$host"; : ${builddir=build/$host} ;;
      x86_64-pc-*) host="$host"; : ${builddir=build/$host} ;;
      i686-pc-*) host="$host"; : ${builddir=build/$host} ;;
    esac
  fi
  

  if false && [ -n "$host" -a -z "$prefix" ]; then
    case "$host" in
      x86_64-w64-mingw32) : ${prefix=/mingw64} ;;
      i686-w64-mingw32) : ${prefix=/mingw32} ;;
      x86_64-pc-*) : ${prefix=/usr} ;;
      i686-pc-*) : ${prefix=/usr} ;;
    esac
  fi

  : ${prefix:=/usr/local}
  : ${libdir:=$prefix/lib}
  [ -d "$libdir/$host" ] && libdir=$libdir/$host

  if [ -e "$TOOLCHAIN" ]; then
    cmakebuild=$(basename "$TOOLCHAIN" .cmake)
    cmakebuild=${cmakebuild%.toolchain}
    cmakebuild=${cmakebuild#toolchain-}
    : ${builddir=build/$cmakebuild}
  else
   : ${builddir=build/$host}
  fi
  case "$host:$builddir" in
    *msys*) ;;
    *) test -n "$builddir" && builddir=`echo $builddir | sed 's|-pc-|-|g'` ;;
  esac
    echo "builddir=$builddir" 1>&2

  case $(uname -o) in
   # MSys|MSYS|Msys) SYSTEM="MSYS" ;;
    *) SYSTEM="Unix" ;;
  esac

  case "$STATIC:$TYPE" in
    YES:*|yes:*|y:*|1:*|ON:*|on:* | *:*[Ss]tatic*) set -- "$@" \
      -DBUILD_SHARED_LIBS=OFF \
      -DENABLE_PIC=OFF ;;
  esac

  [ -n "$PKG_CONFIG_PATH" ] && echo "PKG_CONFIG_PATH=$PKG_CONFIG_PATH" 1>&2
  [ -n "$PKG_CONFIG" ] && case "$PKG_CONFIG" in
     */*) ;;
     *) PKG_CONFIG=$(which "$PKG_CONFIG") ;; 
  esac
  : ${generator:="Unix Makefiles"}
  echo "host=$host" 1>&2
  echo "builddir=$builddir" 1>&2

  mkdir -p $builddir
  : ${relsrcdir=`realpath --relative-to "$builddir" .`}
  : set -x
  cd "${builddir:-.}"
  IFS="$IFS "
 set -- -Wno-dev \
    -G "$generator" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    ${prefix:+-DCMAKE_INSTALL_PREFIX="$prefix"} \
    ${VERBOSE:+-DCMAKE_VERBOSE_MAKEFILE=OFF${VERBOSE:-OFF}} \
    -DCMAKE_BUILD_TYPE="${TYPE:-Debug}" \
    -DBUILD_SHARED_LIBS=ON \
    ${CC:+-DCMAKE_C_COMPILER="$CC"} \
    ${CXX:+-DCMAKE_CXX_COMPILER="$CXX"} \
    ${PKG_CONFIG:+-DPKG_CONFIG_EXECUTABLE="$PKG_CONFIG"} \
    ${TOOLCHAIN:+-DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN"} \
    ${CC:+-DCMAKE_C_COMPILER="$CC"} \
    ${CXX:+-DCMAKE_CXX_COMPILER="$CXX"} \
    ${MAKE:+-DCMAKE_MAKE_PROGRAM="$MAKE"} \
    "$@" \
    $relsrcdir 
  eval "${CMAKE:-cmake} \"\$@\""
  ) 2>&1 |tee "${builddir##*/}.log"
}

cfg-diet() {
 (: ${build=$(${CC:-gcc} -dumpmachine | sed 's|-pc-|-|g')}
  : ${host=${build/-gnu/-diet}}
  : ${prefix=/opt/diet}
  : ${libdir=/opt/diet/lib-${host%%-*}}
  : ${bindir=/opt/diet/bin-${host%%-*}}

  CC="diet-gcc"

  export CC

  if [ "${PKG_CONFIG-unset}" = unset ]; then
    if type pkgcfg >/dev/null; then
      export PKG_CONFIG=`LC_ALL=C type pkgcfg 2>&1 |sed 's,.* is ,,'`
    elif type pkg-config >/dev/null; then
      export PKG_CONFIG=`LC_ALL=C type pkg-config 2>&1 |sed 's,.* is ,,'`
    fi
  fi

  : ${builddir=build/${host%-*}-diet}
  prefix=/opt/diet

  echo "PKG_CONFIG='${PKG_CONFIG}'" 1>&2

  export builddir prefix
  PKG_CONFIG_PATH=/opt/diet/lib-x86_64/pkgconfig:/opt/diet/lib/pkgconfig:/usr/lib/diet/lib/pkgconfig \
  cfg \
    -DCMAKE_INSTALL_PREFIX="$prefix" \
    -DBUILD_SSL=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DENABLE_SHARED=OFF \
    -DSHARED_LIBS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_FIND_ROOT_PATH="$prefix" \
    -DCMAKE_SYSTEM_LIBRARY_PATH="$prefix/lib-${host%%-*}" \
      ${launcher:+-DCMAKE_C_COMPILER_LAUNCHER="$launcher"} \
      -DPKG_CONFIG_EXECUTABLE="$PKG_CONFIG" \
    "$@")
}

cfg-diet64() {
 (build=$(gcc -dumpmachine | sed 's|-pc-|-|g')
  host=${build%%-*}-linux-diet
  host=x86_64-${host#*-}

  export prefix=/opt/diet


  builddir=build/$host \
  PKG_CONFIG_PATH=/opt/diet/lib-x86_64/pkgconfig:/usr/lib/diet/lib-x86_64/pkgconfig \
  CC="diet-gcc" \
  cfg-diet \
    -DCMAKE_SYSTEM_LIBRARY_PATH=/opt/diet/lib-x86_64 \
  "$@")
}

cfg-diet32() {
 (build=$(gcc -dumpmachine | sed 's|-pc-|-|g')
  host=${build%%-*}-linux-diet
  host=i686-${host#*-}
  
  if type diet32-clang 2>/dev/null >/dev/null; then
    CC="diet32-clang"
    export CC
  elif type diet32-gcc 2>/dev/null >/dev/null; then
    CC="diet32-gcc"
    export CC
  else
    CC="gcc"
    launcher="/opt/diet/bin-i386/diet"
    CFLAGS="-m32"
    export CC launcher CFLAGS
  fi
  
  builddir=build/$host \
  PKG_CONFIG_PATH=/opt/diet/lib-i386/pkgconfig:/usr/lib/diet/lib-i386/pkgconfig \
  cfg-diet \
    -DCMAKE_SYSTEM_LIBRARY_PATH=/opt/diet/lib-i386 \
    "$@")
}

cfg-mingw() {
 (build=$(gcc -dumpmachine)
  : ${host=${build%%-*}-w64-mingw32}
  : ${prefix=/usr/$host/sys-root/mingw}

  case "$host" in
    x86_64-*) : ${TOOLCHAIN=/opt/cmake-toolchains/mingw64.cmake} ;;
    *) : ${TOOLCHAIN=/opt/cmake-toolchains/mingw32.cmake} ;;
  esac

  PKG_CONFIG=${host}-pkg-config
  : ${PKG_CONFIG_PATH=$prefix/lib/pkgconfig}

  export TOOLCHAIN PKG_CONFIG PKG_CONFIG_PATH
  
  : ${builddir=build/$host}

  bindir=$prefix/bin \
  libdir=$prefix/lib \
  cfg \
    -DPKG_CONFIG_EXECUTABLE=$(which ${host}-pkg-config) \
    "$@")
}
cfg-mingw32() {
  host=i686-w64-mingw32 cfg-mingw "$@"
}
cfg-mingw64() {
  host=x86_64-w64-mingw32 cfg-mingw "$@"
}

cfg-emscripten() {
 (build=$(${CC:-emcc} -dumpmachine | sed 's|-pc-|-|g')
  host=${build/-gnu/-emscriptenlibc}
  builddir=build/${host%-*}-emscripten
  
  prefix=`which emcc | sed 's|/emcc$|/system|'` 
  libdir=$prefix/lib
  bindir=$prefix/bin

  CC="emcc" \
  PKG_CONFIG="PKG_CONFIG_PATH=$libdir/pkgconfig pkg-config" \
  cfg \
    -DCMAKE_INSTALL_PREFIX="$prefix" \
    -DENABLE_SHARED=OFF \
    -DSHARED_LIBS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    "$@")
}

cfg-tcc() {
 (build=$(cc -dumpmachine | sed 's|-pc-|-|g')
  host=${build/-gnu/-tcc}
  builddir=build/$host
  prefix=/usr/local
  includedir=/usr/lib/$build/tcc/include
  libdir=/usr/lib/$build/tcc/
  bindir=/usr/bin

  : ${CC:=${TCC:-tcc}}
  export CC

  cfg \
    "$@")
}

cfg-musl() {
 (: ${build=$(${CC:-gcc} -dumpmachine | sed 's|-pc-|-|g')}
  : ${host=${build%-*}-musl}

 : ${prefix=/opt/musl}
 : ${includedir=$prefix/include/$host}
 : ${libdir=$prefix/lib/$host}
 : ${bindir=$prefix/bin/$host}
  : ${builddir=build/$host}

  CC=/usr/bin/musl-gcc \
  PKG_CONFIG=musl-pkg-config \
  PKG_CONFIG_PATH=/opt/musl/lib/pkgconfig:/usr/lib/${host%%-*}-linux-musl/pkgconfig \
  cfg \
    -DENABLE_SHARED=OFF \
    -DSHARED_LIBS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    "$@")
}


cfg-musl64() {
 (build=$(gcc -dumpmachine | sed 's|-pc-|-|g')
  host=${build%%-*}-linux-musl
  host=x86_64-${host#*-}

  : ${builddir=build/$host}
  
  CFLAGS="-m64" \
  cfg-musl \
  -DCMAKE_C_COMPILER="musl-gcc" \
  "$@")
}

cfg-musl32() {
 (build=$(gcc -dumpmachine | sed 's|-pc-|-|g')
  host=$(echo "$build" | sed "s|x86_64|i686| ; s|-gnu|-musl|")

  : ${builddir=build/$host}

  CFLAGS="-m32" \
  cfg-musl \
  -DCMAKE_C_COMPILER="musl-gcc" \
  "$@")
}

cfg-msys() {
 (echo "host: $host"
  build=$(gcc -dumpmachine)
  : ${host=${build%%-*}-pc-msys}
  : ${prefix=/usr/$host/sys-root/usr}
   echo "host: $host"

  case "$host" in
    x86_64*) TOOLCHAIN=/opt/cmake-toolchains/msys64.cmake ;;
   *) TOOLCHAIN=/opt/cmake-toolchains/msys32.cmake ;;
  esac
  echo "builddir: $builddir"

  export PKG_CONFIG=$host-pkg-config
  export PKG_CONFIG_PATH="$prefix/lib/pkgconfig"

  : ${builddir=build/$host}

  export TOOLCHAIN PKG_CONFIG PKG_CONFIG_PATH

  bindir=$prefix/bin \
  libdir=$prefix/lib \
  host=$host \
  build=$build \
  cfg \
    -DPKG_CONFIG_EXECUTABLE=$(which ${host}-pkg-config) \
    "$@")
}

cfg-msys32() {
  host=i686-pc-msys \
    cfg-msys "$@"
}

cfg-msys64() {
  host=x86_64-pc-msys \
    cfg-msys "$@"
}


# cfg-termux: cross-compile for Termux (aarch64, bionic) with the NDK clang against a Termux sysroot,
# a copy or sshfs mount of $PREFIX (sshfs termux:.. ~/mnt/termux gives ~/mnt/termux/usr).
#   NDK=/mnt/data/opt/android-ndk-r27c  TERMUX_SYSROOT=~/mnt/termux/usr
#   ANDROID_ABI=arm64-v8a  ANDROID_PLATFORM=android-24      (ANDROID_ABI=armeabi-v7a for 32 bit)
# Termux ships its own bionic headers, so its include dir must come after the NDK's: the pkg-config
# wrapper turns -I<sysroot>/include into -idirafter. Binaries get an rpath to the Termux lib dir.
cfg-termux()
{
  (: ${builddir=build/termux}
   : ${NDK:=/mnt/data/opt/android-ndk-r27c}
   # a local copy (make sysroot-termux in js-engines) is fast; the sshfs mount works too but every lookup is a round trip
   if [ -z "${TERMUX_SYSROOT:-}" ]; then
     if [ -d "$HOME/sysroots/termux/usr/lib" ]; then TERMUX_SYSROOT=$HOME/sysroots/termux/usr; else TERMUX_SYSROOT=$HOME/mnt/termux/usr; fi
   fi
   : ${ANDROID_ABI:=arm64-v8a}
   : ${ANDROID_PLATFORM:=android-24}
   : ${TOOLCHAIN:=$NDK/build/cmake/android.toolchain.cmake}
   termux_prefix=/data/data/com.termux/files/usr

   [ -e "$TOOLCHAIN" ] || { echo "cfg-termux: no $TOOLCHAIN (set NDK)" 1>&2; exit 1; }
   [ -d "$TERMUX_SYSROOT/lib" ] || echo "cfg-termux: no Termux sysroot at $TERMUX_SYSROOT (make sysroot-termux in js-engines, or mount it: sshfs termux:.. ~/mnt/termux)" 1>&2

   mkdir -p "$builddir"
   pcwrap=$(cd "$builddir" && pwd)/termux-pkg-config
   cat >"$pcwrap" <<EOF
#!/bin/sh
PKG_CONFIG_PATH=$TERMUX_SYSROOT/lib/pkgconfig:$TERMUX_SYSROOT/share/pkgconfig
export PKG_CONFIG_PATH
out=\$(pkgcfg --path-map $termux_prefix=$TERMUX_SYSROOT "\$@") || exit
printf '%s\\n' "\$out" | sed -E 's#-I$TERMUX_SYSROOT/include( |\$)#-idirafter $TERMUX_SYSROOT/include\\1#g'
EOF
   chmod +x "$pcwrap"
   PKG_CONFIG=$pcwrap
   export PKG_CONFIG TOOLCHAIN

   cfg \
     -DCMAKE_INSTALL_PREFIX=$termux_prefix \
     -DANDROID_ABI=$ANDROID_ABI \
     -DANDROID_PLATFORM=$ANDROID_PLATFORM \
     -DCMAKE_FIND_ROOT_PATH=$TERMUX_SYSROOT \
     -DCMAKE_PREFIX_PATH=$TERMUX_SYSROOT \
     -DCMAKE_MODULE_PATH=$TERMUX_SYSROOT/lib/cmake \
     -DCMAKE_EXE_LINKER_FLAGS=-Wl,-rpath,$termux_prefix/lib \
     -DCMAKE_SHARED_LINKER_FLAGS=-Wl,-rpath,$termux_prefix/lib \
     "$@"
    )
}
cfg-wasm() {
  export VERBOSE
 (EMCC=$(which emcc)
  EMSCRIPTEN=$(dirname "$EMCC");
  EMSCRIPTEN=${EMSCRIPTEN%%/bin*};
  test -f /opt/cmake-toolchains/generic/Emscripten-wasm.cmake && TOOLCHAIN=/opt/cmake-toolchains/generic/Emscripten-wasm.cmake
  test '!' -f "$TOOLCHAIN" && TOOLCHAIN=$(find "$EMSCRIPTEN" -iname emscripten.cmake);
  test -f "$TOOLCHAIN" || unset TOOLCHAIN;
  : ${prefix:="$EMSCRIPTEN"}
  : ${builddir=build/emscripten-wasm}

  CC="$EMCC" \
  cfg \
    -DEMSCRIPTEN_PREFIX="$EMSCRIPTEN" \
    ${TOOLCHAIN:+-DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN"} \
    -DCMAKE_EXE_LINKER_FLAGS="-s WASM=1" \
    -DCMAKE_EXECUTABLE_SUFFIX=".html" \
    -DCMAKE_EXECUTABLE_SUFFIX_INIT=".html" \
    -DUSE_{ZLIB,BZIP,LZMA,SSL}=OFF \
  "$@")
}

cfg-android () 
{ 
    ( build=$(cc -dumpmachine);
    host=arm-linux-androideabi;
    : ${builddir=build/$host};
    PKG_CONFIG_PATH=/opt/${host}/sysroot/usr/lib/pkgconfig:/opt/${host}/sysroot/usr/share/pkgconfig TOOLCHAIN=/opt/cmake-toolchains/android.cmake prefix=/opt/$host/sysroot/usr CMAKE_PREFIX_PATH=/opt/$host/sysroot/usr cfg "$@" )
}
cfg-android64 () 
{ 
    ( : ${builddir=build/android64};
    cfg -DCMAKE_INSTALL_PREFIX=/opt/aarch64-linux-android/sysroot/usr -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN:-/opt/cmake-toolchains/android64.cmake} -DANDROID_NATIVE_API_LEVEL=21 -DPKG_CONFIG_EXECUTABLE=aarch64-linux-android-pkg-config -DCMAKE_PREFIX_PATH=/opt/aarch64-linux-android/sysroot/usr -DCMAKE_MAKE_PROGRAM=/usr/bin/make "$@" )
}

cfg-emscripten() {
  (build=$(cc -dumpmachine | sed 's|-pc-|-|g')
  host=$(emcc -dumpmachine)
  : ${builddir=build/${host%-*}-emscripten}
  : ${prefix=$EMSCRIPTEN/system}
  : ${libdir=$prefix/lib}
  : ${bindir=$prefix/bin}
  : ${EMSCRIPTEN=$EMSDK/upstream/emscripten}
  export TOOLCHAIN="${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake"

PREFIX_PATH=$(set -- /opt/*-wasm;  IFS=";"; echo "$*") 
LIBRARY_PATH=$(set -- /opt/*-wasm/lib ;  IFS=";"; echo "$*") 
  PKG_CONFIG_PATH=$(set -- /opt/*-wasm/lib/pkgconfig; IFS=":"; echo "$*") #${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}
  PKG_CONFIG_PATH="${PKG_CONFIG_PATH:+$PKG_CONFIG_PATH:}${EMSCRIPTEN}/system/lib/pkgconfig"
  export PKG_CONFIG_PATH
  echo PKG_CONFIG_PATH="${PKG_CONFIG_PATH}"
  CC="emcc" CXX="em++" TYPE="Release" \
    CFLAGS="'-sWASM=1 -sUSE_PTHREADS=0 -sLLD_REPORT_UNDEFINED'" \
    CXXFLAGS="'-sWASM=1 -sUSE_PTHREADS=0 -sLLD_REPORT_UNDEFINED'" \
    CMAKE_WRAPPER="emcmake" \
    prefix=/opt/${PWD##*/}-wasm \
    cfg \
    -DCMAKE_PREFIX_PATH="$PREFIX_PATH" \
    -DCMAKE_LIBRARY_PATH="$LIBRARY_PATH" \
    -DENABLE_PIC=FALSE \
    "$@")
}

# cfg-aarch64: cross-compile for a Raspberry Pi (aarch64 glibc) with the host clang (--target=aarch64-linux-gnu, lld)
# against a copy of the Pi's /usr (make sysroot-pi in js-engines puts it in ~/sysroots/pi). No cross gcc is needed.
# Variables: PI_SYSROOT (default ~/sysroots/pi, else the sshfs mount ~/mnt/pi-root), PI_TARGET (default
# aarch64-linux-gnu), prefix (default /usr/local, the install prefix ON the Pi). The sysroot's gcc (usr/lib/gcc,
# usr/include/c++) supplies crt files, libgcc and the C++ headers, so C++ matches the Pi's libstdc++.
cfg-aarch64() {
 (: ${builddir=build/aarch64-linux-gnu}
  : ${PI_TARGET:=aarch64-linux-gnu}
  : ${prefix:=/usr/local}
  if [ -z "${PI_SYSROOT:-}" ]; then
    if [ -d "$HOME/sysroots/pi/usr/lib" ]; then PI_SYSROOT=$HOME/sysroots/pi; else PI_SYSROOT=$HOME/mnt/pi-root; fi
  fi
  [ -d "$PI_SYSROOT/usr/lib" ] || { echo "cfg-aarch64: no Pi sysroot at $PI_SYSROOT (make sysroot-pi in js-engines)" 1>&2; exit 1; }
  type clang >/dev/null 2>&1 && type ld.lld >/dev/null 2>&1 || { echo "cfg-aarch64: needs clang and lld" 1>&2; exit 1; }

  mkdir -p "$builddir"
  bd=$(cd "$builddir" && pwd)
  mt=$PI_TARGET
  tc=$bd/aarch64-toolchain.cmake
  cat >"$tc" <<TCEOF
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_SYSROOT $PI_SYSROOT)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER_TARGET $mt)
set(CMAKE_CXX_COMPILER_TARGET $mt)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_FIND_ROOT_PATH $PI_SYSROOT)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
TCEOF
  pcwrap=$bd/aarch64-pkg-config
  cat >"$pcwrap" <<PCEOF
#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=$PI_SYSROOT
PKG_CONFIG_LIBDIR=$PI_SYSROOT/usr/lib/$mt/pkgconfig:$PI_SYSROOT/usr/lib/pkgconfig:$PI_SYSROOT/usr/share/pkgconfig
unset PKG_CONFIG_PATH
export PKG_CONFIG_SYSROOT_DIR PKG_CONFIG_LIBDIR
exec pkg-config "\$@"
PCEOF
  chmod +x "$pcwrap"
  PKG_CONFIG=$pcwrap TOOLCHAIN=$tc
  export PKG_CONFIG TOOLCHAIN prefix
  cfg -DCMAKE_INSTALL_PREFIX=$prefix "$@")
}
