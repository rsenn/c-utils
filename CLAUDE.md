# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A large collection of C utility libraries (`lib/*`) modeled on libowfat (djb-style I/O, string, and buffer primitives), plus dozens of standalone CLI tools built on top of them (compression cat, binary format listers (ELF/COFF/OMF/PE/Mach-O), a C preprocessor, HTTP/DNS/TLS clients, XML/JSON tools, Windows registry converters, etc.). See `README.txt` for the full module/tool index — it's kept up to date and is the fastest way to find what a given `lib/<name>.h` or top-level `*.c` tool actually does.

## Build

CMake, out-of-tree. Existing build dirs in `build/` (e.g. `build/x86_64-linux-gnu`, `build/x86_64-linux-debug`, `build/x86_64-linux-profile`, `build/aarch64-linux-android`) are already configured — reuse them rather than creating new ones unless targeting a new toolchain:

```sh
cd build/x86_64-linux-gnu
cmake --build . --target <name>   # build one library or program
cmake --build .                   # build everything (slow)
```

If you add/remove/rename a source file under `lib/<module>/`, CMake's globbed file list goes stale — rerun `cmake .` in the build dir before rebuilding, otherwise you'll get "No such file or directory" for the old filename or the new file won't compile in.

Tests are off by default (`BUILD_TESTS` option). To enable, reconfigure with `-DBUILD_TESTS=ON` and build the `all_tests` target (see `tests/CMakeLists.txt`); it globs `tests/test_*.c` into one binary.

## Architecture

**Module system**: every `lib/<name>.h` public header has a matching `lib/<name>/` directory containing one `.c` file per function (e.g. `lib/hashmap.h` + `lib/hashmap/hashmap_get.c`, `hashmap_put.c`, ...). CMake globs each `lib/<name>/*.c` directory into its own static library target named `<name>` (see `lib/CMakeLists.txt`'s `MODSUBDIRS` list). A CLI tool declares its dependencies with the `program(name lib1 lib2 ...)` CMake macro (`cmake/Functions.cmake`), which builds `name.c` and links the named module libraries. When adding a function to a module, follow the one-function-per-file convention rather than appending to an existing file, so it participates correctly in the glob.

**cpp module** (`lib/cpp.h`, `lib/cpp_internal.h`, `lib/cpp/*.c`): a standalone C preprocessor (chibicc-derived). All mutable preprocessing state (macro table, include-guard tracking, tokenizer position, const-expr scope, `__COUNTER__`, etc.) lives in a caller-owned `cpp_ctx` struct — there is no global or thread-local state anywhere in this module. Every `cpp_*` function that needs it takes `cpp_ctx* pp` as its first argument. `cpp_init(cpp_ctx*)` initializes a struct the caller already owns (stack, static, or heap) in place and allocates nothing; `cpp_free(cpp_ctx*)` releases only the internal resources cpp_init/the API came to own, never `*pp` itself. When adding new preprocessor state, add a field to `struct cpp_ctx` in `cpp_internal.h` and thread `pp` to it explicitly — do not reintroduce a global/static variable or an implicit "current context" mechanism.

**socket-intercept.c**: an `LD_PRELOAD` shim that intercepts libc socket/TLS calls per-thread (`thread_local` state throughout) to log traffic; not linked into the module system above.

## Testing

Do not run `tests/genmakefile/testsuite.sh` on your own initiative -- only when the user explicitly
asks for it (it's slow, exercises real toolchains/build tools, and produces large scratch trees).

## TODO and BUGS files

`TODO` and `BUGS` are plain text files in the repo root — the roadmap and known bugs,
respectively. No title header, no numbering: every entry is a top-level `- ` bullet whose text
starts with a `kebab-case-slug-title:` naming the bug/item, followed by a description. Wrapped
continuation lines are indented two spaces to line up under the bullet; a blank-line-separated
indented block inside an entry is a code/repro snippet. Use `` `backticks` `` for every file,
function, and symbol name mentioned. Prefer `--` over an em dash. Match the style already in the
files (and see `../shish/BUGS` for a reference example from a sibling project). Check them for
planned work or known issues; add entries the same way when asked to note something down.

This repo keeps these as a running log, not a one-off report. Whenever you diagnose a bug, fix a
bug, or find follow-up work while doing something else:

- Update `BUGS` immediately when a bug is found or fixed. A newly discovered bug becomes a new
  entry at the end of the file. Once a bug is verified fixed, **delete its entry** rather than
  marking it — a fixed bug isn't a known bug anymore, and the fix itself belongs in the commit
  message/git history, not in this file. Don't wait to be asked.
- Update `TODO` immediately when you notice follow-up work, a deferred fix, or a known limitation
  that isn't being addressed in the current task. Delete entries once done, same rule as above.
- Do this automatically, without being asked each time, as part of normal work in this
  repository — these files are how work carries over between sessions.
