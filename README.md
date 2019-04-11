




Contignuous containers:

File Description

lib/range.h          tuple {start,end,size} a pointer range with item size in bytes                              
lib/strarray.h       an array of pointer to string, string always '\0' terminated, vector always NULL terminated 
lib/strlist.h        character separated list (\0 \n , ...) in a stralloc                                        


Linked list containers:

File Description

lib/slist.h          single ended queue (head) singly linked (next) list container. 
                     mutable traversal using node** x = &head iteration with node* p = head

lib/list.h           double ended queue (head,tail) using double linked lists (prev,next) 
with list traversal macros list_foreach,*_safe,*_reverse

Assiociative and hierarchical containers:

File Description

lib/hmap.h           hash map container with ownership on property of built-in type or custom *ptr,struct_size 
lib/hashmap.h        associative map container using hashed bucket algorithms 
lib/cb.h             crit-bit trees 
lib/cbmap.h          an associative map container based on crit-bittrees 
lib/map.h             



File Description

lib/http.h           HTTP client model with requests, responses and headers, compression chains async reading via iopause 
lib/charbuf.h        an 1-byte character buffer 


Serialization / Data formats:

File Description

lib/json.h           JSON callback and tree parsers, tree iteration, token output 
lib/xml.h            XML read with callback, set of functions for building element and attribute lists 
lib/playlist.h       Read/write Playlist generic playlist abstraction 

Word expand, exec and wait process

File Description

lib/expand.h         shell word expansion of ${vars}, special params and backticks command into string list 
lib/process.h        exec and wait for processes 
lib/wait.h           wait/waitpid from djb 
lib/sig.h            sig action/block from djb 


File Description

lib/var.h            Lexically ordered NAME=VALUE pairs with attributes (exported, local, readonly, ...) 
lib/vartab.h         Table for variable lookup stacked for subshell context 


Dumb charset conversion UCS, UTF-8, Latin 1

t
lib/ucs.h             


Periphery / Sensing

lib/gpio.h           Memory map Raspberry Pi GPIO pins, provide tri-state digital i/o 


Base function from fefe libowfat:

lib/array.h          dns      open     taia 
lib/buffer.h         examples scan     test 
lib/byte.h           fmt      socket   textcode 
lib/case.h           io       str      uint 
cdb mmap stralloc unix
critbit mult tai


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

pkgcfg.c             Drop-in pkg-config, prints compile and link flags for package 
impgen.c             Generate a .DEF file from a .DLL library 

ar-wrap.c            generic cross-compiler wrapper for the archiver (lib bundler) 
cc-wrap.c            generic cross-compiler wrapper for the front (preprocessor, compiler, assembler) 
compiler-wrapper.c   A compiler driver for Microchip PIC toolchains (picc,picc18,xc8,sdcc) 
cmake-run.c          find cross environment using dir and path functions and pass it to CMake 

genmakefile.c        Makefile / Ninja generator based on 

ar-wrap.c binfmttest.c bsdiffcat.c buffertest.c ccat.c cc-wrap.c cmake-run.c cofflist.c compiler-wrapper.c count-depth.c crc32.c decode-ls-lR.c dnsip.c dnsname.c dnstest.c eagle-gen-cmds.c eagle-init-brd.c eagle-to-circuit.c elf64list.c elflist.c elfwrsec.c genmakefile.c hexedit.c httptest.c impgen.c jsonpp.c jsontest.c list-r.c macho32list.c mediathek-list.c mediathek-parser.c msys-shell.c ntldd.c omflist.c opensearch-dump.c pathtool.c pelist.c pkgcfg.c plsconv.c rdir-test.c reg2cmd.c regfilter.c sln.c strarraytest.c tcping.c torrent-progress.c xml2json.c xml2moon.c xmlpp.c xmltest2.c xmltest3.c xmltest4.c xmltest.c ziptest.c intelhex.h libntldd.h files
.                    directories 

CC, CFLAGS compiler, flags

AR, ARFLAGS librarian
LINK, LDFLAGS linker, flags

-L and -l add library
-I and -D add include/definition


File Description

ntldd.c              recurse dynamic bindings of .exe files like /usr/bin/ldd on Linux 
pelist.c             List .EXE and .DLL file sections, import tables and export directories 
omflist.c            List OMF object files symbols 
cofflist.c           List COFF object files symbols 
elflist.c            List Elf32 headers, sections, segments, symbols and dynamic section 
elf64list.c          List Elf64 headers, sections, segments, symbols and dynamic section 
macho32list.c        List Mach-O segments and symbol table 


Compression and integrity

ccat.c               Compressed file cat with getopt() options and error reporting 
crc32.c              Calculate crc32 checksum, using zlib or diet version 


Network utilities

dnsip.c              Resolve hostname into IP addresses (NS from resolv.conf or DNSCACHE environment variable) 
dnsname.c            Reverse lookup of IP address 
tcping.c             try to TCP connect and output time of each acknowledge or reset 


Binary file tools

bsdiffcat.c          Translates a binary diff made with bsdiff into patch(buf, offset, ifval, thenval) calls 
hexedit.c            Memory map a file and process a set of patch data in the form: ADDRESS IF-BYTE SET-BYTE 
torrent-progress.c   memory map incomplete torrent data calculate percentage of pages which are all zero-bytes 


Media and streams

mediathek-list.c     HTTP load and filter mediathek-filme.json of the MediathekView application 
mediathek-parser.c   Parse a mediathek-filme.json store and generate a Playlist from it 

Windows registry

reg2cmd.c            Convert a windows registry .REG file into a .BAT with REG ADD commands 
regfilter.c          zombie copy reg2cmd, was intended a filtering version, like grepdiff 


Path helpers

pathtool.c           dropin for realpath, with --relative-to DIR feature and windows/msys support cygpath compliant 
sln.c                for a /usr/lib/libX.so.major.minor file pass one or more link names: libX.so.major libX.so /lib/libX.so 
msys-shell.c         Registry & path search for msys/msys2 environments and launch mintty and bash inside 

Utilities reading/writing XML/JSON

opensearch-dump.c    parse an OpenSearch XML file into an URL /search?query=%s to have them in Firefox/Chrome 
plsconv.c            Convert playlists between the formats .M3U .PLS .XSPF 

xmlpp.c              pretty-print XML/XHML 
jsonpp.c             Pretty-print JSON data 
xml2json.c           convert an element tree into json objects { tagName, ...attributes, children: [ ..innerElements 
xml2moon.c           convert XHTML content into lapis html generator syntax: element prop: val, prop2: .. 

eagle-gen-cmds.c     Read EAGLE XML schematics/layouts and generate commands to draw contour and x/y axis measures 
eagle-init-brd.c     Generate component arrangement congruent with Schematic and generate translate and rotate commands 
eagle-to-circuit.c   Read EAGLE XML packages and netlists, write .circuit file for striprouter 


Basic usage examples or boilerplates:

binfmttest.c         Test binary format readers 
buffertest.c         Test I/O buffer system, chaining with zlib/bzip2/lzma filter 
dnstest.c            Test DNS resolver 
httptest.c           Test HTTP client 
jsontest.c           Test JSON parser 
strarraytest.c       Test array of char* pointer functions 
xmltest.c            Test XML reading with callbacks (diet version of SAX) 
xmltest2.c           Test XML reading as tree (diet version of DOM) 
xmltest3.c           Test XML traversal and selection (diet version of getElementBy* and XPath) 
xmltest4.c           Test XML query 
ziptest.c            Simple universal file compression of *.gz, *.bz2, *.xz based on file extension 


