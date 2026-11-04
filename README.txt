c-utils -- where this code comes from
=====================================

  Dan Bernstein (djb), 1995-2000.  While writing qmail, djbdns, daemontools
  and cdb, djb did not use the C library for strings, I/O or memory.  He
  wrote his own small helper functions instead: byte_*, str_*, fmt_*, scan_*,
  buffer, stralloc, open_*, case_*, uint32_pack and so on.  Their API is the
  thing to learn from: a function does one job, takes the operand being
  worked on as argument 1, returns an index or a length instead of a pointer
  (strcpy and strcat return pointers that nobody needs), and reports
  failure in a way that cannot be ignored by accident.  The functions were
  never released as a library; they are copied between his packages.

  Felix von Leitner (fefe), 2001-today.  In his talk "Writing Small And Fast
  Software" (January 2001, in doc/small-and-fast.md) he argued that
  indirection induces bloat and kills performance, that APIs should be
  specific first and extended by adding functions, and that the way to small
  programs is a libc that is only a static library of tiny objects.  He wrote
  dietlibc for the second part and libowfat for the first: "a GPL
  reimplementation of Dan Bernstein's internal helper functions", so called
  because you link it with -lowfat.  He extended it where needed (IPv6
  sockets, uint32_read, the io/iob event loop, critbit trees, protocol
  buffers, JSON and ASN.1 helpers).  Many of the original djb functions have
  since been put in the public domain; the libowfat versions and extensions
  are GPL version 2 only.

  c-utils, this repository.  It started from a libowfat tree and grew along
  three lines: more modules in the same style (hmap, set, cb/cbmap, json,
  xml, http, sse, cpp, elf/pe/coff/omf/ihex readers, ...), ports to places
  libowfat does not go (Windows native and Borland/MSVC/MinGW toolchains,
  Android, dietlibc/musl static builds, Mach-O), and a set of command line
  tools built on them.  The conventions are unchanged:

      - one function per file, one static library per lib/<name>.h, so a
        program links only the functions it calls
      - no stdio.h, stdlib.h or string.h in program code; use buffer, errmsg,
        str, byte, stralloc, alloc and env
      - fmt_* with a NULL destination only measures; scan_* returns the
        number of bytes consumed
      - to extend an API, add a function; never make an old one more powerful

  Compatibility.  A program written for libowfat 0.32 should compile and
  behave the same against lib/*.h.  The audit, the list of intentional
  differences and the test harness are in doc/libowfat-compat.md and
  tests/libowfat-compat/.  Where c-utils and libowfat disagree, c-utils adds
  a new function (scan_u64 beside scan_ulonglong) and keeps the libowfat
  signature.  The libowfat-derived parts (cdb, critbit, the fmt/scan
  protocol functions) are GPL version 2 only; see doc/libowfat-compat.md
  about the license of the repository as a whole.

  Further reading, in the order to read them:

      doc/small-and-fast.md      the design rules, where each one is in the
                                 tree, where the tree breaks it
      doc/libowfat-compat.md     what "compatible" means and how it is tested
      doc/review.md              module by module assessment
      doc/http.md, doc/genmakefile.md, doc/genmakefile-spec.md


  Contignuous containers:

      lib/range.h          tuple {start,end,size} a pointer range with item size in bytes
      lib/strarray.h       an array of pointer to string, string always '\0' terminated, vector always NULL terminated
      lib/strlist.h        character separated list (\0 \n , ...) in a stralloc


  Linked list containers:

      lib/slist.h          single ended queue (head) singly linked (next) list container.
                           mutable traversal using node** x = &head
                           iteration with node* p = head

      lib/list.h           double ended queue (head,tail) using double linked lists (prev,next)
                           with list traversal macros list_foreach,*_safe,*_reverse

  Assiociative and hierarchical containers:

      lib/hmap.h           hash map container with ownership on property of built-in type or custom *ptr,struct_size
      lib/hashmap.h        associative map container using hashed bucket algorithms
      lib/cb.h             crit-bit trees
      lib/cbmap.h          an associative map container based on crit-bittrees
      lib/map.h



      lib/http.h           HTTP client model with requests (any method, headers, body or chunked body), responses and headers, streamed body decoding, compression chains async reading via iopause
      lib/charbuf.h        an 1-byte character buffer


  Serialization / Data formats:

      lib/json.h           JSON callback and tree parsers, strict validation, property paths, NDJSON streams, tree iteration, token output
      lib/xml.h            XML read with callback, set of functions for building element and attribute lists
      lib/playlist.h       Read/write Playlist generic playlist abstraction

   Word expand, exec and wait process

      lib/expand.h         shell word expansion of ${vars}, special params and backticks command into string list
      lib/process.h        exec and wait for processes
      lib/wait.h           wait/waitpid from djb
      lib/sig.h            sig action/block from djb
      lib/sse.h            server-sent events (text/event-stream) parser for streamed answers


      lib/var.h            Lexically ordered NAME=VALUE pairs with attributes (exported, local, readonly, ...)
      lib/vartab.h         Table for variable lookup stacked for subshell context


  Dumb charset conversion UCS, UTF-8, Latin 1, Punycode and IDNA

      lib/ucs.h


  Terminal output through function calls (VT on POSIX, console API on Windows)

      lib/terminal.h


  Periphery / Sensing

      lib/gpio.h           Memory map Raspberry Pi GPIO pins, provide tri-state digital i/o


  Base function from fefe libowfat:

        array    dns      open     taia
        buffer   examples scan     test
        byte     fmt      socket   textcode
        case     io       str      uint
        cdb      mmap     stralloc unix
        critbit  mult     tai


      lib/array.h          allocated byte vector item indexing with umult64
      lib/buffer.h         buffered standard input/output/error. op is pointer to a read/write(fd,buf,len) = num_bytes
      lib/byte.h           operations on fixed length byte ranges
      lib/case.h           case-insensitive versions of str comparision functions from fefe libowfat
      lib/dns.h            djbdns client api
      lib/fmt.h            formatted output into byte buffer functions
      lib/io.h             libfefe I/O muxing for unix and windows
      lib/alloc.h          alloc, alloc_re, alloc_free djb interface

      lib/iarray.h         array of integers container
      lib/fmt.h            formatted output into byte buffer functions
      lib/io.h             libfefe I/O muxing for unix and windows
      lib/dns.h            djbdns client api
      lib/errmsg.h         fefe libowfat





  Command line (CLI) utilities:

      src/toolchain/pkgcfg.c Drop-in pkg-config, prints compile and link flags for package
      src/binfmt/impgen.c  Generate a .DEF file from a .DLL library

      src/toolchain/ar-wrap.c generic cross-compiler wrapper for the archiver (lib bundler)
      src/toolchain/cc-wrap.c generic cross-compiler wrapper for the front (preprocessor, compiler, assembler)
      src/toolchain/compiler-wrapper.c A compiler driver for Microchip PIC toolchains (picc,picc18,xc8,sdcc)
      src/toolchain/cmake-run.c find cross environment using dir and path functions and pass it to CMake

      src/genmakefile/genmakefile.c Makefile / Ninja generator based on

                                *.c *.h          files
                                .                directories

                                CC, CFLAGS       compiler, flags

                                AR, ARFLAGS      librarian
                                LINK, LDFLAGS    linker, flags

                                -L and -l        add library
                                -I and -D        add include/definition


      src/binfmt/ntldd.c   recurse dynamic bindings of .exe files like /usr/bin/ldd on Linux
      src/binfmt/pelist.c  List .EXE and .DLL file sections, import tables and export directories
      src/binfmt/omflist.c List OMF object files symbols
      src/binfmt/cofflist.c List COFF object files symbols
      src/binfmt/elflist.c List Elf32 headers, sections, segments, symbols and dynamic section
      src/binfmt/elf64list.c List Elf64 headers, sections, segments, symbols and dynamic section
      src/binfmt/macho32list.c List Mach-O segments and symbol table


  Compression and integrity

      src/filedata/ihextool.c Intel HEX swiss knife: info, records, dump, merge/cut, bin, frombin, vectors, verify, sum
      src/filedata/ccat.c  Compressed file cat with getopt() options and error reporting
      czip                 ccat run under this name (installed as a link) compresses by default; type from -t or the file extension (gz bz2 xz lzma br)
      crc32.c              Calculate crc32 checksum, using zlib or diet version


  Network utilities

      src/dnsutils/dnsip.c Resolve hostname into IP addresses (NS from resolv.conf or DNSCACHE environment variable)
      src/dnsutils/dnsname.c Reverse lookup of IP address
      src/tcp/tcping.c     try to TCP connect and output time of each acknowledge or reset
      src/tcp/chttpcat.c   HTTP/HTTPS client, GET of every URL, stdin is POSTed chunked, -i -H -X -L -o
      src/tcp/copenai.c    streams the answer of an OpenAI compatible chat API (chunked POST, SSE or NDJSON), $OPENAI_API_URL, $OPENAI_MODEL, $OPENAI_API_KEY
      src/tcp/cgemini.c    conversation with Google Gemini on the terminal, a pipe or a TCP socket (-l), $GEMINI_API_KEY
      src/tcp/llmstream.c  shared by those two: the event loop, SSE or NDJSON by Content-Type, error messages
      src/tcp/cftpcat.c    FTP client, ftp:// URL lists or downloads, HOST COMMAND runs ls/cat/cd/pwd/size/quote -u -p -P -v
      src/tcp/ctlscat.c    netcat style relay of stdin/stdout over TLS (plain with -n), connects or listens with -l


  Binary file tools

      src/filedata/bsdiffcat.c Translates a binary diff made with bsdiff into patch(buf, offset, ifval, thenval) calls
      src/filedata/hexedit.c Memory map a file and process a set of patch data in the form: ADDRESS IF-BYTE SET-BYTE
      src/filedata/torrent-progress.c memory map incomplete torrent data calculate percentage of pages which are all zero-bytes


  Media and streams

      src/media/mediathek-list.c HTTP load and filter mediathek-filme.json of the MediathekView application
      src/media/mediathek-parser.c Parse a mediathek-filme.json store and generate a Playlist from it

  Windows registry

      src/winreg/reg2cmd.c Convert a windows registry .REG file into a .BAT with REG ADD commands
      src/winreg/regfilter.c zombie copy reg2cmd, was intended a filtering version, like grepdiff


  Path helpers

      src/toolchain/pathtool.c dropin for realpath, with --relative-to DIR feature and windows/msys support cygpath compliant
      src/system/sln.c     for a /usr/lib/libX.so.major.minor file pass one or more link names: libX.so.major libX.so /lib/libX.so
      src/toolchain/msys-shell.c Registry & path search for msys/msys2 environments and launch mintty and bash inside

  Utilities reading/writing XML/JSON

      src/xmljson/opensearch-dump.c parse an OpenSearch XML file into an URL /search?query=%s to have them in Firefox/Chrome
      src/media/plsconv.c  Convert playlists between the formats .M3U .PLS .XSPF

      src/xmljson/xmlpp.c  pretty-print XML/XHML
      src/xmljson/jsonpp.c Pretty-print JSON data
      src/xmljson/xml2json.c convert an element tree into json objects { tagName, ...attributes, children: [ ..innerElements
      src/xmljson/xml2moon.c convert XHTML content into lapis html generator syntax: element prop: val, prop2: ..

      src/eagle/eagle-gen-cmds.c Read EAGLE XML schematics/layouts and generate commands to draw contour and x/y axis measures
      src/eagle/eagle-init-brd.c Generate component arrangement congruent with Schematic and generate translate and rotate commands
      src/eagle/eagle-to-circuit.c Read EAGLE XML packages and netlists, write .circuit file for striprouter


  Basic usage examples or boilerplates:

      src/toolchain/cpre.c        C preprocessor driver (-D -I -P -L, dump defines, dependencies)

  The unit tests for the libraries (ini, uri, strarray, walk, binfmt, xml, json, tokenizer, buffer, ...) are in tests/.


