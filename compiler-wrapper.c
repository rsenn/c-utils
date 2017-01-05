#include <ctype.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#ifdef __MINGW32__
#include <process.h>
#else
#include <sys/wait.h>
#endif

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
static int argi, argn;

static operation_mode mode = COMPILE_ASSEMBLE_LINK;
static int debug = 0, warn = 0, dblbits = 24, ident_len = 31;
static strlist defines, includedirs, opts, longopts, params;
static stralloc output_dir, output_file;
static stralloc map_file, chip, optimization, runtime, debugger;
static stralloc err_format, warn_format;

void
print_strlist(const strlist* sl, const char* s);

int
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
    stralloc_copys(&debugger, &optstr[9]);
  } else if(!str_diffn(optstr, "errformat=", 10)) {
    stralloc_copys(&err_format, &optstr[10]);
  } else if(!str_diffn(optstr, "warnformat=", 11)) {
    stralloc_copys(&warn_format, &optstr[11]);
  } else if(tolower(*optstr) == 'o') {
    stralloc_copys(&output_file, &optstr[1]);
  } else if(tolower(*optstr) == 'm') {
    stralloc_copys(&map_file, &optstr[1]);
  } else if(tolower(*optstr) == 'g') {
    debug = 1;
  } else if(toupper(*optstr) == 'N') {
    ident_len = atoi(&optstr[1]);
  } else {
    return 0;
  }
  return 1;
}

void
strlist_foreach(strlist* sl, void(*slptr)(const char*)) {
  int i, n = strlist_count(sl);;

  for(n = strlist_count(sl), i = 0; i < n; ++i) {
    slptr(strlist_at(sl, i));
  }
}

int
strlist_execve(const strlist* sl) {
  char** av = strlist_to_argv(sl);
  const char* p = av[0];
    av[0] = basename(p);
  
#ifdef __MINGW32__
return spawnv(P_WAIT, p, av);
#else
  int pid = vfork();

  if(pid == -1)
    return -1;

  if(pid == 0) {

    if(execv(p, av) == -1)
      exit(127);
  } else { 
    int status = 0;
     if(waitpid(pid, &status, 0) == -1)
      return 127;

     return status;
  }
  #endif
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
      if(!process_option(s))
        strlist_push(&longopts, s);
    } else if(s[0] == '-') {
      if(!process_option(s))
        strlist_push(&opts, s);
    } else {
      strlist_push(&params, s);
    }

    ++argi;
  }

  #define DUMP_LIST(n,sep)   buffer_puts(buffer_1, #n); print_strlist(&n, sep);
  #define DUMP_VALUE(n,fn,v)   buffer_puts(buffer_1, n ": "); fn(buffer_1, v); buffer_putnlflush(buffer_1);

  DUMP_LIST(defines, "\n\t");
  DUMP_LIST(includedirs, "\n\t");
  DUMP_LIST(longopts, "\n\t");
  DUMP_LIST(opts, "\n\t");
  DUMP_LIST(params, "\n\t");

//  strlist_foreach(&longopts, process_option);
//  strlist_foreach(&opts, process_option);

  DUMP_VALUE("output file", buffer_putsa, &output_file);
  DUMP_VALUE("mode", buffer_puts, opmode_strs[mode]);
  DUMP_VALUE("chip", buffer_putsa, &chip);
  DUMP_VALUE("output dir", buffer_putsa, &output_dir);
  DUMP_VALUE("map file", buffer_putsa, &map_file);
  DUMP_VALUE("optimization", buffer_putsa, &optimization);
  DUMP_VALUE("runtime", buffer_putsa, &runtime);
  DUMP_VALUE("debugger", buffer_putsa, &debugger);

  DUMP_VALUE("warn", buffer_putlong, warn);
  DUMP_VALUE("dblbits", buffer_putlong, dblbits);
  DUMP_VALUE("ident len", buffer_putlong, ident_len);

strlist cmd;
strlist_init(&cmd);


//  strlist_unshift(&cmd, "-v");

strlist_copy(&cmd, &opts);
  
  stralloc_0(&chip);  strlist_pushm(&cmd, "--chip=", chip.s, 0);
  if(runtime.len > 0) { stralloc_0(&runtime);  strlist_pushm(&cmd, "--runtime=", runtime.s, 0); }
  if(optimization.len > 0) { stralloc_0(&optimization);  strlist_pushm(&cmd, "--opt=", optimization.s, 0); }
  if(debugger.len > 0) {  stralloc_0(&debugger);  strlist_pushm(&cmd, "--debugger=", debugger.s, 0);  }
  strlist_push(&cmd, "--mode=PRO");

  strlist_copy(&cmd, &longopts);

  size_t i, n = strlist_count(&includedirs);
  for(i = 0; i < n; ++i) {
    strlist_pushm(&cmd, "-I", strlist_at(&includedirs, i));
  }

  n = strlist_count(&defines);
  for(i = 0; i < n; ++i) {
    strlist_pushm(&cmd, "-D", strlist_at(&defines, i));
  }

  switch(mode) {
    case PREPROCESS: { strlist_push(&cmd, "-P"); break;  }
    case COMPILE: { strlist_push(&cmd, "-S"); break;  }
    case COMPILE_AND_ASSEMBLE: { strlist_push(&cmd, "--pass1"); break; }
    default: { break; }
  //  case COMPILE_ASSEMBLE_LINK: { break; }
  }


if(output_dir.len > 0) {
  stralloc_0(&output_dir);
  strlist_pushm(&cmd, "--outdir=", output_dir.s, 0);
}

  strlist_unshift(&cmd, "C:\\Program Files (x86)\\Microchip\\xc8\\v1.34\\bin\\xc8.exe");

  strlist_copy(&cmd, &params);
  DUMP_LIST(cmd, "\n\t")

  if(strlist_execve(&cmd) == -1) {
    buffer_puts(buffer_1, "ERROR: ");
    buffer_puts(buffer_1, strerror(errno));
    buffer_putnlflush(buffer_1);
    
  }
}

void
print_strlist(const strlist* sl, const char* separator) {
  size_t n = strlist_count(sl);
  buffer_puts(buffer_1, " (#");
  buffer_putlong(buffer_1, n);
  buffer_puts(buffer_1, "):\n\n\t");
  for(int i = 0; i < n; ++i) {
    const char* s = strlist_at(sl, i);

    if(str_len(s)) { 

      if(i > 0)
        buffer_puts(buffer_1, separator);
      buffer_puts(buffer_1, strlist_at(sl, i));
    }

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
