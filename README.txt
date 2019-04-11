






    binfmt   abstract binary format (PE, Elf, Mach-O)
        cb   crit-bit trees
     cbmap   an associative map container based on crit-bittrees
   charbuf   an 1-byte character buffer
      coff   COFF object file inspection
       dir   portable directory entries reader (UNIX readdir and Windows FindFirstFile)
       elf   Elf32/Elf64 object, dynamically linked executables inspection
    expand   shell word expansion of ${vars}, special params and backticks command into string list
      gpio   raspberry pi GPIO pins
   hashmap   associative map container using hashed bucket algorithms
      hmap   hash map container with ownership on property of built-in type or custom *ptr,struct_size
      http   HTTP client model with requests, responses and headers, compression chains async reading via iopause
      json   JSON callback and tree parsers, tree iteration, token output
      list   doubly-linked list originated from ircd code list:{node*head,node*tail} node:{node*next,node*prev}
       map
       omf
      path operations on path strings
        pe PE32 / PE32+ executable format
  playlist generic playlist abstraction
   process exec and wait for processes
     range struct range is a tuple of { char*start,*end;size_t step }
      rdir recursive directory traversal on top of dir module
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
