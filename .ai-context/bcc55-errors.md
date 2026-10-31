# bcc55 build errors, classified (full `make -i` over src/, 1917 error lines)

Run: genmakefile built from the tree (not /usr/local/bin/genmakefile, which is
older), `-m borland -t bcc55 -I/opt/bcc55/include src/`, Borland make 5.2 with
-i. Log of that run: scratchpad/bcc55-fresh.log

1 = a genmakefile change (the source, or how it is run) can fix it
2 = needs a source change in lib/ (or src/)

## Done in this round

- config.h (E2209, 5 left in the log): lib/io.h was the only file that
  included config.h unconditionally. It is gated with HAVE_CONFIG_H now, like
  the other five files; genmakefile never defines HAVE_CONFIG_H. lib/io.h has
  its own fallback `#define io_seek lseek`, so nothing else is lost. Checked
  with bcc32 on lib/iob/iob_prefetch.c: the config.h error is gone. (2, done)
- "Fatal: bcc55-Release\lib\windoze.lib does not exist": a header alone,
  lib/windoze.h, was taken as a library module, and every program linked the
  phantom library. A module now needs the directory lib/<name>/ as well.
  (1, done, project.c is_module)

## 1. genmakefile

- The "Error: opening bcc55-Release\lib\cb.lib" lines are a consequence: tlib
  is started with no objects because every compile of the module failed. With
  -i make goes on; no change needed once the sources compile.
- Optional, not done: a unit whose scan records a missing include (inc_missing
  is filled already: sys/socket.h, sys/mman.h, sys/uio.h, netinet/*.h,
  unistd.h, alloca.h) cannot be built for this compiler. genmakefile could
  leave that unit out and say so with -v. It hides real problems of portable
  files though, so the gating in lib/ below is the better fix for those that
  should build; for the real POSIX-only modules (io_passfd, iob_send,
  iob_prefetch, iom) leaving them out is right.
- Optional: a program whose libraries have no object is better skipped than
  linked. Same idea as above.
- Nothing in how genmakefile is run helps with the C99 errors: bcc32 -P
  compiles as C++, which the sources are not.

## 2. Source changes in lib/ and src/

A. C99 in the code, Borland 5.5 is C89: `for(size_t i = 0; ...)` and
   declarations after a statement. 46 files, 2 of them headers that are
   included everywhere: lib/hashmap.h:58 (hashmap_fnv_hash, static inline) and
   lib/cpp_internal.h:621/679/687/799. They break every unit of lib/cpp,
   lib/hashmap, lib/ini and every program using them (the error counts of
   those headers are 966 + 184 of the 1917 lines).
   Other files: lib/stralloc (17), lib/utf8 (6), lib/buffer (3),
   lib/tokenizer (2), lib/path (2), lib/http (2), lib/fmt (2), lib/byte (2),
   lib/io/io_waituntil2.c, lib/iom/iom_wait.c (declaration after a statement,
   `fd_type i` in for), lib/sig/sig_dfl.c (designated initializers
   `{.sa_handler = ...}`, and struct sigaction does not exist on Windows),
   lib/utf8/wc_charwidth.c (static array after a statement).
   Fix: declare the variable at the top of the block. Mechanical, but many
   files; start with the two headers.
B. POSIX headers that Borland does not have, 56 occurrences:
   sys/time.h (47, all through lib/cpp.h:14 and lib/io/io_passfd.c:26),
   sys/socket.h, sys/uio.h, sys/mman.h, netinet/in.h, netinet/tcp.h,
   unistd.h, alloca.h. lib/cpp.h should include <time.h> only, or sys/time.h
   behind `#if !defined(__BORLANDC__)`; that one line frees lib/cpp and all
   its programs. io_passfd.c, iob_send.c, iob_prefetch.c are Unix-only code
   (iovec, msghdr, mmap) and need `#if !WINDOWS_NATIVE`-style gating, or an
   emptied file on Windows.
C. Things config.h.cmake would have defined. Only one matters for bcc55:
   HAVE_ALLOCA. lib/path/path_is_directory.c and path_exists.c fall back to
   `wchar_t w[len + 1]` (a VLA, E2313 "Constant expression required"). bcc32
   has `_alloca` in <malloc.h>. A block in lib/windoze.h (it is included
   first by most files) for __BORLANDC__ that includes <malloc.h>, defines
   `alloca` as `_alloca` and HAVE_ALLOCA does it. The other HAVE_* names the
   sources test (HAVE_OPENSSL, HAVE_EPOLL, HAVE_POLL, ...) stay undefined,
   which is what bcc55 needs, so there is no need for a config header.
   Everything else in config.h.cmake (SIZEOF_*, io_seek, HAVE_STDINT_H) is
   not used by a file that fails here.
D. Types of C99 missing from Borland's headers: `uint32_t` (lib/http/
   http_read.c:44 and http_read_internal.c:22), `intptr_t` (lib/unix/
   win32_pipe.c:28). lib/typedefs.h already has a Borland branch (ssize_t,
   uint32); add uint32_t, intptr_t and friends there, as typedefs of the
   uint32/int32 types.
E. Missing constants/APIs of Borland's winsock and shell headers:
   IP_MULTICAST_LOOP (lib/socket/socket_mcloop6.c:19), EAFNOSUPPORT
   (lib/dns/dns_rcip.c:228), FOLDERID_Profile/SHGetKnownFolderPath
   (lib/path/path_gethome.c:24). Gate by `#ifdef` of the constant or
   `#ifndef __BORLANDC__`, or define the value (IP_MULTICAST_LOOP is 11,
   EAFNOSUPPORT 102 in Winsock) in lib/windoze.h.
F. `#warning` (lib/iom/iom_add.c:32, iom_init.c:28, iom_wait.c:107): E2048
   unknown preprocessor directive. Use `#pragma message` or guard the whole
   file; lib/iom is epoll/kqueue code and cannot work on Windows anyway.
G. A module without its library: lib/tls/*.c (tls_instance_t undefined)
   needs HAVE_OPENSSL; without it tls_internal.h declares nothing. Wrap each
   file's body in `#ifdef HAVE_OPENSSL`, or let genmakefile not build the
   module (CMake builds it only when OpenSSL is found).
H. Path macros, 7 files in lib/path (E2342 "Type mismatch in parameter
   'what' (wanted const void*, got int)"): the DRIVE() macro in lib/path/
   internal.h calls str_contains(SEPCHARS ".", (p)[0]) with a char where a
   string is wanted. A real bug in the Windows branch, not only for Borland:
   it should be a byte/char search (str_chr or byte_chr).
I. lib/tokenizer/tokenizer_next.c:150: label `string_handling` undefined
   (E2448); probably a goto into a block that the C89 errors above broke, look
   again after A.
J. src/ftp/ftpcopy.c: the includes are commented out (ftpparse.h, strhash.h,
   getln.h ...), so strhash, struct ftpparse and several helpers are
   undefined (26 errors). It cannot build with any compiler: it is unfinished,
   not a Borland problem.
K. lib/sig/sig_dfl.c is Unix only (struct sigaction); same treatment as B.

## Suggested order

1. lib/cpp.h (B, one line) and lib/hashmap.h + lib/cpp_internal.h (A): unlocks
   lib/cpp, lib/hashmap, lib/ini and their programs.
2. lib/windoze.h alloca block (C) and lib/typedefs.h additions (D).
3. The remaining lib/stralloc, utf8, buffer, ... for loops (A).
4. Gating of the POSIX-only files (B, E, F, G, K).
5. H on its own: it is a bug on every Windows compiler.

## Status 2026-10-31

Full `make -i` over src/ (bcc55): 555 error lines, 42 programs link, 68
libraries have members. With bcc102 (clang, C99): 224 error lines, 50
programs link. Libraries compile with both.

Fixed since the first round: tlib members (quoted +"obj", and -+ so a rebuilt
object replaces the old one), library dependencies (iarray through io.h, hmap
through map.h), isnan/isinf (lib/fmt/fmt_double.c), import libraries on the
command line (-lshlwapi ...), mkdir rules, the optional packages.

Left: the Unix-only programs (src/ftp, dnsutils, serial, tcp, intercept,
system, filedata/toolchain pieces), src/genmakefile itself (unistd.h),
_lseek64, _isblank, _GetNativeSystemInfo, _die/_diesys, _sockaddr_size,
_tls_* (needs OpenSSL), _buffer_lzma/_buffer_bz2 (no libraries unless LIBLZMA
and LIBBZ2 are given).

