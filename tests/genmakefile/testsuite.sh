#!/bin/sh
# genmakefile shell testsuite.
#
# Builds a small mock source tree, runs genmakefile over it for a matrix
# of {compiler, build tool, directory layout} combinations, then actually
# invokes the resulting build tool and checks it succeeds.
#
# Compilers are not assumed to be on PATH: gm_discover_compilers globs
# every sdcc/xc8/xc8-cc install found under /opt (plus gcc, if already
# on PATH), and each one found becomes its own row in the matrix, with
# its bin directory prepended to PATH only for that row's build-tool run.
#
# By default this stops after the first DISTINCT failure (see -n/--num)
# rather than exhausting the whole matrix, so it's cheap to call
# repeatedly (e.g. from an agent hunting one bug at a time) without
# burning time/tokens re-discovering failures already known about.
# Subcommand (toolchain) failures dedup per compiler instance already
# (same underlying issue regardless of build tool/layout), so "distinct"
# roughly means "genuinely different problem", not "different case".
#
# Usage: tests/genmakefile/testsuite.sh [-n|--num N] [--all]
# Env:   GENMAKEFILE=path/to/genmakefile   GM_KEEP=1 (keep scratch dir)

set -u

THISDIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
# shellcheck source=./common.sh
. "$THISDIR/common.sh"

GM_MAX_FAILS=1
GM_RUN_ALL=0

while [ $# -gt 0 ]; do
  case "$1" in
    -n | --num)
      GM_MAX_FAILS=$2
      shift 2
      ;;
    --num=*)
      GM_MAX_FAILS=${1#--num=}
      shift
      ;;
    --all)
      GM_RUN_ALL=1
      shift
      ;;
    -h | --help)
      echo "Usage: $0 [-n|--num N] [--all]" >&2
      echo "  -n, --num N   stop after N distinct failures (default 1)" >&2
      echo "  --all         ignore -n/--num, run the whole matrix" >&2
      exit 0
      ;;
    *)
      echo "$0: unknown argument '$1'" >&2
      exit 1
      ;;
  esac
done

GM_MAKE_TYPES="make gmake ninja shell"
GM_PIC_ARGS="-a pic18 -p 18f25k50 --debug"

# number of make_type/partition combinations one compiler's submatrix
# has, used below to tell "stopped early" apart from "really did fail
# every case".
GM_CASES_PER_GROUP=0
for _mt in $GM_MAKE_TYPES; do
  for _p in $GM_PARTITIONS; do
    GM_CASES_PER_GROUP=$((GM_CASES_PER_GROUP + 1))
  done
done

GM_STOP=0

while IFS=: read -r label compiler kind bindir; do
  [ -z "$label" ] && continue
  [ "$GM_STOP" -eq 1 ] && break

  extra_args=""
  [ "$kind" = "pic" ] && extra_args=$GM_PIC_ARGS

  group_ran=0
  group_nonskip=0
  group_fail=0
  group_complete=1

  for make_type in $GM_MAKE_TYPES; do
    for partition in $GM_PARTITIONS; do
      name="$label-$make_type-$partition"
      # shellcheck disable=SC2086
      gm_run_case "$name" "$label" "$kind" "$compiler" "$bindir" "$make_type" "$partition" $extra_args
      group_ran=$((group_ran + 1))
      case "$GM_LAST_OUTCOME" in
        fail)
          group_nonskip=$((group_nonskip + 1))
          group_fail=$((group_fail + 1))
          ;;
        pass) group_nonskip=$((group_nonskip + 1)) ;;
      esac

      if [ "$GM_RUN_ALL" -ne 1 ] && [ "$GM_DISTINCT_FAILS" -ge "$GM_MAX_FAILS" ]; then
        GM_STOP=1
        group_complete=0
        break 2
      fi
    done
  done

  if [ "$group_complete" -eq 1 ] && [ "$group_ran" -eq "$GM_CASES_PER_GROUP" ] &&
    [ "$group_nonskip" -gt 0 ] && [ "$group_fail" -eq "$group_nonskip" ]; then
    GM_REPORT="$GM_REPORT
**Note:** the entire matrix failed for \`$label\` ($group_fail/$group_nonskip non-skipped cases).
"
    printf 'NOTE    entire matrix failed for %s (%s/%s cases)\n' "$label" "$group_fail" "$group_nonskip"
  fi
done <<EOF
$(gm_discover_compilers)
EOF

gm_summary
