



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



      lib/http.h           HTTP client model with requests, responses and headers, compression chains async reading via iopause
      lib/charbuf.h        an 1-byte character buffer


  Serialization / Data formats:

      lib/json.h           JSON callback and tree parsers, tree iteration, token output
      lib/xml.h            XML read with callback, set of functions for building element and attribute lists
      lib/playlist.h       Read/write Playlist generic playlist abstraction

   Word expand, exec and wait process

      lib/expand.h         shell word expansion of ${vars}, special params and backticks command into string list
      lib/process.h        exec and wait for processes
      lib/wait.h           wait/waitpid from djb
      lib/sig.h            sig action/block from djb


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

      src/filedata/ihextool.c Intel HEX swiss knife: info, records, dump, merge/cut, bin, frombin, verify, sum
      src/filedata/ccat.c  Compressed file cat with getopt() options and error reporting
      czip                 ccat run under this name (installed as a link) compresses by default; type from -t or the file extension (gz bz2 xz lzma br)
      crc32.c              Calculate crc32 checksum, using zlib or diet version


  Network utilities

      src/dnsutils/dnsip.c Resolve hostname into IP addresses (NS from resolv.conf or DNSCACHE environment variable)
      src/dnsutils/dnsname.c Reverse lookup of IP address
      src/tcp/tcping.c     try to TCP connect and output time of each acknowledge or reset
      src/tcp/chttpcat.c   HTTP/HTTPS client, GET of every URL, stdin is POSTed chunked, -i -H -X -L -o
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

  The unit tests for the libraries (ini, uri, strarray, rdir, binfmt, xml, json, tokenizer, buffer, ...) are in tests/.


