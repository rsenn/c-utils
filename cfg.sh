cfg() { 
  case $(uname -o) in
    MSys|MSYS|Msys) SYSTEM="MSYS" ;;
    *) SYSTEM="Unix" ;;
  esac

  case "$TYPE" in
      [Dd]ebug) builddir=build/cmake-debug ;;
      [Rr]elease) builddir=build/cmake-release ;;
      [Rr]el[Ww]ith[Dd]eb*) builddir=build/cmake-relwithdebinfo ;;
      [Mm]in[Ss]ize[Rr]el*) builddir=build/cmake-minsizerel ;;
      *) builddir=build/cmake ;;
  esac

#  for builddir in build/cmake{-debug,,-release,-minsizerel}
#  do
#    case "$builddir" in
#            *debug*) TYPE=Debug ;;
#            *release*) TYPE=Release ;;
#            *minsiz*) TYPE=MinSizeRel ;;
#            *) TYPE=RelWithDebInfo ;;
#    esac
 (mkdir -p $builddir
  set -x
  cd $builddir
  cmake -Wno-dev -DCMAKE_INSTALL_PREFIX="${prefix-/usr}" \
    -G "${SYSTEM:-MSYS} Makefiles" \
    ${VERBOSE+-DCMAKE_VERBOSE_MAKEFILE=TRUE} \
    -DCMAKE_BUILD_TYPE="${TYPE:-RelWithDebInfo}" \
    "$@" \
  ../..)
  #done
}
