# genmakefile idea: #pragma comment(lib, ...) / #pragma lib

Win32 compilers can name import libraries from the source, so the linker
needs no -l flags:
- MSVC, clang-cl, Pelles: `#pragma comment(lib, "shlwapi.lib")`
- Borland C++ 5.5: no comment(lib); use `#pragma link "shlwapi.lib"`
  (check; otherwise only the response file / -l works)
- mingw gcc ignores both, so -l flags are still needed there.

Two parts:
1. Sources (or lib/windoze.h, guarded by the compiler macro) carry the
   pragma where a Win32 API is used, e.g. path_exists.c ->
   shlwapi (PathFileExistsW), tls/socket code -> ws2_32, iphlpapi.
2. genmakefile reads the pragmas during the lib/cpp scan (they are
   already tokenized as `#pragma` lines) and collects the lib names per
   unit, merged like the D/H/L lines (new line type, e.g. "P<name>").
   Use: add them to LIBS of the program linking that unit when the
   compiler ignores the pragma (mingw), or just report them with -v.
   Also useful when porting a foreign project: genmakefile shows which
   system libs the sources want.

Related: bcc55 import libs live in /opt/bcc55/lib/psdk (shlwapi, ws2_32,
iphlpapi, advapi32, ole32, shell32, user32, mswsock).

Checked 2026-10-31: bcc32c (Embarcadero 10.2, clang) takes
`#pragma comment(lib, "x.lib")` like MSVC; bcc55 and bcc56 have no such
pragma. For bcc55 the import libraries still go on the command line
(-L/opt/bcc55/lib/psdk -lshlwapi -lws2_32 ...).

