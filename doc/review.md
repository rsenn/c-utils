# Review and assessment of c-utils

Written on 2026-11-01 during a long working session on the tree
(branch `claude/gifted-galileo-ryrk08`). It covers every directory under
`lib/` and every program under `src/`, says what each one is for, how
healthy it is and what I would do with it, infers what the author is
building, and proposes where the project can go.

Nothing here is guessed from names alone where a measurement was possible;
each module carries a depth mark (see "Library modules") so that you can
tell a thing I tested from a thing I only read.

## 1. Method and evidence

| what | how | result |
| --- | --- | --- |
| build | full CMake build of 143 targets | clean; 6 warnings with the project's own flags |
| stricter warnings | `gcc -Wall -Wextra` on all 1,462 library files | 317 warnings: 155 `-Wswitch` (almost all in `cpp`), 53 unused variables, 35 `-Warray-parameter`, 10 missing `return` (one macro in `map.h`, repeated per file), 9 function-pointer casts, 4 comparisons of unsigned with 0 that were real bugs |
| static analysis | clang static analyzer + bugprone checks on `lib/` | about 45 findings, mostly dead stores; real ones: null-pointer paths in `cpp_hideset`/`cpp_token` (not confirmed), a division by zero in `hashmap_rehash` (fixed), a leak in `hmap_init` (fixed) |
| dynamic | valgrind over the whole unit test suite | one real invalid read (`cbmap_insert`, fixed); none left |
| complexity | micro-benchmarks at n and 4n for 28 operations, plus tool timings | three quadratic or worse operations found and fixed (below); the rest linear |
| robustness | every tool with empty, random, truncated, corrupt and missing input | about twenty tools crashed or hung; all fixed (below) |
| declarations | every function declared in a header against the sources | about 4 % declared and never defined (`linked_list.h` and `tree.h` entirely) |
| tests | unit test suite | 548 at the start of the session, 641 at the end |

### What the session found, in order of cost

1. **JSON and XML were unusable on real data.** Parsing a 20,000 element JSON
   array took 1.3 s (quadratic: a character source measured the rest of the
   string for every byte). Parsing 20,000 keys took 9.4 s, and 80,000 keys
   177 s, because the map under JSON objects (`hmap`) had a fixed table of
   1024 buckets that never grew. Each JSON object reserved 1.1 MB, so a
   200 KB document of small objects needed more address space than 8 GB. XML
   reserved 53 KB per element. All linear now (80,000 keys: 0.12 s).
2. **`hmap` copied keys into a 1023-byte buffer without checking.** A longer
   key corrupted its neighbours silently, because the table was one huge
   allocation. Replaced.
3. **`set` could not be trusted.** `set_delete` freed a pointer into its own
   table and crashed on a missing value, `set_clear` leaked every value,
   `set_at` always crashed, `set_has` on a fresh set divided by zero. It is
   the main container of `genmakefile`. Redesigned (section 5).
4. **Memory-unsafe file parsers.** Nine binary-format and XML tools crashed on
   a missing file; the ELF/PE listers crashed on any corrupt file because the
   library functions take a bare pointer with no size. Format validators
   (`elf_check`, `pe_check`) now stand in front of them.
5. **Silent data corruption in compressors.** `buffer_lzma`'s close wrote each
   round of output over the previous one, so every `.xz`/`.lzma` larger than
   its buffer was damaged; on an error it looped for ever.
6. **A list of smaller defects**: `utf8` (search functions that loop on an
   invalid byte, checks on an unsigned value that could never fire),
   `slist`/`dlist` (endless and crashing finds, a macro that never stops),
   `cbmap` (overwrote another key's value), `scan_double` (no signed
   exponents), `json` (stack overflow on deep nesting), `ini` (crash on a
   file without sections), `cpp` (endless loop on non-UTF-8 bytes).
7. **The tree did not build**: `io_sendfile.c` lacked a closing brace.

## 2. What the author is building

The commit log, the README, `CLAUDE.md` and the shape of the code agree on
one intention: **a self-contained C workbench for people who build and take
apart software on unusual machines**, assembled in layers.

* **Layer 1: primitives in the manner of djb and fefe's libowfat.** Small
  functions, one per file, explicit lengths, no hidden globals, no `<stdio.h>`
  or `<string.h>` in programs. The aim is not purity but *portability by
  construction*: the same few primitives are implemented over POSIX,
  Winsock, Borland, musl and Android, so every tool built on them comes along.
* **Layer 2: formats and protocols.** JSON, XML, INI, Intel HEX, playlists;
  ELF, PE, COFF, OMF, Mach-O; DNS, HTTP, FTP, TLS. The binary-format and PIC
  pieces show the author reverse engineers, cross-compiles to Windows from
  Linux (`ntldd`, `impgen`, `pemkwingui`, `reg2cmd`, the msys tools) and
  works with Microchip toolchains and EAGLE board files.
* **Layer 3: a toolchain around it.** `pkgcfg`, `cc-wrap`, `ar-wrap`,
  `exe-wrap`, `cmake-run`, `pathtool`, `cpre` and above all `genmakefile`.
  The C preprocessor in `lib/cpp` exists so that `genmakefile` can answer
  "which headers does this file really include *on this target*" the way the
  compiler would. This is the keystone: **the library is, to a large degree,
  the standard library of `genmakefile`.** `set`, `strlist`, `path`, `cpp`,
  `var`, `uri` and `buffer` are shaped by its needs.
* **A maintenance habit built on AI assistance.** `CLAUDE.md`, `.ai-context/`,
  the ChangeLog/BUGS/TODO discipline and the handoff documents show a
  workflow in which sessions continue each other's work. This is a strength
  (the BUGS file is an honest log) and a risk (see "documentation drift").

The history before 2026-10-15 is not in the repository (52 commits, the
oldest from that day), so the earlier life of the code (libowfat, shish,
chibicc, djbdns imports) is visible only in file headers and style.

## 3. Overall judgement

*Strong parts*: the buffer/filter family, `path`, `cpp`'s context-struct
architecture, the I/O loop, the discipline of the notes, the breadth of
working tools (72 programs in the CMake file; all of them start, and since
the fixes below none crashes on empty, random or missing input). The code is consistently formatted and mostly readable.

*Weak parts*: containers (six overlapping hash/tree maps, two list families,
two dead headers), validation of untrusted binary input, the TLS client
(no certificate check), test coverage of about a quarter of the modules'
functions, documentation that drifted from the code (several man pages
described functions that do not exist), and a build story with four
descriptions (CMake, `configure.ac`, `Makefile.genmakefile`, shell scripts)
that are not generated from one source.

The healthy pattern to copy is what happened to `path` (vendored from the
sibling project, migrated in one sweep, tested, documented). The unhealthy
pattern is code imported once and never reconciled (`hmap`, `ftpcopy`, `redir`,
`bmap`, `linked_list`).

## Library modules

Depth: **F** fixed and tested in this session, **T** tests written in this session, **M** measured (warnings, analyzer, benchmark, fuzz) , **S** structure and headers read only.
### `lib/alloc` [S]

Thin djb `alloc`/`alloc_zero`/`alloc_free` over malloc.

*4 files, 105 lines, 4 man pages, tests 75 lines, no warnings.*

Allocation failure policy is per caller; `alloc_re` has a DJB_STRICT mode that is never on. Fine as a seam; the real value is that one switch could add tracking or arenas later.
### `lib/array` [M]

libowfat `array`: byte vector with `array_allocate`. 20 functions, tested (347 lines).

*20 files, 362 lines, 20 man pages, tests 347 lines, no warnings.*

Growth was a fixed 4 KB step (quadratic for big vectors): now geometric. `array_shift`/`array_chop` are declared and never defined (BUGS).
### `lib/binfmt` [S]

One file, `binfmt_open`, that was meant to sniff ELF/PE/COFF/OMF/Mach-O and give one handle.

*1 files, 12 lines, 1 man pages, tests 26 lines, no warnings.*

It detects nothing (BUGS). This is the natural front door for the format modules and the place to put size-aware views (see proposals).
### `lib/bmap` [S]

rxi/map-style typed hash map generated by macros (`bmap_t(T)`).

*7 files, 202 lines, 7 man pages, no test file, no warnings.*

No tests; `bugprone-sizeof-expression` hits in `bmap_set`. One of six hash containers (hashmap, hmap, set, cbmap, cb, bmap); nothing in the tree uses it. Candidate for removal.
### `lib/buffer` [M]

The heart of the I/O model: 101 files, buffered read/write over any `op`, plus filters (gzip, deflate, bz2, lzma, brotli, zlib), mmap buffers, chunked coding, line and token readers. Tested (1233 lines).

*101 files, 2785 lines, 102 man pages, tests 1233 lines, 1 warnings.*

Fixed: failed `buffer_mmap*` left an uninitialized buffer that eight tools then read (now an empty valid buffer); `buffer_lzma` close loop overwrote its own output and spun on errors. Still open: `buffer_flush` forgets what a non-blocking socket refused (BUGS), `buffer_mmapshared` unimplemented. Great shape otherwise; the filter family is the project's most reusable idea.
### `lib/byte` [M]

35 byte-range primitives. Tested.

*35 files, 570 lines, 35 man pages, tests 303 lines, no warnings.*

Were byte loops (slow at -O0); `byte_copy/zero/fill/chr` now use libc where the result is identical. `byte_diff` keeps its own loop (its result is a difference, not just a sign).
### `lib/case` [S]

Case-insensitive str/byte comparisons. Tested.

*9 files, 161 lines, 9 man pages, tests 114 lines, no warnings.*

Small and right.
### `lib/cb` [T]

Critbit tree (Enno Rehling): insert/find/erase/prefix/foreach, plus a key+value helper. 9 new tests.

*10 files, 329 lines, 10 man pages, tests 283 lines, no warnings.*

Correct as tested. `cb_find` is prefix-tolerant by design (`len >= keylen`): exact lookups need the NUL-including `_str` macros. Document that at the call site.
### `lib/cbmap` [F]

Critbit map with copy or static keys and pluggable allocators. 8 new tests.

*12 files, 709 lines, 12 man pages, tests 301 lines, 1 warnings.*

Fixed: inserting a key that is the start of another key read past the stored key and could overwrite the other key's value (found by valgrind). Keys differing only by trailing zero bytes are now refused (documented).
### `lib/charbuf` [F]

Lookahead character window for hand-written parsers (JSON reader, tokenizer).

*21 files, 568 lines, 21 man pages, no test file, 1 warnings.*

Fixed: `charbuf_froms` measured the whole rest of the string for every byte, so reading JSON from a string was quadratic (1.3 s for 20 000 elements, now 11 ms); `charbuf_pred_lookahead` compared a pointer with 0. Reads one byte per call through a function pointer: fine, not fast. No test file of its own yet.
### `lib/coff` [S]

COFF structures and a few accessors (Microchip and Windows object files).

*10 files, 181 lines, 10 man pages, no test file, no warnings.*

`cofflist` is the only user. No validation; the tool now only checks the size of the file header. Needs a `coff_check` like ELF and PE.
### `lib/cpp` [M]

The C preprocessor (chibicc-derived), with all state in a caller-owned `cpp_ctx`: no globals. 48 files, 4.6 k lines. This is the module that makes `genmakefile` correct about `#if` gates.

*48 files, 4651 lines, 46 man pages, no test file, 148 warnings.*

Architecture is exactly right. Weak points: 148 `-Wswitch` warnings (enum cases not handled), analyzer-reported null dereferences in `cpp_hideset`/`cpp_token` (unconfirmed), no unit tests at all (only exercised through `cpre`/`genmakefile`). Fixed: an endless loop on bytes that are not UTF-8 in an identifier (`cpre` on random data never returned).
### `lib/debug` [S]

Tiny dump helpers.

*9 files, 96 lines, 0 man pages, no test file, 1 warnings.*

No man pages, a counter bug in `debug_sl` (BUGS). Keep or fold into `buffer`.
### `lib/dir` [S]

`dir_open/read/close`, name/type/size/time. Tested.

*8 files, 422 lines, 8 man pages, tests 174 lines, 1 warnings.*

`dir_size` only works on Windows (BUGS).
### `lib/dlist` [F]

Intrusive doubly linked list. 7 new tests.

*12 files, 287 lines, 12 man pages, tests 299 lines, no warnings.*

Fixed: `dlist_swap` broke neighbours, `dlist_foreach_down_safe` did not compile; find/swap/move are now declared.
### `lib/dns` [M]

djbdns client library (24 files, 2.6 k lines): transmit, packet, domain names, resolv.conf, IDN.

*24 files, 2636 lines, 31 man pages, no test file, 15 warnings.*

A faithful port and the base of ten tools. No tests (pure packet functions would be easy to test with known vectors). `dnsq`, `dnsqr`, `dnstrace` do not compile (BUGS). Blocking by design.
### `lib/elf` [F]

ELF structures (32/64) and 22 accessors.

*23 files, 474 lines, 22 man pages, tests 216 lines, no warnings.*

All accessors take a base pointer and no size, so they cannot be made safe one by one. Added `elf_check(base,size)`: header, tables, sections, string tables, symbol names, dynamic entries (11 tests). Every lister now calls it first.
### `lib/env` [S]

djb `env` plus portable getenv/setenv. Tested.

*12 files, 525 lines, 12 man pages, tests 162 lines, no warnings.*

A BUGS entry about unset/init bookkeeping is marked fixed.
### `lib/errmsg` [S]

fefe `errmsg_warn*/die*`. Tested.

*8 files, 151 lines, 8 man pages, tests 194 lines, no warnings.*

Variadic with a required 0 terminator: `omflist` forgot it and crashed. A macro adding the terminator would remove the class of bug.
### `lib/fmt` [S]

46 formatters (numbers, escapes, base64, ASN.1). Tested (585 lines).

*46 files, 1303 lines, 46 man pages, tests 585 lines, 2 warnings.*

Healthy; several functions repaired in earlier commits.
### `lib/gpio` [S]

Raspberry Pi GPIO through /dev/mem.

*10 files, 153 lines, 10 man pages, no test file, no warnings.*

Untestable without hardware; fine as a leaf.
### `lib/hashmap` [M]

Open-addressing hash map (FNV, tombstones) from chibicc; used by `ini` and, via `map.h`, the tools.

*10 files, 174 lines, 10 man pages, tests 251 lines, no warnings.*

Tested. No insertion order. Same job as `hmap`/`set`: see proposals.
### `lib/hmap` [F]

Insertion-ordered map with typed values (int, double, chars, custom): the engine under JSON objects, XML attributes and several tools.

*18 files, 437 lines, 14 man pages, no test file, no warnings.*

Rewritten. Was 1104 bytes per entry with a fixed 1023-byte inline key that was copied unchecked (silent corruption), 1024 entries reserved per map (a JSON object cost 1.1 MB of address space; 20 000 small objects could not be read), a table that never grew (80 000 keys: 177 s), an O(n) `hmap_size`. Now: entries allocated one by one with the key behind them, table made on first insert and doubled, O(1) everything. 11 tests.
### `lib/http` [F]

HTTP/1.x client for the io loop, with TLS, any method, headers, chunked request bodies and decoded streamed responses. See `doc/http.md`.

*34 files, 2157 lines, 36 man pages, tests 242 lines, 7 warnings.*

Extended this session (see ChangeLog). Known: no certificate verification, one request per connection.
### `lib/iarray` [S]

libowfat `iarray` (O(1) fd-indexed arrays). Tested.

*5 files, 146 lines, 5 man pages, tests 81 lines, no warnings.*

Right.
### `lib/ihex` [S]

Intel HEX reader/writer/verify. Tested (128 lines).

*21 files, 530 lines, 15 man pages, tests 128 lines, 6 warnings.*

Six unused-variable warnings; the base of `ihextool`.
### `lib/ini` [F]

INI sections over `hashmap`.

*10 files, 283 lines, 0 man pages, tests 91 lines, 21 warnings.*

21 warnings were one missing `return` in `map.h`'s `hashmap_insert` (fixed). `ini_get`, `ini_section`, `ini_keys` crashed for a file that had no sections (that is how `exe-wrap` died); now NULL-safe.
### `lib/io` [M]

libowfat I/O multiplexer: epoll/poll/kqueue/select back ends, timeouts, sendfile, fd passing. 50 files, 3.7 k lines. Tested (294).

*50 files, 3671 lines, 50 man pages, tests 294 lines, 8 warnings.*

The Linux `io_sendfile` did not compile (a missing brace: the whole tree failed to build until fixed). `io_sendfile` is Linux-only in practice. The reason the HTTP client is not blocking.
### `lib/iob` [S]

libowfat `iob`: batched writes with files.

*15 files, 614 lines, 15 man pages, tests 128 lines, no warnings.*

Tested lightly.
### `lib/iom` [S]

Threaded io variant.

*4 files, 261 lines, 4 man pages, tests 52 lines, 3 warnings.*

epoll/kqueue are never enabled (BUGS).
### `lib/json` [F]

Lenient tree reader + printer, and new: strict `json_valid`/`json_parse_text`, `json_property_path`, NDJSON feed.

*40 files, 1395 lines, 40 man pages, tests 271 lines, 9 warnings.*

Strings now hold the real text (the old store kept `\u` escapes and could not tell an escaped backslash from an escape). Parsing was quadratic twice over (charbuf, hmap); now linear. Depth limited to 512 (a deep document overflowed the stack). Leaks in the reader fixed. The old reader still accepts garbage silently: keep it only for tools that want leniency.
### `lib/mmap` [S]

mmap_read/private/shared and unmap. Tested.

*12 files, 550 lines, 12 man pages, tests 201 lines, 3 warnings.*

`mmap_anon` unimplemented (BUGS). Failure returns NULL: callers must look.
### `lib/ndelay` [S]

Non-blocking toggles. Tested.

*2 files, 44 lines, 2 man pages, tests 33 lines, no warnings.*

Right.
### `lib/omf` [S]

Borland OMF record reader.

*11 files, 321 lines, 11 man pages, no test file, no warnings.*

No tests, no validation; `omflist` now reports a missing file instead of crashing.
### `lib/open` [S]

open_read/trunc/append/... Tested.

*8 files, 341 lines, 8 man pages, tests 163 lines, no warnings.*

Right.
### `lib/path` [M]

Lexical path functions over (pointer,length) views, vendored from the sibling project shish: collapse, canonicalize, relative, fnmatch, basename/extension, lookup.

*36 files, 1635 lines, 13 man pages, tests 312 lines, 32 warnings.*

36 functions, 312 test lines, a recent migration of all clients. 32 warnings (`-Wswitch`). The best-factored module in the tree; the model for how the others should look.
### `lib/pe` [F]

PE/COFF image structures and 20 accessors.

*12 files, 440 lines, 11 man pages, tests 192 lines, no warnings.*

Same story as ELF: added `pe_check(base,size)` (DOS/NT headers, optional header, sections, export names, import tables, thunks; 5 tests). Wired into `pelist`, `ntldd`, `impgen`, `pemkwingui`.
### `lib/playlist` [S]

M3U/PLS/XSPF reader and writer.

*8 files, 384 lines, 8 man pages, no test file, no warnings.*

No tests; XSPF goes through `xml`.
### `lib/process` [S]

Spawn and wait for processes (Windows and POSIX).

*3 files, 243 lines, 3 man pages, no test file, 2 warnings.*

No tests; `process_executable` depended on the broken `wcs_to_u8sa` (fixed).
### `lib/range` [T]

`{start,end,elem_size}` views and rotate. 4 new tests.

*2 files, 37 lines, 2 man pages, tests 110 lines, 1 warnings.*

Correct as tested.
### `lib/rdir` [S]

Recursive directory walk. Tested.

*3 files, 128 lines, 3 man pages, tests 54 lines, no warnings.*

Right.
### `lib/safemult` [S]

Overflow-checked multiplication. Tested.

*10 files, 346 lines, 17 man pages, tests 60 lines, no warnings.*

Right.
### `lib/scan` [M]

48 parsers (numbers, escapes, base64, dates). Tested (288).

*48 files, 1511 lines, 47 man pages, tests 288 lines, no warnings.*

`scan_double` rejected every signed exponent (`1e-5`, `2.5E+3`): fixed.
### `lib/seek` [S]

Portable lseek/fseek helpers. Tested.

*6 files, 128 lines, 6 man pages, tests 89 lines, no warnings.*

Right.
### `lib/set` [F]

Insertion-ordered set of byte strings. Redesigned, 15 tests; see ChangeLog.

*26 files, 569 lines, 20 man pages, tests 575 lines, 4 warnings.*

It is used by `genmakefile` for everything; the rewrite changed its output only in the order of rules (now deterministic).
### `lib/sig` [S]

djb-style signal helpers with a handler stack. Documented in `doc/sig.md`.

*19 files, 431 lines, 20 man pages, no test file, no warnings.*

No tests (they need real signals: `raise` plus `sig_push/pop` would do). The TODO about a self-pipe is the right next step.
### `lib/slist` [F]

Intrusive singly linked list. 9 new tests.

*15 files, 218 lines, 15 man pages, tests 292 lines, no warnings.*

Fixed: `slist_find` never advanced (endless loop when nothing matched), `slist_find_delete` crashed when nothing matched, both `*_foreach_safe` macros never ended, `slist.h` declared `dlist_find`. Appending is O(n) unless the caller chains the returned tail pointer (documented).
### `lib/socket` [M]

libowfat sockets (68 files): TCP/UDP, v4/v6, Unix, Winsock shims. Tested (110).

*68 files, 2396 lines, 68 man pages, tests 110 lines, 23 warnings.*

Wide platform surface; the shims are where portability lives.
### `lib/sse` [T]

Server-sent events parser (new). 3 tests.

*3 files, 138 lines, 4 man pages, tests 75 lines, no warnings.*

Complete for the protocol.
### `lib/str` [M]

38 string primitives. Tested.

*38 files, 885 lines, 38 man pages, tests 340 lines, no warnings.*

`str_len`/`str_chr` now use libc. `str_str`, `str_decamelize` declared and missing; `str_triml` inverted; `str_istr` first-char bug (BUGS).
### `lib/stralloc` [M]

112 files: the growable string, the most used type. Tested (385).

*112 files, 1881 lines, 112 man pages, tests 385 lines, 10 warnings.*

Growth is 12.5 % + 30 (djb), amortized O(1). `stralloc_ready` zeroes fresh memory. A weak (borrowed) stralloc exists and bit once (BUGS, fixed).
### `lib/strarray` [S]

Array of `char*`. Tested lightly (49 lines for 36 files).

*36 files, 649 lines, 36 man pages, tests 49 lines, no warnings.*

Under-tested for its size.
### `lib/strlist` [M]

Separator-delimited list inside one stralloc. 63 functions, tested (550).

*63 files, 1274 lines, 63 man pages, tests 550 lines, 4 warnings.*

Compact and cache-friendly, but `strlist_at`, `strlist_count`, `strlist_push_unique`, `strlist_contains` are O(n) by construction: a loop over them is O(n²). Measured: `strlist_count` in a loop, 16 000 items 28 ms, 64 000 items 473 ms. Fine for command lines, wrong for big data.
### `lib/tai` [S]

TAI64. Tested.

*6 files, 72 lines, 6 man pages, tests 98 lines, no warnings.*

Right.
### `lib/taia` [S]

TAI64N. Tested.

*12 files, 229 lines, 12 man pages, tests 149 lines, no warnings.*

`taia_fmtfrac` unimplemented (BUGS).
### `lib/terminal` [S]

VT/console output through function calls, key input; used by `least`.

*32 files, 897 lines, 0 man pages, no test file, no warnings.*

No tests (a pty test is possible).
### `lib/textbuf` [S]

Line-indexed text buffer for the editor-like tools.

*6 files, 68 lines, 6 man pages, no test file, no warnings.*

Tiny, untested.
### `lib/textcode` [S]

35 codecs (base64, quoted-printable, C/JSON/HTML escapes, URL, uuencode). Tested lightly (46 lines).

*35 files, 1643 lines, 36 man pages, tests 46 lines, 1 warnings.*

`fmt_base85`/`scan_base85` declared and missing. The widest function list with the thinnest test file: next in line.
### `lib/tls` [S]

OpenSSL/LibreSSL wrapper per fd. 27 files.

*27 files, 1164 lines, 27 man pages, no test file, no warnings.*

No tests, no certificate verification (BUGS), no SNI until this session (`tls_servername`). The weakest security spot in the project.
### `lib/tokenizer` [S]

C-like tokenizer. Tested (113).

*17 files, 416 lines, 17 man pages, tests 113 lines, no warnings.*

Fixed earlier (hex literals).
### `lib/ucs` [S]

UCS/Latin-1/Punycode/IDNA conversion. 15 functions, 7 man pages.

*15 files, 499 lines, 7 man pages, no test file, no warnings.*

No tests; overlaps `utf8`.
### `lib/uint16` [S]

Pack/unpack. Tested.

*6 files, 50 lines, 6 man pages, tests 52 lines, no warnings.*

Right.
### `lib/uint32` [S]

Pack/unpack. Tested.

*9 files, 166 lines, 9 man pages, tests 85 lines, no warnings.*

Right.
### `lib/uint64` [S]

Pack/unpack. Tested.

*6 files, 49 lines, 6 man pages, tests 57 lines, no warnings.*

Right.
### `lib/unix` [M]

The porting layer: glob, fnmatch, getopt_long, Windows `popen`/`loadprog`/`readlink` shims. 23 files, 4.1 k lines, 16 uses of strcpy-class calls.

*23 files, 4114 lines, 23 man pages, tests 93 lines, 1 warnings.*

Large for a "shim" module and the least libowfat-like part of the library; this is where the Windows story is kept. Candidate for being split per platform.
### `lib/uri` [S]

URI scanner/formatter. Tested.

*9 files, 263 lines, 0 man pages, tests 129 lines, no warnings.*

A relative path is taken as a host (BUGS).
### `lib/utf8` [F]

UTF-8 <-> wide conversion and searching. 18 new tests.

*18 files, 647 lines, 18 man pages, tests 402 lines, 4 warnings.*

Rewritten for strictness: invalid sequences, overlong forms and surrogates are errors that callers can see (the old `size_t` "< 0" checks could never fire); `*chrs` looped for ever on a bad byte; `wcs_to_u8sa` appended nothing and was never declared.
### `lib/wait` [S]

wait/waitpid wrappers. Tested.

*10 files, 310 lines, 10 man pages, tests 97 lines, no warnings.*

Right.
### `lib/xml` [F]

Event reader and tree builder for XML/XHTML with attributes, comments, CDATA; printer; finders. Tested (180).

*53 files, 1810 lines, 53 man pages, tests 180 lines, 8 warnings.*

Quadratic on wide documents (53 KB table per element): now linear (20 000 siblings: 260 ms to 13 ms). `xml_print(NULL)` and finders on NULL are now harmless.

## Utilities

| path | what it does | state |
| --- | --- | --- |
| `src/binfmt/elflist` | ELF32 lister | robust on corrupt input after `elf_check` |
| `src/binfmt/elf64list` | ELF64 lister | was crashing on any non-ELF/missing file; fixed |
| `src/binfmt/elfwrsec` | rewrite a section of an ELF in place | modifies the file; now validates first |
| `src/binfmt/pelist` | PE sections/imports/exports | robust after `pe_check` |
| `src/binfmt/pemkwingui` | set the PE subsystem to GUI | modifies the file; now validates first |
| `src/binfmt/cofflist` | COFF symbols (Microchip too) | size check only; no `coff_check` yet |
| `src/binfmt/omflist` | OMF symbols | missing varargs terminator crashed it; fixed |
| `src/binfmt/macho32list` | Mach-O 32 segments/symbols | bounds on load commands added; still uses stdio |
| `src/binfmt/ntldd` | `ldd` for Windows binaries | follows imports through `pe_check` |
| `src/binfmt/impgen` | make a .DEF from a DLL | validated |
| `src/dirlist/list-r` | recursive listing, 1.6 k lines | largest tool without tests |
| `src/dirlist/decode-ls-lR, count-depth, dump` | `ls -lR` decoding helpers | fine |
| `src/dnsutils/dnsip, dnsip6, dnsipq, dnsip6q, dnsmx, dnsname, dnstxt` | djbdns-style resolvers | tiny frontends over `dns`; consistent usage text |
| `src/dnsutils/dnsqr, nscache` | recursive query, caching resolver | compile and run; `dnsq`, `dnstrace` do not (BUGS) |
| `src/eagle/*` | EAGLE CAD XML: commands, board init, SVG, circuit export | hard-coded personal default paths in `eagle-to-svg`; crashed on missing files; fixed; otherwise scratch-quality |
| `src/filedata/ihextool` | Intel HEX swiss knife | good: usage, subcommands |
| `src/filedata/ccat (czip)` | compressed cat/compress | xz/lzma close fixed (corrupt output and endless loop) |
| `src/filedata/crc, bsdiffcat, hexedit, piccfg, findsig, torrent-progress` | binary utilities | `torrent-progress` has format warnings; `crc` needs a file |
| `src/ftp/*` | ftpcopy/ftpls (imported) | do not compile (BUGS); superseded by `cftpcat` |
| `src/genmakefile/*` | build generator, 16 files, 10 k lines | the keystone; see the spec |
| `src/intercept/*` | `LD_PRELOAD` shims for exec and sockets | intentionally outside the module system; unconditional logging (BUGS) |
| `src/media/mediathek-list, mediathek-parser, plsconv` | MediathekView list tools, playlist converter | `mediathek-list` is the only user of the first HTTP reader |
| `src/serial/logserial, montty, serial` | serial port logger and monitor | `montty` still uses stdio (BUGS) |
| `src/system/lsproc, pipeinfo, setuidgid, sln, aio_passwd, try-llseek` | small system tools | `aio_passwd`, `try-llseek` are scratch |
| `src/tcp/chttpcat, ctlscat, cftpcat, tcping` | curl-like, TLS netcat, FTP, TCP ping | good usage and error reporting |
| `src/tcp/copenai, cgemini, llmstream` | streaming LLM clients (new) | see `doc/http.md` |
| `src/tcp/httpproxy, tcpproxy, tinyproxy, redir` | proxies and redirectors | `redir` does not compile (BUGS); `tinyproxy` 1.4 k lines |
| `src/text/least, parse, q, strip-comments` | pager, playlist/format parser, query tool, comment stripper | `least` is a real program (1.2 k lines) built on `terminal` |
| `src/toolchain/pkgcfg` | pkg-config replacement, 1.6 k lines | one of the more finished tools |
| `src/toolchain/ar-wrap, cc-wrap, exe-wrap, compiler-wrapper, cmake-run, msys-shell, pathtool, cpre` | compiler/archiver/exe wrappers, cross environment finder, realpath, C preprocessor | `exe-wrap` crashed without an ini; fixed; `cpre` hang fixed |
| `src/winreg/reg2cmd, regfilter` | .REG to batch | `regfilter` is a "zombie copy" by its own README |
| `src/xmljson/jsonpp, xmlpp, xml2json, xml2moon, opensearch-dump` | pretty printers and converters | crashes on empty/missing input fixed |

## 4. Cross-cutting findings

**Containers.** The tree has `hashmap` (open addressing), `hmap` (ordered,
typed), `set` (ordered, bytes), `cbmap`/`cb` (critbit), `bmap` (macro map),
`map.h` (a facade chosen by `#define MAP_USE_*` at the top of each file, so
two files that include the same header can disagree about what `MAP_T` is),
plus `list.h`, `slist`, `dlist`, `linked_list.h` (declared, never
implemented, included by `map.h` and used by nothing) and `tree.h` (a shell AST with no user in this tree; it reads like shish's). Four of these
do the same job. Proposal in section 6.

**Error handling.** Three idioms: return 0/-1 and `errno` (libowfat),
`assert` (hmap, set before this session), and fall-through silence (the lenient
JSON reader, `buffer_mmap*` before this session). The first is the project's
own tradition and should win; `assert` on out-of-memory must go.

**Bounds.** The format modules (elf, pe, coff, omf, macho) take a base
pointer. A parser of untrusted files needs a view type: `{x, size}`. Plan in
section 6.

**Headers.** Several headers declare half their API only if another header was
included first (`#ifdef STRALLOC_H`, `#ifdef BUFFER_H`). That made
`wcs_to_u8sa` and `set_join` invisible to callers; fixed for `utf8`, `set`
and `hmap`, still true for `xml.h` (`XML_HMAP_BUCKETS`), `strarray.h`.

**Complexity discipline.** Where the notes say O(n), the code mostly is.
The sins were all hidden behind a convenient interface: a function that
measures a string it was handed piecewise, a table that cannot grow, a
growth step that is a constant. Measurements to keep as a regression
benchmark are in `tests/` (proposal 3).

**Documentation drift.** Before this session many of the man pages in `lib/http`
described functions that did not exist or arguments that did not match
(`http_connect`, `http_response_init`, examples passing `NULL` buffers). The
pages for `http`, `sse`, `ndjson`, `set` follow the code; the other modules'
pages were not audited and should be assumed to need the same pass (a
script can check synopses against headers).

**Repository hygiene.** Removed in this session: eight `faillog-*` files, a
Windows test binary, and an RSA private key with its certificate (expired
2019, unreferenced). The key stays in git history. `.gitmodules` points at
paths that no longer exist. No CI configuration exists.

## 5. What changed in the library's API during this session

* `set`: smaller (nine functions removed, none had users), insertion ordered,
  never aborts; `set_first/last` replace `set_at*`; return codes
  `SET_ADDED/EXISTS/ERROR`.
* `hmap`: same function names, new internals; `TUPLE` lost `type`, `index`,
  `hash_prev`, the inline key and `data`; `hmap_add_tuple_with_data` and the
  `hmap_print_*` helpers were removed (unused).
* `utf8`: invalid input is now observable (`u8_to_wc` returns 0; bulk
  converters return `(size_t)-1`); `wc_to_u8` stops at U+10FFFF.
* `elf_check`, `pe_check`, `sse_*`, `ndjson_*`, `json_valid/parse_text/property_path`,
  `http_*` additions, `tls_servername`: new.

## 6. Where the project can go

Ordered by value for effort; each item says what it enables.

1. **Make `genmakefile` the product** (see `doc/genmakefile-spec.md`).
   It is the only part of the project that nothing else does as well, and the
   library already is its runtime. A tool that reads a source tree and emits
   a correct build for the compiler at hand, with no configuration step, is
   rare and wanted by exactly the audiences the author serves (retro and
   embedded toolchains, cross builds, reproducible bootstraps).
2. **One ordered map.** Replace `hmap`/`hashmap`/`bmap` and the `MAP_USE_*`
   facade by a single type: bytes keys, bytes or typed values, insertion
   order, growth, O(1). `set` is already that for keys; a map is a set whose
   buckets carry a value. This removes about 1,000 lines and a whole class of
   include-order bugs.
3. **Make every claim a test, make tests a gate.** Add the missing test
   files (`charbuf`, `coff`, `cpp`, `dns`, `omf`, `playlist`, `process`, `sig`,
   `terminal`, `textbuf`, `tls`, `ucs`, `bmap`), a CI job that builds with
   `-Wall -Wextra -Werror` for the files that are clean, runs the suite under
   valgrind or ASan, and runs a time-ratio benchmark (the one used in this
   session) so a quadratic change fails the build. A fuzz target for each
   parser (`json_valid`, `sse_feed`, `ndjson_feed`, `xml_read_tree`,
   `elf_check`, `pe_check`, `cpp_tokenize`) costs an hour each and would have
   found most of the crashes above.
4. **A bounded view for binary formats.** `typedef struct { const uint8* x; size_t n; } bytes;`
   and `elf_open(bytes)` returning an object whose accessors are
   range-checked; keep `elf_check` as the cheap front check. Same for PE and
   COFF; `binfmt_open` becomes the sniffing entry point it was meant to be.
5. **TLS that verifies.** Certificate chain and host name against
   `h->host`, system roots or a pinned file, SNI (done). Until then
   `copenai`/`cgemini` should refuse `https` unless `--insecure` is given.
6. **HTTP keep-alive and connection reuse**, now that `http_recv` knows
   where a response ends; multiple requests per `http` for the LLM
   clients' multi-turn conversations.
7. **Split `lib/unix`** (4,100 lines of platform shims) into per-platform
   directories selected by `genmakefile`/CMake, and put `glob`, `fnmatch`
   (duplicated in `path`) and `getopt` in their own modules.
8. **Generate the README and man page index** from the headers, and a script
   that diffs each man page synopsis against its header, so that drift is a
   CI failure instead of a rumor.
9. **Finish or delete**: `linked_list.h`, `tree.h` (move to shish),
   `ftpcopy`/`ftpls`/`redir`/`dnsq` imports that do not compile, `cxx/`,
   `regfilter`, `bmap`, `iom`, `aio_passwd`, `try-llseek`.
10. **One multi-call binary** (`c-utils cat`, `c-utils pelist`, ...) in
    the BusyBox manner for the 72 programs, since they share the same static
    library: smaller install, easier to ship to a target that has nothing.

### Things I would not do

* Rewrite `buffer` or `io`: they are the parts that work and that other
  people know (libowfat).
* Replace the C preprocessor with a call to the compiler: the independence
  from the target compiler is the point of `genmakefile`.
* Chase all 155 `-Wswitch` warnings by adding `default:` branches; give the
  `cpp` enum an explicit "not handled here" case where that is true.

## 7. What I did not do

* I read the structure and public headers of every module, and the code of
  those I fixed or tested (marks F, T, M). Modules marked **S** were not read
  line by line; their notes come from headers, warnings, BUGS entries and
  the README, and may be wrong in detail.
* Tests written this session cover utf8, range, slist, dlist, cb, cbmap, set,
  hmap, elf, pe, sse, json (new functions), http (new functions). That is not
  "every library function": sixteen modules still have no test file and
  several others (`textcode`, `strarray`, `iob`) have far less than their
  size warrants. Stating this plainly is better than a number.
* Tools were exercised with `-h`, empty, random, truncated, corrupt and
  missing input, not with realistic inputs; network tools were not run
  against real servers.
* The Windows, Borland and Android branches were not built.
