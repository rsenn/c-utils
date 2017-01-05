#include <ctype.h>
#include <stdlib.h>
#include "strlist.h"
#include "str.h"
#include "buffer.h"


typedef enum {
  COMPILE_ASSEMBLE_LINK = 0,
  PREPROCESS = 1,
  COMPILE = 2,
  COMPILE_AND_ASSEMBLE = 3,

} operation_mode;

const char const* opmode_strs[] = { "compile,assemble,link", "preprocess", "compile", "compile,assemble" };
static strlist args;
static   int argi, argn;

static operation_mode mode = COMPILE_ASSEMBLE_LINK;
static int debug = 0, warn = 0, dblbits = 24, ident_len = 31;
static strlist defines, includedirs, opts, longopts, params;
static stralloc output_dir, output_file;
static stralloc map_file, chip, optimization, runtime, debugger;

void
print_strlist(const strlist* sl, const char* s);

void
process_option(const char* optstr) {
  while(*optstr == '-')
    ++optstr;
  //buffer_puts(buffer_1, "optstr: ");   buffer_puts(buffer_1, optstr);  buffer_putnlflush(buffer_1);

  if(!str_diff(optstr, "pass1") || (str_len(optstr) == 1 && tolower(*optstr) == 'c')) {
    mode = COMPILE_AND_ASSEMBLE;
  } else if(!str_diff(optstr, "S") || (str_len(optstr) == 1 && toupper(*optstr) == 'S')) {
    mode = COMPILE;
  } else if(!str_diff(optstr, "P") || (str_len(optstr) == 1 && toupper(*optstr) == 'E')) {
    mode = PREPROCESS;
  } else if(!str_diffn(optstr, "outdir=", 7)) {
    stralloc_copys(&output_dir, &optstr[7]);
  } else if(!str_diffn(optstr, "chip=", 5)) {
    stralloc_copys(&chip, &optstr[5]);
  } else if(!str_diffn(optstr, "mode=", 5)) {
    //stralloc_copys(&license_mode, &optstr[5]);
  } else if(!str_diffn(optstr, "warn=" , 5)) {
    warn = atoi(&optstr[5]);
  } else if(!str_diffn(optstr, "opt=" , 4)) {
    stralloc_copys(&optimization, &optstr[4]);
  } else if(!str_diffn(optstr, "runtime=" ,8)) {
    stralloc_copys(&runtime, &optstr[8]);
  } else if(!str_diffn(optstr, "debugger=", 9)) {
    stralloc_copys(&debugger, &optstr[8]);
  } else if(tolower(*optstr) == 'o') {
    stralloc_copys(&output_file, &optstr[1]);
  } else if(tolower(*optstr) == 'm') {
    stralloc_copys(&map_file, &optstr[1]);
  } else if(tolower(*optstr) == 'g') {
    debug = 1;
  } else if(toupper(*optstr) == 'N') {
    ident_len = atoi(&optstr[1]);
  }
}

void
strlist_foreach(strlist* sl, void(*slptr)(const char*)) {
  int i, n = strlist_count(sl);;

  for(n = strlist_count(sl), i = 0; i < n; ++i) {
    slptr(strlist_at(sl, i));
  }
}


void
read_arguments() {
  strlist* sl = &args;

  argn = strlist_count(sl);;


  argi = 0;

  while(argi < argn) {
    const char* s = strlist_at(sl, argi);

    if(!str_diffn("-D", s, 2)) {  
      if(str_len(s) == 2 && argi + 1 < argn) {
        strlist_push(&defines, strlist_at(sl, ++argi));
      } else {
        strlist_push(&defines, &s[2]);
      }
    } else if(!str_diffn("-I", s, 2)) {
      if(str_len(s) == 2 && argi + 1 < argn) {
        strlist_push(&includedirs, strlist_at(sl, ++argi));
      } else {
        strlist_push(&includedirs, &s[2]);
      }
    } else if(!str_diffn("--", s, 2)) {
      strlist_push(&longopts, &s[2]);
    } else if(s[0] == '-') {
      strlist_push(&opts, &s[1]);
    } else {
      strlist_push(&params, s);
    }

    ++argi;
  }

  buffer_puts(buffer_1, "defines"); print_strlist(&defines, "\n\t");
  buffer_puts(buffer_1, "includedirs"); print_strlist(&includedirs, "\n\t");
  buffer_puts(buffer_1, "longopts"); print_strlist(&longopts, "\n\t");
  buffer_puts(buffer_1, "opts"); print_strlist(&opts, "\n\t");
  buffer_puts(buffer_1, "params"); print_strlist(&params, " ");

  strlist_foreach(&longopts, process_option);
  strlist_foreach(&opts, process_option);

  buffer_puts(buffer_1, "mode: ");
  buffer_puts(buffer_1,   opmode_strs[mode]);
  buffer_putnlflush(buffer_1);

  buffer_puts(buffer_1, "output file: "); buffer_putsa(buffer_1, &output_file); buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "output dir: "); buffer_putsa(buffer_1, &output_dir); buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "chip: "); buffer_putsa(buffer_1, &chip); buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "map file: "); buffer_putsa(buffer_1, &map_file); buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "warn: "); buffer_putlong(buffer_1, warn); buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "double bits: "); buffer_putlong(buffer_1, dblbits); buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "identifier length: "); buffer_putlong(buffer_1, ident_len); buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "optimization: "); buffer_putsa(buffer_1, &optimization); buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "runtime: "); buffer_putsa(buffer_1, &runtime); buffer_putnlflush(buffer_1);
 buffer_puts(buffer_1, "debugger: "); buffer_putsa(buffer_1, &debugger); buffer_putnlflush(buffer_1);

}

void
print_strlist(const strlist* sl, const char* separator) {
  size_t n = strlist_count(sl);
  buffer_puts(buffer_1, " (#");
  buffer_putlong(buffer_1, n);
  buffer_puts(buffer_1, "):\n\n\t");
  for(int i = 0; i < n; ++i) {
    if(i > 0)
      buffer_puts(buffer_1, separator);
    buffer_puts(buffer_1, strlist_at(sl, i));


  }
  buffer_puts(buffer_1, "\n\n");
  buffer_flush(buffer_1);
}

int
main(int argc, char* argv[]) {

  strlist_init(&args);

  strlist_init(&defines);
  strlist_init(&includedirs);
  strlist_init(&longopts);
  strlist_init(&opts);
  strlist_init(&args);

  for(int i = 1; i < argc; ++i) {
    strlist_push(&args, argv[i]);
  }
  read_arguments();
  //print_strlist(&args);

  return 0;
}
