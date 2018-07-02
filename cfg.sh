cfg() {
 (: ${generator="${SYSTEM:-Unix} Makefiles"}
  : ${prefix:=/usr${host:+/$host}}
  CROSS_COMPILE=${host:+$host-}
  if [ "$CROSS_COMPILE" ]; then
    echo "Building for $host" 1>&2
      case "$host" in
          *rpi*) : ${TOOLCHAIN_FLIE:=arm-rpi3.cmake} ;;
          *osxcross*|*apple*|*darwin*) : ${TOOLCHAIN_FILE:=osxcross-apple-darwin.cmake} ;;
          *android*)
            : ${TOOLCHAIN_FILE:=/opt/android-cmake/android.cmake}
            set -- "$@" \
    -DANDROID_NATIVE_API_LEVEL="android-19"
            : ${K:=android} ;;
      esac
      [ -n "$TOOLCHAIN_FILE" ] && TOOLCHAIN_FILE=`realpath "$TOOLCHAIN_FILE"`
      [ -z "$PKG_CONFIG" ] &&  export PKG_CONFIG="${CROSS_COMPILE}pkg-config"
      [ -n "$host" -a -z "$PKG_CONFIG_PATH" ] &&    export PKG_CONFIG_PATH="/usr/${host}/sys-root/lib/pkgconfig:/usr/${host}/sys-root/mingw/lib/pkgconfig"
    fi
  case "$TYPE" in
      *[Dd]ebug) : ${builddir=build/${K-cmake}-debug} ;;
      *[Rr]elease) : ${builddir=build/${K-cmake}-release} ;;
       *[Pp]rof*) CXXFLAGS="-g -pg -O2"; : ${builddir=build/${K-cmake}-profile}; TYPE=RelWithDebInfo ;;
      *[Rr]el[Ww]ith[Dd]eb*) : ${builddir=build/${K-cmake}-relwithdebinfo} ;;
      *[Mm]in[Ss]ize[Rr]el*) : ${builddir=build/${K-cmake}-minsizerel} ;;
      *) : ${builddir=build/${K-cmake}} ;;
  esac
  case "$TYPE" in
    *[Ss]tatic*)  SHARED_LIBS=OFF ;;
    * | *[Ss]hared*)  SHARED_LIBS=ON ;;
  esac
  mkdir -p $builddir 
  relsrcdir=`realpath --relative-to="$builddir" "$(pwd)"`
  set -x;
  cd $builddir
  ${CMAKE:-cmake} -Wno-dev \
    -DCMAKE_INSTALL_PREFIX="$prefix" \
    -G "$generator" \
    ${VERBOSE+-DCMAKE_VERBOSE_MAKEFILE=TRUE} \
    ${TOOLCHAIN_FILE:+-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE} \
    \
    -DCMAKE_BUILD_TYPE="${TYPE:-RelWithDebInfo}" \
    ${CXXFLAGS:+-DCMAKE_CXX_FLAGS="$CXXFLAGS"} \
    "$@" \
  "$relsrcdir")
  #done
}

cfg-all() {
  for b in '' -gtk -qt4 -qt5; do 
    (builddir=build/cmake$b
    set -- "$@" \
    -DCMAKE_PREFIX_PATH=/opt/Qt5.10.1/5.10.1/gcc_64 
     case "$b" in
         *gtk) set -- "$@" \
    -D{USE_,}GTK=ON ;;
         *qt4) set -- "$@" \
    -D{USE_,}QT4=ON ;;
         *qt5) set -- "$@" \
    -DUSE_QT5=ON \
    -D{QT_DIR,Qt5_DIR}="/opt/Qt5.10.1/5.10.1/gcc_64" \
    -DQT_MOC_EXECUTABLE="/opt/Qt5.10.1/5.10.1/gcc_64/bin/moc" ;;
         *) set -- "$@" \
    -DUSE_QT{,4,5}=OFF \
    -DUSE_GTK=OFF ;;
    esac
    cfg "$@" ) || return $?
  done
}

cfg-android() {
 (builddir=build/android/
  cfg \
    -DCMAKE_VERBOSE_MAKEFILE="TRUE" \
    -DUSE_QT5="ON" \
    -DCMAKE_TOOLCHAIN_FILE="/opt/android-cmake/android.cmake" \
    -DANDROID_NATIVE_API_LEVEL="21" \
    -DCMAKE_C_COMPILER="/usr/bin/arm-linux-androideabi-gcc" \
    -DCMAKE_CXX_COMPILER="/usr/bin/arm-linux-androideabi-g++" \
    -DCMAKE_SYSROOT="/opt/android-19/sysroot" \
    -DPKG_CONFIG_EXECUTABLE="arm-linux-androideabi-pkg-config" \
    -DCMAKE_PREFIX_PATH="/opt/android-qt5/5.10.1/armeabi-v7a" \
    -DCMAKE_MAKE_PROGRAM="/usr/bin/make" \
  "$@")
}

cfg-mingw() {
 (builddir=build/mingw/
  cfg \
  -DCMAKE_VERBOSE_MAKEFILE=TRUE \
  -DUSE_QT5=ON \
  -DCMAKE_C_COMPILER=/usr/bin/$host-gcc \
  -DCMAKE_CXX_COMPILER=/usr/bin/$host-g++ \
    ${SYSROOT:+-DCMAKE_SYSROOT="$SYSROOT"} \
  -DPKG_CONFIG_EXECUTABLE=$host-pkg-config \
  -DCMAKE_PREFIX_PATH=/usr/$host \
     "$@")
}

cfg-android() {
  (builddir=build/android
    pathmunge "/opt/Qt5.10.1/5.10.1/android_armv7/bin"
   unset TOOLCHAIN TOOLCHAIN_FILE
   export CMAKE
    set -x
    PKG_CONFIG_PATH="/opt/Qt5.10.1/5.10.1/android_armv7/lib/pkgconfig:/usr/local/arm-androideabi-libraries/lib/pkgconfig:/opt/arm-linux-androideabi/sysroot/usr/lib/pkgconfig:/opt/android-ndk-r10e/platforms/android-16/arch-arm/lib/pkgconfig"
    export PKG_CONFIG_PATH
      cfg \
    -DUSE_QT5=ON \
    -DUSE_QML=ON \
    -DCMAKE_TOOLCHAIN_FILE="/opt/android-cmake/android.cmake" \
    -DCMAKE_INSTALL_PREFIX="/opt/arm-linux-androideabi/sysroot/usr" \
    -DCMAKE_PREFIX_PATH="/opt/Qt5.10.1/5.10.1/android_armv7" \
    -DCMAKE_VERBOSE_MAKEFILE=TRUE \
    -DCMAKE_MAKE_PROGRAM="/usr/bin/make" \
      "$@")
}

android-cfg() { 
  ( K=opensuse ANDROID_SDK=/opt/android-sdk TOOLCHAIN_FILE=$(pwd)/qt-android-cmake/toolchain/android.toolchain.cmake TYPE=RelWithDebInfo cfg \
    -DUSE_QT5=ON \
    -DUSE_QML=ON \
    -DQt5_DIR=/opt/Qt5.10.1/5.10.1/android_armv7 \
    -DCMAKE_PREFIX_PATH=/opt/Qt5.10.1/5.10.1/android_armv7/lib/cmake \
    -DQT_MOC_EXECUTABLE=/opt/Qt5.10.1/5.10.1/android_armv7/bin/moc \
    -DCMAKE_VERBOSE_MAKEFILE=TRUE \
      "$@" 
    )
}

cfg-outoftree() {
  for G in codeblocks codelite eclipse kate ninja sublime; do
    (case "$G" in
          ninja) generator="Ninja" ;;
          codeblocks) generator="CodeBlocks - Ninja" ;;
          codelite) generator="CodeLite - Ninja" ;;
          sublime) generator="Sublime Text 2 - Ninja" ;;
          kate) generator="Kate - Ninja" ;;
          eclipse) generator="Eclipse CDT4 - Ninja" ;;
    esac
    builddir="../build-tanqram-$G" generator="$generator" cfg "$@"
    ); done
}

