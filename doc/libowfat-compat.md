# libowfat compatibility

c-utils started as a copy of libowfat (see the primer at the top of
`README.txt`) and has grown away from it. This page states what "compatible"
means here, how it is checked, what was found broken and fixed in the
2026-11 audit, and the differences that remain on purpose.

Reference: **libowfat 0.32** (Debian `libowfat-dev 0.32-4.1`), headers, `libowfat.a`,
manual pages and source.

## What is promised

1. **Source compatibility.** A program written against libowfat's headers
   compiles against `lib/*.h` without changes, apart from the documented
   exceptions below, and behaves the same.
2. **Same conventions** (index not pointer, operand first, `NULL`
   destination measures, `scan_*` returns bytes consumed). See
   [small-and-fast.md](small-and-fast.md).
3. **Extensions never change existing signatures.** If c-utils needs another
   shape, it adds a function (`scan_u64` next to `scan_ulonglong`,
   `buffer_get_token_pred_arg` next to `buffer_get_token_pred`).

Not promised: binary (ABI) compatibility with `libowfat.a`, the exact set of
`#define`d feature macros, or bug-for-bug behaviour where libowfat is wrong
(see "libowfat bugs" below).

The types differ in one place that matters. On LP64 c-utils' `uint64` is
`unsigned long` and `int64` is `long`, while libowfat's `scan_*longlong` and
`fmt_*longlong` use `unsigned long long`. Pointer parameters must therefore
use libowfat's exact type, which they now do; value parameters convert freely.

## How it is checked

All of it is in `tests/libowfat-compat/` and needs only the libowfat source
tree and `libowfat.a` (`apt-get source libowfat`, `apt install libowfat-dev`).

```sh
# 1. every prototype in libowfat's headers vs lib/*.h
python3 tests/libowfat-compat/api-diff.py /path/to/libowfat-0.32 "" /usr/lib/libowfat.a

# 2. differential test: both libraries linked into one program, random
#    inputs through 94 fmt/scan/str/byte/case/uintN functions, outputs compared
tests/libowfat-compat/run.sh /path/to/libowfat-0.32 /usr/lib/libowfat.a \
     build/x86_64-linux-gnu/lib 20000

# 3. syntax check of every libowfat source file against lib/*.h
for f in libowfat-0.32/*/*.c; do gcc -fsyntax-only -w -Ilib -I libowfat-0.32 $f; done
```

`run.sh` renames libowfat's defined symbols to `owf_*` with `objcopy
--redefine-syms` so both libraries link into one binary. It compiles the
libowfat side with `__attribute__` neutralised, because libowfat declares
many writing functions `pure`, which lets the optimiser drop the stores
(the Debian `libowfat.a` itself suffers from this in `fmt_ulong0` and
`fmt_netstring`, which print no digits; those two are the only expected
differences).

Result of the audit, after the fixes below: **517 prototypes compared, 0 missing,
512 identical in shape, 5 benign differences; 94 behaviour functions, 92
identical over 20 000 random inputs each, 2 differing only through libowfat's
own dropped stores.** Unit tests for everything ported are in
`tests/test_cdb.c`, `test_critbit.c`, `test_atomic.c` and the additions to
`test_fmt.c`, `test_scan.c`.

## What was broken, what was fixed

### Programs that stopped compiling

| libowfat API | What c-utils had | Fix |
|---|---|---|
| `buffer_get_token_pred(b, x, len, pred, ...)` with predicate `int(*)(const char*, size_t)` | predicate with an extra user argument under the same name | libowfat signature restored (`string_predicate`); the argument variant is `buffer_get_token_pred_arg` (`string_predicate_arg`). Same for `buffer_get_token_sa_pred`, `buffer_get_new_token_sa_pred` and their `_arg` forms |
| `buffer_init(b, op, fd, y, ylen)` with `op` of type `ssize_t(*)()` | `buffer_op_proto*` | new `buffer_op_ptr` parameter type accepts both |
| `io_pipe`, `io_socketpair`, `io_readfile`, `io_createfile`, `io_appendfile`, `io_readwritefile` take `int64*` | `int*` | libowfat pointer type |
| `scan_ulonglong`, `scan_xlonglong`, `scan_longlong`, `scan_varint`, `scan_pb_*`, `scan_asn1der*`, `scan_human` take `unsigned long long*` / `signed long long*` | `uint64*` | libowfat types (26 call sites in the tree adjusted); `scan_u64`, `scan_x64`, `scan_i64` are the `uint64`/`int64` siblings |
| `fmt_escapecharjson(char*, uint32_t)` | different semantics | libowfat's function; the quoting variant is `fmt_escapecharjsonq` |
| `assign(dest, src)` and `add_of`/`sub_of` in `rangecheck.h` | `assign(type, dest, src)` as a statement block (unusable in an expression) | libowfat's expression macros; nothing in the tree used the old form |

### Programs that compiled but behaved differently

| Function | Defect | Fix |
|---|---|---|
| `scan_varint`, `scan_pb_tag`, `scan_pb_type0_int/sint`, `scan_pb_type2_stringlen` | shifted an `int`: values over 32 bits were truncated (undefined behaviour), non-minimal encodings accepted, no overflow check | libowfat's 64-bit loop with the length limit and the minimal-encoding check |
| `scan_asn1derlength` | read an uninitialised variable when the length value was invalid | checks the sub-result first; `scan_asn1derlengthvalue` is declared in `scan.h` |
| `scan_fromhex` | returned `c - '0'` garbage for non-hex characters | libowfat's unsigned-byte version, `-1` for non-hex |
| `str_diff` | compared `char` (signed) so bytes above 0x7f sorted before ASCII, unlike `strcmp` | compares `unsigned char` |
| `fmt_str` and everything built on it (`fmt_httpdate`, `fmt_iso8601`, `fmt_escapecharxml`, `fmt_escapecharhtml`) | wrote a terminating NUL one byte beyond the returned length, overflowing exactly-sized buffers | copies exactly `str_len` bytes |
| `fmt_utf8` | refused code points above 0x3ffffff (5- and 6-byte forms) | libowfat's loop, up to 0x7fffffff |
| `fmt_escapecharc` | returned 0 for characters above 0xff | emits `\uXXXX` / `\UXXXXXXXX` like libowfat; parameter is `uint32` |

### Missing API, now present

* New modules `lib/cdb/` (`cdb.h`, `cdb_make.h`: reader, writer, traversal) and
  `lib/critbit/` (`critbit.h`: `critbit0_*`), ported one function per file.
* `fmt_netstring`, `fmt_varint`, `fmt_pb_tag`, `fmt_pb_type0_int/sint`,
  `fmt_pb_type1_double/fixed64`, `fmt_pb_type2_string`,
  `fmt_pb_type5_float/fixed32`, `fmt_pb_int/sint/double/float/string`,
  `fmt_copybytes`, `fmt_escapecharhtml` (declared; it existed as an alias).
* `scan_netstring`, `scan_pb_type1_double`, `scan_pb_type1_fixed64`,
  `scan_pb_type2_stringlen`, `scan_pb_type5_float/fixed32`, `scan_iso8601`,
  `scan_8longlong`; `mmap_readat` (declared).
* Headers `compiletimeassert.h`, `isset.h`, `va_narg.h`, `readclose.h`,
  `openreadclose.h`; `EDISCON` and `EPROCLIM` in `socket.h` for Windows.
* `atomic.h`: `compare_and_swap`, `atomic_add`, `atomic_add_return`,
  `atomic_inc/dec`, `atomic_inc_return/dec_return`, `atomic_or`,
  `atomic_and`, `barrier`, `mb`, `rmb`, `wmb`, the `smp_mb__*` macros.

## Differences that remain on purpose

| Item | Difference | Why |
|---|---|---|
| `CAS.h` | named `atomic.h` | `lib/cas.h` (internal `__CAS` macros) already exists; two files differing only in case cannot coexist on Windows/macOS checkouts. Change `#include <CAS.h>` to `"atomic.h"`. `atomic_add_return` returns the value (libowfat's builtin branch forgot `return`) and every operation takes `volatile` pointers |
| `buffer_free`, `buffer_munmap` | take `buffer*` (libowfat: `void*` of the data) | they are the `deinit` hooks of `buffer`, which here receives the buffer itself |
| `buffer_putflush`, `buffer_putsflush`, `socket_fastopen_connect4/6` | return `ssize_t` where libowfat returns `int` | superset; callers comparing with `-1` or `0` are unaffected |
| `mmap_read` | returns `char*` | libowfat: `const char*`; assignment to a `const char*` still works |
| `errmsg_iam` | returns `const char*` | libowfat: `void` |
| `io_wait` | returns `int64` | libowfat: `void` |
| `fmt_*longlong`, `fmt_human*`, `fmt_asn1der*` value parameter | `uint64`/`int64` | same width as `unsigned long long` on LP64 and LLP64; only function-pointer casts see a difference |
| `cdb_make_start` | returns the seek offset (2048) | that is libowfat 0.32's behaviour, kept; success of the later calls is `0` |
| `struct cdb` | must be zero-initialised before `cdb_init` | libowfat contract: `cdb_init` calls `cdb_free` on it |
| libowfat sources `io/*.c`, `iom` | do not compile against `lib/io_internal.h` | they are implementation, not API; `io.h` is compatible, the internal tables are c-utils' own |

## Headers and include order

Every header in `lib/` compiles on its own (`tests/check-headers.sh` checks
all of them). As in libowfat, a few functions that need the type of another
module are declared only when that header was included first, with
`#ifdef STRALLOC_H`, `#ifdef BUFFER_H` and the like (the stralloc and buffer
headers refer to each other, so neither can include the other). Include
`stralloc.h`, `array.h`, `strlist.h` before `buffer.h`, `json.h`, `xml.h` or
`textcode.h` when you use the functions that take them. The unit tests do so,
and an implicit declaration warning is the sign of a wrong order.

## libowfat bugs found on the way

These are bugs in libowfat 0.32 or its Debian build, noted for upstream and
**not** copied:

* `CAS.h`: `atomic_add_return` in the `__sync` branch has no `return`;
  the fallback branches pass `&x` (a `size_t**`) to `compare_and_swap`.
* `fmt.h` declares writing functions `__attribute__((pure))`. The compiler
  may drop the stores; the Debian `libowfat.a` prints no digits from
  `fmt_ulong0` and `fmt_netstring`.
* `cdb_make_start` returns `lseek`'s result, so the caller cannot tell
  success from failure by `== 0`.

## License

libowfat is licensed **GNU General Public License version 2 only** ("no later
versions", see its `README` and `COPYING`). Code ported from it in this
tree (the `cdb` and `critbit` modules, the netstring/protobuf/iso8601
functions, the compat headers, and the files marked "this header file comes
from libowfat") carries that obligation. The repository has no top-level
license file yet; `lib/cbmap/LICENSE` covers only `cbmap`. Choosing and
adding the project license is an open item (see `BUGS`).
