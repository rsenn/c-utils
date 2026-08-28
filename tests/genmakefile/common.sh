#!/bin/sh
# Common functions for the genmakefile shell testsuite.
#
# Sourced by testsuite.sh (and any additional *-scenarios.sh files); not
# meant to be run directly.
#
# Env overrides:
#   GENMAKEFILE   path to the genmakefile binary (default: auto-detect
#                 under build/*/genmakefile relative to the repo root)
#   GM_KEEP       if set to 1, don't delete the scratch dir on exit

GM_THISDIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
GM_REPO_ROOT=$(CDPATH= cd -- "$GM_THISDIR/../.." && pwd)

# --- genmakefile binary discovery -------------------------------------

GENMAKEFILE=${GENMAKEFILE:-}
GM_KEEP=${GM_KEEP:-}

if [ -z "$GENMAKEFILE" ]; then
  for d in x86_64-linux-gnu x86_64-linux-debug x86_64-linux-profile; do
    if [ -x "$GM_REPO_ROOT/build/$d/genmakefile" ]; then
      GENMAKEFILE="$GM_REPO_ROOT/build/$d/genmakefile"
      break
    fi
  done
fi

if [ -z "$GENMAKEFILE" ] || [ ! -x "$GENMAKEFILE" ]; then
  echo "genmakefile binary not found -- build it first (cmake --build build/<dir> --target genmakefile)" >&2
  echo "or set GENMAKEFILE=/path/to/genmakefile" >&2
  exit 1
fi

# --- scratch workspace ---------------------------------------------------

GM_WORKROOT=$(mktemp -d "${TMPDIR:-/tmp}/genmakefile-testsuite.XXXXXX")

gm_cleanup() {
  [ "$GM_KEEP" = "1" ] || rm -rf "$GM_WORKROOT"
}
trap gm_cleanup EXIT INT TERM

# --- bookkeeping -----------------------------------------------------

GM_PASS=0
GM_FAIL=0
GM_SKIP=0
GM_FAILED_NAMES=""
# one "name:logpath" record per failure (logpath may be empty), used by
# gm_summary to print every failed case's full log at the end.
GM_FAILED_LOGS=""

gm_info() { printf '  %s\n' "$*" >&2; }

gm_pass() {
  GM_PASS=$((GM_PASS + 1))
  printf 'ok      %s\n' "$1"
}

# gm_fail NAME MESSAGE [LOGFILE...] -- LOGFILE(s), if given, are
# concatenated into the combined failure report gm_summary prints.
gm_fail() {
  name=$1
  message=$2
  shift 2
  GM_FAIL=$((GM_FAIL + 1))
  GM_FAILED_NAMES="$GM_FAILED_NAMES
$name -- $message"
  GM_FAILED_LOGS="$GM_FAILED_LOGS
$name:$*"
  printf 'FAIL    %s -- %s\n' "$name" "$message"
}

gm_skip() {
  GM_SKIP=$((GM_SKIP + 1))
  printf 'skip    %s -- %s\n' "$1" "$2"
}

gm_summary() {
  echo
  echo "genmakefile testsuite: $GM_PASS passed, $GM_FAIL failed, $GM_SKIP skipped"

  if [ "$GM_FAIL" -gt 0 ]; then
    echo "failed cases:"
    printf '%s\n' "$GM_FAILED_NAMES" | sed '/^$/d; s/^/  - /'

    echo
    echo "=== combined logs for failed cases ==="
    while IFS=: read -r case_name logfiles; do
      [ -z "$case_name" ] && continue
      for logfile in $logfiles; do
        echo
        echo "----- $case_name: $logfile -----"
        if [ -f "$logfile" ]; then
          cat "$logfile"
        else
          echo "(no log file)"
        fi
      done
    done <<EOF
$GM_FAILED_LOGS
EOF

    return 1
  fi

  return 0
}

# --- mock source trees -------------------------------------------------
#
# Both trees have the same shape (matching the layout that exposed the
# --create-libs directory bugs found in the pictest project):
#
#   src/main.c            (the one file with main())
#   lib/foo.c, lib/foo.h
#   lib/extra/bar.c, lib/extra/bar.h
#
# so a single genmakefile invocation always exercises: a program
# directory, a top-level library directory with no separator-free
# basename edge case ("lib"), and a nested library directory
# ("lib/extra").

# mock_tree_host DIR -- hosted/gcc-style tree: freestanding of any PIC
# toolchain assumptions, links and runs as a normal executable.
mock_tree_host() (
  dir=$1
  mkdir -p "$dir/src" "$dir/lib/extra"

  cat >"$dir/src/main.c" <<'EOF'
#include <stdio.h>
#include "foo.h"
#include "bar.h"

int
main(void) {
  int result = foo() + bar();
  printf("%d\n", result);
  return result == 3 ? 0 : 1;
}
EOF

  cat >"$dir/lib/foo.c" <<'EOF'
#include "foo.h"

int
foo(void) {
  return 1;
}
EOF

  cat >"$dir/lib/foo.h" <<'EOF'
int foo(void);
EOF

  cat >"$dir/lib/extra/bar.c" <<'EOF'
#include "bar.h"

int
bar(void) {
  return 2;
}
EOF

  cat >"$dir/lib/extra/bar.h" <<'EOF'
int bar(void);
EOF
)

# mock_tree_pic DIR -- xc8/sdcc-style tree: no libc, no main() return
# value depended on by an OS, safe to compile freestanding for an 8-bit
# PIC target.
mock_tree_pic() (
  dir=$1
  mkdir -p "$dir/src" "$dir/lib/extra"

  cat >"$dir/src/main.c" <<'EOF'
#include "foo.h"
#include "bar.h"

int
main(void) {
  int result = foo() + bar();
  while(1) {
    if(result == 3)
      break;
  }
  return 0;
}
EOF

  cat >"$dir/lib/foo.c" <<'EOF'
#include "foo.h"

int
foo(void) {
  return 1;
}
EOF

  cat >"$dir/lib/foo.h" <<'EOF'
int foo(void);
EOF

  cat >"$dir/lib/extra/bar.c" <<'EOF'
#include "bar.h"

int
bar(void) {
  return 2;
}
EOF

  cat >"$dir/lib/extra/bar.h" <<'EOF'
int bar(void);
EOF
)

# --- directory-layout partitions ----------------------------------------
#
# genmakefile has three independently settable directories:
#   builddir (-d)  where objects/binaries/libraries are written
#   outdir   (-O)  reference dir sources are made relative to
#   workdir        the directory the generated build file lives in
#                   (derived from the dirname of -o; there is a -w flag
#                   but it is overridden whenever -o is also given, so
#                   we don't use it here)
#
# There are 5 set partitions of {workdir, builddir, outdir}: all equal,
# each possible pair equal with the third different, and all different.

# gm_dirs_for_partition PARTITION BASEDIR
# sets GM_WORKDIR, GM_BUILDDIR, GM_OUTDIR (paths under BASEDIR).
gm_dirs_for_partition() {
  partition=$1
  base=$2

  case "$partition" in
    all-same)
      GM_WORKDIR="$base/out"
      GM_BUILDDIR="$GM_WORKDIR"
      GM_OUTDIR="$GM_WORKDIR"
      ;;
    work-build-same)
      GM_WORKDIR="$base/wb"
      GM_BUILDDIR="$GM_WORKDIR"
      GM_OUTDIR="$base/o"
      ;;
    work-out-same)
      GM_WORKDIR="$base/wo"
      GM_OUTDIR="$GM_WORKDIR"
      GM_BUILDDIR="$base/b"
      ;;
    build-out-same)
      GM_BUILDDIR="$base/bo"
      GM_OUTDIR="$GM_BUILDDIR"
      GM_WORKDIR="$base/w"
      ;;
    all-different)
      GM_WORKDIR="$base/w"
      GM_BUILDDIR="$base/b"
      GM_OUTDIR="$base/o"
      ;;
    *)
      echo "gm_dirs_for_partition: unknown partition '$partition'" >&2
      return 1
      ;;
  esac
}

GM_PARTITIONS="all-same work-build-same work-out-same build-out-same all-different"

# --- running the generated build -----------------------------------------

# gm_outfile_name MAKE_TYPE -- the filename genmakefile should write.
gm_outfile_name() {
  case "$1" in
    ninja) echo "build.ninja" ;;
    shell) echo "build.sh" ;;
    *) echo "Makefile" ;;
  esac
}

# gm_run_build MAKE_TYPE DIR OUTFILE -- invoke the build tool that
# corresponds to MAKE_TYPE against DIR/OUTFILE. Echoes nothing; returns
# the build tool's exit status (127 if the tool itself isn't installed).
gm_run_build() (
  make_type=$1
  dir=$2
  outfile=$3

  cd "$dir" || return 1

  case "$make_type" in
    ninja)
      command -v ninja >/dev/null 2>&1 || return 127
      ninja -f "$outfile"
      ;;
    shell)
      sh -x "$outfile"
      ;;
    gmake)
      command -v gmake >/dev/null 2>&1 || return 127
      gmake -f "$outfile"
      ;;
    make)
      command -v make >/dev/null 2>&1 || return 127
      make -f "$outfile"
      ;;
    *)
      echo "gm_run_build: unknown make type '$make_type'" >&2
      return 1
      ;;
  esac
)

# --- toolchain availability ---------------------------------------------

# gm_compiler_available COMPILER -- 0 if the underlying compiler binary
# for -t COMPILER is on PATH.
gm_compiler_available() {
  case "$1" in
    gcc) command -v gcc >/dev/null 2>&1 ;;
    sdcc) command -v sdcc >/dev/null 2>&1 ;;
    xc8) command -v sdcc >/dev/null 2>&1 || command -v xc8-cc >/dev/null 2>&1 ;;
    *) command -v "$1" >/dev/null 2>&1 ;;
  esac
}

# --- the actual per-case driver -----------------------------------------

# gm_run_case NAME KIND COMPILER MAKE_TYPE PARTITION [EXTRA_GENMAKEFILE_ARGS...]
#   NAME       short id for the test log
#   KIND       host | pic  -- which mock_tree_* to use
#   COMPILER   -t value (gcc, sdcc, xc8, ...)
#   MAKE_TYPE  make | gmake | ninja | shell
#   PARTITION  one of $GM_PARTITIONS
gm_run_case() {
  name=$1
  kind=$2
  compiler=$3
  make_type=$4
  partition=$5
  shift 5
  extra_args=$*

  if ! gm_compiler_available "$compiler"; then
    gm_skip "$name" "compiler for -t $compiler not found on PATH"
    return
  fi

  case "$make_type" in
    gmake) command -v gmake >/dev/null 2>&1 || { gm_skip "$name" "gmake not on PATH"; return; } ;;
    ninja) command -v ninja >/dev/null 2>&1 || { gm_skip "$name" "ninja not on PATH"; return; } ;;
  esac

  caseroot="$GM_WORKROOT/$name"
  srcroot="$caseroot/src-tree"
  mkdir -p "$srcroot"

  case "$kind" in
    host) mock_tree_host "$srcroot" ;;
    pic) mock_tree_pic "$srcroot" ;;
    *)
      gm_fail "$name" "unknown tree kind '$kind'"
      return
      ;;
  esac

  gm_dirs_for_partition "$partition" "_out" || {
    gm_fail "$name" "bad partition '$partition'"
    return
  }

  outfile_name=$(gm_outfile_name "$make_type")
  # the generated file's own directory becomes genmakefile's workdir
  # (see the note in gm_dirs_for_partition), so place it under $GM_WORKDIR
  outfile_rel="$GM_WORKDIR/$outfile_name"

  genmakefile_log="$caseroot.genmakefile.log"
  build_log="$caseroot.build.log"

  (
    cd "$srcroot" || exit 1
    mkdir -p "$GM_WORKDIR" "$GM_BUILDDIR" "$GM_OUTDIR"

    # shellcheck disable=SC2086
    "$GENMAKEFILE" \
      -t "$compiler" -m "$make_type" \
      -I. -Ilib -Ilib/extra -Isrc \
      src/main.c lib/ \
      --create-bins --create-libs \
      -d "$GM_BUILDDIR" -O "$GM_OUTDIR" \
      -o "$outfile_rel" \
      $extra_args
  ) >"$genmakefile_log" 2>&1
  gm_status=$?

  if [ "$gm_status" -ne 0 ]; then
    gm_fail "$name" "genmakefile exited $gm_status" "$genmakefile_log"
    return
  fi

  if [ ! -f "$srcroot/$outfile_rel" ]; then
    gm_fail "$name" "genmakefile did not write $outfile_rel" "$genmakefile_log"
    return
  fi

  gm_run_build "$make_type" "$srcroot/$GM_WORKDIR" "$outfile_name" >"$build_log" 2>&1
  build_status=$?

  if [ "$build_status" -eq 127 ]; then
    gm_skip "$name" "build tool for $make_type not on PATH"
    return
  fi

  if [ "$build_status" -ne 0 ]; then
    gm_fail "$name" "$make_type build exited $build_status" "$genmakefile_log" "$build_log"
    return
  fi

  gm_pass "$name"
}
