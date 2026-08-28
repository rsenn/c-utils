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
# markdown report body, one "## name" section per failure, built up by
# gm_report_generate_failure / gm_report_build_failure and dumped
# verbatim by gm_summary.
GM_REPORT=""
# how many trailing lines of a log to quote in the report
GM_LOG_TAIL=${GM_LOG_TAIL:-20}

gm_info() { printf '  %s\n' "$*" >&2; }

gm_pass() {
  GM_PASS=$((GM_PASS + 1))
  printf 'ok      %s\n' "$1"
}

gm_skip() {
  GM_SKIP=$((GM_SKIP + 1))
  printf 'skip    %s -- %s\n' "$1" "$2"
}

# gm_fail_simple NAME MESSAGE -- a harness-level failure with nothing
# useful to quote (bad arguments to gm_run_case itself, not something
# genmakefile or a build tool did).
gm_fail_simple() {
  GM_FAIL=$((GM_FAIL + 1))
  printf 'FAIL    %s -- %s\n' "$1" "$2"
  GM_REPORT="$GM_REPORT
## $1

- result: $2
"
}

# gm_tail FILE -- last GM_LOG_TAIL lines of FILE, or a placeholder.
gm_tail() {
  if [ -f "$1" ]; then
    tail -n "$GM_LOG_TAIL" "$1"
  else
    echo "(no log)"
  fi
}

# gm_fence_lang FILENAME -- markdown code-fence language tag to use
# when quoting FILENAME's contents.
gm_fence_lang() {
  case "$1" in
    *.ninja) echo ninja ;;
    *.sh) echo sh ;;
    Makefile | *.mk) echo makefile ;;
    *) echo text ;;
  esac
}

# gm_classify_build_failure LOGFILE MAKE_TYPE -- "buildtool" if the
# build tool itself couldn't process the generated file (bad rule,
# parse error, missing target -- a genmakefile-output bug), or
# "subcommand" if the build tool ran fine but a command it invoked
# (the compiler/archiver/linker) failed on its own terms.
gm_classify_build_failure() {
  logfile=$1
  make_type=$2

  case "$make_type" in
    ninja)
      if grep -qE '^FAILED:|: subcommand failed' "$logfile" 2>/dev/null; then
        echo subcommand
      else
        echo buildtool
      fi
      ;;
    make | gmake)
      if grep -qE '\*\*\* \[.+\] Error [0-9]+' "$logfile" 2>/dev/null; then
        echo subcommand
      else
        echo buildtool
      fi
      ;;
    shell)
      if grep -qE 'syntax error|unexpected (end of file|EOF)|unexpected token' "$logfile" 2>/dev/null; then
        echo buildtool
      else
        echo subcommand
      fi
      ;;
    *)
      echo subcommand
      ;;
  esac
}

# gm_build_cmd_str MAKE_TYPE OUTFILE -- the command line gm_run_build
# actually runs for MAKE_TYPE, for display in the report.
gm_build_cmd_str() {
  case "$1" in
    ninja) echo "ninja -f $2" ;;
    shell) echo "sh -x $2" ;;
    gmake) echo "gmake -f $2" ;;
    make) echo "make -f $2" ;;
  esac
}

# gm_report_generate_failure NAME STATUS CMD LOGFILE [NOTE]
# records a failure of the genmakefile invocation itself (the
# generator, not the build tool it produces a file for).
gm_report_generate_failure() {
  name=$1
  status=$2
  cmd=$3
  logfile=$4
  note=${5:-}

  GM_FAIL=$((GM_FAIL + 1))
  msg="genmakefile exited $status"
  [ -n "$note" ] && msg=$note
  printf 'FAIL    %s -- %s\n' "$name" "$msg"

  GM_REPORT="$GM_REPORT
## $name

- result: genmakefile itself failed (exit $status)
- command: \`$cmd\`

\`\`\`text
$(gm_tail "$logfile")
\`\`\`
"
}

# gm_report_build_failure NAME STATUS FAILKIND GENCMD BUILDDIR BUILDCMD
#                          LOGFILE GENFILE GENFILE_NAME
# records a failure of the build tool (or a subcommand it ran) against
# the file genmakefile produced. When FAILKIND is "buildtool", the
# generated file's full contents are quoted too.
gm_report_build_failure() {
  name=$1
  status=$2
  failkind=$3
  gencmd=$4
  builddir=$5
  buildcmd=$6
  logfile=$7
  genfile=$8
  genfile_name=$9

  GM_FAIL=$((GM_FAIL + 1))

  if [ "$failkind" = buildtool ]; then
    label="build tool itself failed"
  else
    label="a subcommand failed (build tool ran fine)"
  fi

  printf 'FAIL    %s -- %s (exit %s)\n' "$name" "$label" "$status"

  GM_REPORT="$GM_REPORT
## $name

- result: $label (exit $status)
- genmakefile command: \`$gencmd\`
- build command: \`(cd $builddir && $buildcmd)\`

\`\`\`text
$(gm_tail "$logfile")
\`\`\`
"

  if [ "$failkind" = buildtool ] && [ -f "$genfile" ]; then
    GM_REPORT="$GM_REPORT
Generated \`$genfile_name\`:

\`\`\`$(gm_fence_lang "$genfile_name")
$(cat "$genfile")
\`\`\`
"
  fi
}

gm_summary() {
  echo
  echo "genmakefile testsuite: $GM_PASS passed, $GM_FAIL failed, $GM_SKIP skipped"

  if [ "$GM_FAIL" -gt 0 ]; then
    printf '%s\n' "$GM_REPORT"
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
      gm_fail_simple "$name" "unknown tree kind '$kind'"
      return
      ;;
  esac

  gm_dirs_for_partition "$partition" "_out" || {
    gm_fail_simple "$name" "bad partition '$partition'"
    return
  }

  outfile_name=$(gm_outfile_name "$make_type")
  # the generated file's own directory becomes genmakefile's workdir
  # (see the note in gm_dirs_for_partition), so place it under $GM_WORKDIR
  outfile_rel="$GM_WORKDIR/$outfile_name"

  genmakefile_log="$caseroot.genmakefile.log"
  build_log="$caseroot.build.log"

  # shellcheck disable=SC2089
  genmakefile_cmd="genmakefile -t $compiler -m $make_type -I. -Ilib -Ilib/extra -Isrc src/main.c lib/ --create-bins --create-libs -d $GM_BUILDDIR -O $GM_OUTDIR -o $outfile_rel $extra_args"

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
    gm_report_generate_failure "$name" "$gm_status" "$genmakefile_cmd" "$genmakefile_log"
    return
  fi

  if [ ! -f "$srcroot/$outfile_rel" ]; then
    gm_report_generate_failure "$name" "$gm_status" "$genmakefile_cmd" "$genmakefile_log" \
      "genmakefile did not write $outfile_rel"
    return
  fi

  build_dir="$srcroot/$GM_WORKDIR"
  build_cmd=$(gm_build_cmd_str "$make_type" "$outfile_name")

  gm_run_build "$make_type" "$build_dir" "$outfile_name" >"$build_log" 2>&1
  build_status=$?

  if [ "$build_status" -eq 127 ]; then
    gm_skip "$name" "build tool for $make_type not on PATH"
    return
  fi

  if [ "$build_status" -ne 0 ]; then
    failkind=$(gm_classify_build_failure "$build_log" "$make_type")
    gm_report_build_failure "$name" "$build_status" "$failkind" "$genmakefile_cmd" \
      "$build_dir" "$build_cmd" "$build_log" "$srcroot/$outfile_rel" "$outfile_name"
    return
  fi

  gm_pass "$name"
}
