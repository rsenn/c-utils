






        cb   crit-bit trees
     cbmap   an associative map container based on crit-bittrees
   charbuf   an 1-byte character buffer
    expand   shell word expansion of ${vars}, special params and backticks command into string list
      gpio   raspberry pi GPIO pins
   hashmap   associative map container using hashed bucket algorithms
      hmap   hash map container with ownership on property of built-in type or custom *ptr,struct_size
      http   HTTP client model with requests, responses and headers, compression chains async reading via iopause
      json   JSON callback and tree parsers, tree iteration, token output
      list   doubly-linked list originated from ircd code list:{node*head,node*tail} node:{node*next,node*prev}
       map
  playlist generic playlist abstraction
   process exec and wait for processes
     range struct range is a tuple of { char*start,*end;size_t step }
     slist singly linked list container.
           mutable traversal using for(node**tail = &root; *tail; tail = &(*tail)->next)
           list and iteration with node = node->next




  strarray an array of pointer to strings
   strlist A character separated list ('\0', '\n', ',', ...) in a stralloc
   textbuf
       ucs
       var
    vartab
       xml XML read with callback, set of functions for building element and attribute lists

      wait
       sig

base function from fefe libowfat:

     array   allocated byte vector item indexing with umult64
    buffer   buffered standard input/output/error. op is pointer to a read/write(fd,buf,len) = num_bytes
      byte   operations on fixed length byte ranges
      case   case-insensitive versions of str comparision functions from fefe libowfat
       dns   djbdns client api
       fmt   formatted output into byte buffer functions
        io   libfefe I/O muxing for unix and windows
     alloc   alloc, alloc_re, alloc_free djb interface
    socket
       str
  stralloc
       tai
      taia
  textcode
    uint16
    uint32
    uint64
      unix
      scan
      mmap
    ndelay
      open
  safemult
    iarray   array of integers container
       fmt   formatted output into byte buffer functions
        io   libfefe I/O muxing for unix and windows
       dns   djbdns client api
    errmsg   fefe libowfat





Command line (CLI) utilities:

           ar-wrap.c   generic cross-compiler wrapper for the archiver (lib bundler)
         bsdiffcat.c   Translates a binary diff made with bsdiff into patch(buf, offset, ifval, thenval) calls
              ccat.c   Compressed file cat with getopt() options and error reporting
           cc-wrap.c   generic cross-compiler wrapper for the front (preprocessor, compiler, assembler)
         cmake-run.c   find cross environment using dir and path functions and pass it to CMake
  compiler-wrapper.c   A compiler driver for Microchip PIC toolchains (picc,picc18,xc8,sdcc)
             crc32.c   Calculate checksum, using zlib or diet version
             dnsip.c   Resolve hostname into IP addresses (NS from resolv.conf or DNSCACHE environment variable)
           dnsname.c   Reverse lookup of IP address
    eagle-gen-cmds.c   Read EAGLE XML schematics/layouts and generate commands to draw contour and x/y axis measures
    eagle-init-brd.c   Generate component arrangement congruent with Schematic and generate translate and rotate commands
  eagle-to-circuit.c   Read EAGLE XML packages and netlists, write .circuit file for striprouter
       genmakefile.c   Takes set of sources, variables CC,CFLAGS,AR,LINK,LDFLAGS, -I and -D preprocessor flags, -L and -l linker flags
           hexedit.c   Memory map a file and process a set of patch data in the form: ADDRESS IF-BYTE SET-BYTE
            impgen.c   Generate a .DEF file from a .DLL library
            jsonpp.c   Pretty-print JSON data
    mediathek-list.c   HTTP load and filter mediathek-filme.json of the MediathekView application
  mediathek-parser.c   Parse a mediathek-filme.json store and generate a Playlist from it
        msys-shell.c   Registry & path search for msys/msys2 environments and launch mintty and bash inside
             ntldd.c   recurse dynamic bindings of .exe files like /usr/bin/ldd on Linux
   opensearch-dump.c   parse an OpenSearch XML file into an URL /search?query=%s to have them in Firefox/Chrome
          pathtool.c   dropin for realpath, with --relative-to DIR feature and windows/msys support cygpath compliant
            pelist.c   List .exe and .dll file sections, import tables and export directories
            pkgcfg.c   A drop-in replacement for pkg-config used with GNU software
           plsconv.c   Convert playlists between the formats .M3U .PLS .XSPF
           reg2cmd.c   Convert a windows registry .REG file into a .BAT with REG ADD commands
         regfilter.c   zombie copy reg2cmd, was intended a filtering version, like grepdiff
               sln.c   for a /usr/lib/libX.so.major.minor file pass one or more link names: libX.so.major libX.so /lib/libX.so
            tcping.c   try to TCP connect and output time of each acknowledge or reset
  torrent-progress.c   memory map incomplete torrent data calculate percentage of pages which are all zero-bytes
          xml2json.c   convert an element tree into json objects { tagName, ...attributes, children: [ ..innerElements
          xml2moon.c   convert XHTML content into lapis html generator syntax: element prop: val, prop2: ..
             xmlpp.c   pretty-print XML/XHML



  Basic usage examples or boilerplates:

        binfmttest.c  Test binary format readers
        buffertest.c  Test I/O buffer system, chaining with zlib/bzip2/lzma filter
           dnstest.c  Test DNS resolver
          httptest.c  Test HTTP client
          jsontest.c  Test JSON parser
      strarraytest.c  Test array of char* pointer functions
           xmltest.c  Test XML reading with callbacks (diet version of SAX)
          xmltest2.c  Test XML reading as tree (diet version of DOM)
          xmltest3.c  Test XML traversal and selection (diet version of getElementBy* and XPath)
          xmltest4.c  Test XML query
           ziptest.c  Simple universal file compression of *.gz, *.bz2, *.xz based on file extension


