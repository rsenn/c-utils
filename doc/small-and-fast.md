# Writing small and fast software -- the design rules behind c-utils

c-utils follows the rules Felix von Leitner (fefe) set out in the talk
*Writing Small And Fast Software* (Convergence, January 2001), the talk that
accompanies dietlibc and libowfat. This page restates the rules, shows where
each one is visible in the tree, says where the tree does not follow it, and
explains the dietlibc support. For the family tree of the code see the
primer at the top of `README.txt`; for how far libowfat source compatibility
goes see [libowfat-compat.md](libowfat-compat.md).

The talk in one sentence: **indirection induces bloat and kills
performance; remove unnecessary indirection.** More code is almost always
bad -- it trashes the CPU cache, hogs the memory bus, burns cycles, makes an
audit harder, is harder to follow and maintain, and needs more documentation.

## 1. Know the enemy

The talk names four sources of large code. They are also the review checklist
for a change to this repository.

| Enemy | What it looks like | What to do here |
|---|---|---|
| Negligence | merged code with overlap left in, debug code left in, general APIs where specific ones exist (`fprintf` for a static string), the same helper written twice | before adding a helper, `grep` lib/ for it; delete the loser when two exist |
| Over-eagerness | the ultimate all-encompassing API, methods "just in case" | write the function the caller needs; add the next one when the next caller appears |
| Bad management | every check-in is a merge; several modules solving one problem (the talk's example: AVL trees in the Linux kernel) | see "where we do not follow it" below |
| Wrappers around wrappers | portability layers on portability layers | one thin layer: `lib/*.h` over the system call, no layer over that |

Two more the talk names: **dynamic run-time resolving** (signal/slot tables,
symbolic attribute lookup in hash tables, late binding) and **the belief that
the compiler or linker removes what you do not use**. They do not: the
compiler keeps every non-static function (`nm t.o` still lists `foo`), and
the linker pulls whole objects, so a global that sits in an unrelated
translation unit comes along (the slide greps a binary for the string
`bloat`). Shared libraries make this worse because the programmer no longer
sees the size of the code; the data segment is coalesced and the page
granularity is 4 KiB, so "only the used pages are loaded" is no excuse.

## 2. The rules and where they are in the tree

### Generalize only if it is free; write specific interfaces first

> If you leave the names of the arguments away, is it still obvious what each
> argument does?  `int __ltostr(char*, int, unsigned long, int, char)` -- no.
> `int fmt_ulong(char*, unsigned long)` -- yes.

`lib/fmt.h` is the model: `fmt_ulong`, `fmt_long`, `fmt_xlong`, `fmt_8long`,
`fmt_ulong0`, `fmt_plusminus`, `fmt_minus`, `fmt_str`, `fmt_strn`. One tiny
function each, no base argument, no case flag. Nobody pays for the
`printf` format interpreter to print a number.

**When an API must be extended, add a function; never make an existing one
more powerful.** The tree applies this literally:

* `buffer_get_token_pred` keeps the exact libowfat signature (predicate with
  `(const char*, size_t)`); the variant with a user argument is the new
  function `buffer_get_token_pred_arg`. The same split exists for the
  stralloc token readers.
* `scan_ulonglong` takes `unsigned long long*` as in libowfat; the
  `uint64` flavour is the new `scan_u64`, `scan_x64`, `scan_i64`.
* `fmt_escapecharjson` stays libowfat's; the quoting variant is
  `fmt_escapecharjsonq`.
* `set_add`, `set_insert` and `set_has` instead of one `set_put` with flags
  (the old general entry point is internal now).

The converse also holds: when a function turned out wrong, the fix changed
the function, not the callers, and the removal list in `ChangeLog` names the
things that nothing used (`set_union`, `set_toarray`, `set_joinq`, ...). An
API nobody calls is bloat that someone has to document.

### Return the index, not the pointer; operand first; NULL means "just measure"

The byte/str/fmt/scan families follow the djb conventions the talk lists:

* `byte_chr`, `str_chr` return an **index**, not a pointer; "not found"
  returns the length. No `NULL` checks, no pointer subtraction.
* The operand being operated on (destination, main operand) is argument 1.
* `str_copy`, `fmt_*` return the **number of bytes**, not the destination
  pointer -- "what did the people smoke who made strcpy and strcat return
  pointers?!"
* `fmt_*` accept `dest == NULL` and then only return the length that would
  have been written. A caller sizes the buffer with one call and fills it
  with the next, with no `snprintf`-style truncation.
* `scan_*` return the number of bytes consumed (0 on failure) and do not
  touch the result on failure.

The differential test in `tests/libowfat-compat` checks 94 of these functions
bit for bit against the real libowfat, so the conventions cannot drift.

### Bad API versus good API

```c
/* bad */  int fd = open("foo", O_WRONLY|O_CREAT, 0600);
/* good */ int fd = open_write("foo");

/* bad */  char buf[100]; sprintf(buf, "the number is %d", i);
/* good */ stralloc s = {0};
           stralloc_copys(&s, "the number is ");
           stralloc_catuint(&s, i);
```

`lib/open.h` (`open_read`, `open_write`, `open_append`, `open_trunc`,
`open_excl`, `open_rw`) and `lib/stralloc.h` are those two slides. The
"no `stdio.h`/`stdlib.h`/`string.h` in program code" rule in `CLAUDE.md` is the
same advice enforced: `buffer` (+`buffer_1`/`buffer_2`) for `printf`,
`errmsg` for `fprintf(stderr, ...)`, `str`/`byte`/`stralloc` for `string.h`,
`alloc`/`env` for `malloc`/`getenv`. It also keeps every tool on one
vocabulary and portable to the `WINDOWS_NATIVE` targets.

### One function per file

`lib/<name>.h` is the public header, `lib/<name>/` holds one `.c` file per
function. This is the talk's static-library argument built into the layout:

> `ld` does not include unreferenced object files from static libraries.
> That's why the diet libc is initially only a static library.

Each module becomes a static library (`libfmt.a`, `libbyte.a`, ...) whose
members are single functions, so a program that calls `fmt_ulong` links
`fmt_ulong.o` and nothing else. Appending a function to an existing file
would pull it into every program that uses its neighbour. That is why new
functions get new files (the `CMakeLists.txt` globs `lib/<name>/*.c`; rerun
`cmake .` after adding one). The ported libowfat sources (`cdb`, `critbit`)
were split the same way, one public function per file, shared `static` helpers
travelling with the functions that use them.

### Prefer specific over generic routines

* `alloca` / stack buffers for temporary storage (`FMT_ULONG`, `FMT_LONG`,
  `FMT_XLONG` are the buffer sizes for the `fmt_*` family).
* `str_chr` instead of `strstr`, `qsort`/`bsearch` instead of a tree for
  small static sets.
* **Optimize for the common case.** Arrays are easy to understand, cheap to
  resize and cache friendly. `lib/array.h`, `lib/stralloc.h`, `lib/strarray.h`
  and `lib/range.h` are the workhorses; the insertion-ordered open-addressing
  `set` and `hmap` replaced linked buckets for the same reason.

### Treat malloc as an expensive operation

The slide counts 20 794 `malloc` calls in one program run with an `LD_PRELOAD`
shim. Here: `stralloc` grows geometrically and is reused; `buffer` works on
a caller-supplied (often static or stack) array; `array` is one block;
`hmap` entries are one allocation (the old 1.1 KB per tuple and the fixed
1024-slot table were removed as bloat).  The `LD_PRELOAD` shims in `src/intercept/` (exec and socket calls) use
the same technique for other calls.

### Static inline, not inline

An `inline` function without `static` still emits a global symbol (the slide's
`nm t.o` shows `T foo`). Use `static inline`: the function disappears when
inlined, and the name space stays clean. Headers in `lib/` use
`static inline` for the small helpers (`scan_u64`, `fmt_copybytes`, the
`atomic.h` operations).

### Profile, do not speculate

Space for time is not bad per se, but only where a profiler says so. `-Os` is
`-O2` without the space-wasting trade-offs; `-fomit-frame-pointer`; look at
`nm` and `objdump`. The complexity audit that produced this release worked the
same way: timing at *n* and *4n* and flagging anything that grew faster than
linearly found the quadratic JSON/XML paths, `charbuf_froms`, and the fixed-size
hash table, none of which a read-through had caught.

## 3. dietlibc support

dietlibc is the other half of the talk: a libc that is only a static library,
so that the "ld does not pull unreferenced objects" rule applies to the C
library itself. c-utils builds against it:

* `scripts/diet-gcc` and `scripts/diet32-gcc` are compiler wrappers that
  call `diet gcc`, map `-O1..9` to `-Os`, drop `-fPIC`/`-pie`/`-shared`, and
  answer `-dumpmachine` with a `...-dietlibc` triple. Point CMake at them
  (`-DCMAKE_C_COMPILER=.../scripts/diet-gcc`).
* `CMakeLists.txt` recognises dietlibc either by the compiler name matching
  `diet` or by `check_symbol_exists(__dietlibc__)`, sets `TOOLCHAIN=diet`, and
  switches to static linking with shared libraries off (the same for musl).
* Source files test `__dietlibc__` where the libc differs (`sys/atomic.h` in
  `lib/cas.h`, `sys/sendfile.h` and `bits/syscall.h` in the capability probes,
  `timegm` in `scan_iso8601`).

What we measured: on a glibc system a statically linked `printf` hello world
and the same program written with `buffer` + `fmt_ulong` are both about 700 KB,
because glibc's start-up and stdio dominate and static glibc cannot drop
them. The saving the talk promises only appears with a libc that is itself
built from small objects -- dietlibc or musl. (dietlibc is not installed in
the build environment of this release, so no diet size table is given here.)

## 4. Where the tree does not follow the talk

Being straight about this is part of the rule "tidy up".

* **Several associative containers.** `hashmap`, `hmap`, `cbmap`, `cb`, `map`,
  `set` and `bmap` all exist. The talk's "bad management" example is exactly
  this. `map.h` already fronts `hmap`; `doc/review.md` and the `TODO` entry
  `one-ordered-map` propose reducing them to one hash container and one
  ordered (crit-bit) container.
* **Dead code and stubs.** `lib/linked_list.h`, `lib/tree.h`, `lib/bmap.h`
  have no or unused implementations; they are listed in `BUGS`.
* **Wrappers.** `lib/cas.h` and `lib/atomic.h` overlap (the second exists only
  for libowfat's `CAS.h` source compatibility, see below), and `io` has an
  `iom` layer over epoll/kqueue over `io`.
* **Big tools.** `genmakefile` and `cpre` carry a full C preprocessor; the
  talk would ask whether that belongs in `lib/cpp` as a separate, optional
  library -- it is one, and a program that does not use it does not link it.
* **Documentation of the intent.** Several modules lack manual pages (see
  the `man-pages-against-headers` TODO entry).

## 5. Checklist for a change

1. Is there already a function that does this? (`grep` `lib/*.h` and `README.txt`.)
2. Is the new function specific -- could its arguments be understood without
   their names?
3. Does it return an index/length rather than a pointer, and does a `NULL`
   destination just measure?
4. One function, one file, `static` for helpers, `static inline` in headers.
5. No `stdio.h`/`string.h`/`stdlib.h` in programs; no hidden `malloc` in a
   function that could use the caller's buffer.
6. Extending an existing function? Stop and add a new one.
7. Did you look at `nm` of the object and the size of the linked tool?
