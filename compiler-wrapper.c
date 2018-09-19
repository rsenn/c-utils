#include "lib/windoze.h"

#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

#if WINDOWS_NATIVE
#include <process.h>
#define mkdir(file,mode) mkdir(file)
#else
#include <sys/wait.h>
#endif
#include <sys/stat.h>

#include "lib/strlist.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/buffer.h"
#include "lib/dir_internal.h"

#define mytolower(c) ((c)>='A'&&(c)<='Z'?(c)+0x20:(c))

static char* argv0;
static buffer* debug_buf, *err_buf;

typedef enum {
  SDCC = 0,
  PICC = 1,
  PICC18 = 2,
  XC8 = 3,
} compiler_type;
const char* compiler_strs[] = { "sdcc", "picc", "picc18", "xc8" };

typedef enum {
  COMPILE_ASSEMBLE_LINK = 0,
  PREPROCESS = 1,
  COMPILE = 2,
  COMPILE_AND_ASSEMBLE = 3,

} operation_mode;

const char* opmode_strs[] = { "compile,assemble,link", "preprocess", "compile", "compile,assemble" };
static strlist args;
static int i, n;

static compiler_type type;
static operation_mode mode = COMPILE_ASSEMBLE_LINK;
static int debug = 0, warn = 0, fltbits = 0, dblbits = 0, ident_len = 127, optlevel = 0, optsize = 0;
static strlist defines, includedirs, opts, longopts, params;
static stralloc output_dir, output_file;
static stralloc map_file, chip, optimization, runtime, debugger;
static stralloc err_format, warn_format, msg_format;
static  stralloc compiler;

void
dump_stralloc(const char* name, const stralloc* out) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, "=\"");
  buffer_putsa(buffer_2, out);
  buffer_puts(buffer_2, "\"\n");
  buffer_flush(buffer_2);
}

void
dump_str(const char* name, const char* s) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, "=\"");
  buffer_puts(buffer_2, s ? s : "(null)");
  buffer_puts(buffer_2, "\"\n");
  buffer_flush(buffer_2);
}
#define DUMP_STRALLOC(n) dump_stralloc(#n, &n)
#define DUMP_STR(s) dump_str(#s, s)

int
get_compiler_dir(const char* basedir, stralloc* out) {
  struct dir_s d;
  char* name;
  struct stat st;

  if(dir_open(&d, basedir) != 0)
    return -1;

  while((name = dir_read(&d))) {
    int dtype = dir_type(&d);

    if(dtype != D_DIRECTORY) continue;
    if(name[0] == '.') continue;

    stralloc_copys(out, basedir);
    stralloc_catb(out, "/", 1);
    stralloc_cats(out, name);
    stralloc_0(out);

    if(stat(out->s, &st) == 0)
      return 0;
  }

  dir_close(&d);
  return 0;
}

int
get_machine(const stralloc* chip, stralloc* mach) {
  if(chip->len < 3 || chip->s[0] != '1')
    return -1;

  stralloc_copys(mach, chip->s[1] > '6' ? "pic16" : "pic14");
  return 0;
}

void
print_strlist(buffer*, const strlist* sl, const char* sep, const char* quot);

int
process_option(const char* optstr, const char*  nextopt, int* i) {
  while(*optstr == '-')
    ++optstr;
  //buffer_puts(debug_buf, "optstr: ");   buffer_puts(debug_buf, optstr);  buffer_putnlflush(debug_buf);

  if(!str_diff(optstr, "outdir")) {
    stralloc_copys(&output_dir, nextopt);
    ++*i;
    return 0;
  } else if(!str_diff(optstr, "pass1") || (str_len(optstr) == 1 && mytolower(*optstr) == 'c')) {
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
  } else if(!str_diffn(optstr, "warn=", 5)) {
    warn = atoi(&optstr[5]);
  } else if(!str_diffn(optstr, "float=", 6)) {
    fltbits = atoi(&optstr[6]);
  } else if(!str_diffn(optstr, "double=", 7)) {
    dblbits = atoi(&optstr[7]);
  } else if(!str_diffn(optstr, "opt=", 4)) {
    stralloc_copys(&optimization, &optstr[4]);
  } else if(!str_diffn(optstr, "runtime=", 8)) {
    stralloc_copys(&runtime, &optstr[8]);
  } else if(!str_diffn(optstr, "debugger=", 9)) {
    stralloc_copys(&debugger, &optstr[9]);
  } else if(!str_diffn(optstr, "errformat=", 10)) {
    stralloc_copys(&err_format, &optstr[10]);
  } else if(!str_diffn(optstr, "warnformat=", 11)) {
    stralloc_copys(&warn_format, &optstr[11]);
  } else if(*optstr == 'p') {
    stralloc_copys(&chip, &optstr[1]);
  } else if(*optstr == 'W') {
    if(!str_diff(&optstr[1], "all")) warn = 9;
    else warn = 5;
  } else if(*optstr == 'w') {
    warn = 0;
  } else if(*optstr == 'O') {
    if(optstr[1] == 's')
      optsize = 1;
    else
      optlevel = atoi(&optstr[1]) * 3;
  } else if(*optstr == 'o') {

    stralloc_copys(&output_file, &optstr[1]);
  } else if(tolower(*optstr) == 'm') {
    stralloc_copys(&map_file, &optstr[1]);
  } else if(tolower(*optstr) == 'g') {
    debug = 1;
  } else if(toupper(*optstr) == 'N') {
    ident_len = atoi(&optstr[1]);
  } else {
    return 1;
  }
  return 0;
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
  char* p = av[0];
  av[0] = str_basename(p);

#if WINDOWS_NATIVE
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
  return -1;
}

void
read_arguments() {
  strlist* sl = &args;
  n = strlist_count(sl);
  i = 0;

  for(i = 0; i < n; ++i) {
    char* s = strlist_at(sl, i);
    char* s2 = i + 1 < n ? strlist_at(sl, i + 1) : NULL;

    if(!str_diffn("-D", s, 2)) {
      if(str_len(s) == 2 && i + 1 < n) {
        strlist_push_unique(&defines, s2);
        ++i;
      } else {
        strlist_push_unique(&defines, &s[2]);
      }
    } else if(!str_diffn("-I", s, 2)) {
      if(str_len(s) == 2 && i + 1 < n) {
        strlist_push_unique(&includedirs, s2);
        ++i;
      } else {
        strlist_push_unique(&includedirs, &s[2]);
      }
    }  else if(!str_diffn("-t", s, 2)) {
      if(str_len(s) == 2 && i + 1 < n) {
        argv0 = s2;
        ++i;
      } else {
        argv0 = &s[2];
      }
      if(strchr(argv0, '/'))
        stralloc_copys(&compiler, argv0);
      argv0 = str_dup(str_basename(argv0));
    }  else if(!str_diffn("-o", s, 2)) {
      stralloc output;
      stralloc_init(&output);
      if(str_len(s) == 2 && i + 1 < n) {
        stralloc_copys(&output, s2);
        ++i;
      } else {
        stralloc_copys(&output, &s[2]);
      }
      stralloc_0(&output);
      stralloc_copys(&output_file, str_basename(output.s));
      if(strchr(output.s, '/') || strchr(output.s, '\\')) {
        stralloc_copys(&output_dir, output.s);
  	output_dir.len = str_rchr(output.s, '/');
      }
      continue;
    } else if(!str_diffn("--", s, 2)) {
      if(process_option(s, s2, &i))
        strlist_push(&longopts, s);
    } else if(s[0] == '-') {

      if(process_option(s, s2, &i))
        strlist_push(&opts, s);
    } else {
      strlist_push(&params, s);
    }
  }

  if(!chip.len) stralloc_copys(&chip, "16f876a");

  DUMP_STRALLOC(compiler);

  dump_str("mode", opmode_strs[mode]);

  if(err_format.len == 0) stralloc_copys(&err_format, "\n%f:%l: error: (%n) %s");
  if(warn_format.len == 0) stralloc_copys(&warn_format, "\n%f:%l: warning: (%n) %s");
  if(msg_format.len == 0) stralloc_copys(&msg_format, "\n%f:%l: advisory: (%n) %s");

  if(!str_case_diffn(argv0, "sdcc", 4)) {
    type = SDCC;
    if(compiler.len == 0) stralloc_copys(&compiler, "C:/Program Files/SDCC");
  } else if(!str_case_diffn(argv0, "picc18", 6)) {
    type = PICC18;
    if(compiler.len == 0) get_compiler_dir("C:/Program Files (x86)/HI-TECH Software/PICC18", &compiler);
  } else if(!str_case_diffn(argv0, "picc", 4)) {
    type = PICC;
    if(compiler.len == 0) get_compiler_dir("C:/Program Files (x86)/HI-TECH Software/PICC", &compiler);
  } else if(strstr(argv0, "xc8") != NULL) {
    type = XC8;
    if(compiler.len == 0) get_compiler_dir("C:/Program Files (x86)/Microchip/xc8", &compiler);
  }

  stralloc_0(&compiler);
  if(strstr(compiler.s, "/bin") == NULL)
    stralloc_cats(&compiler, "/bin/");

  stralloc_0(&compiler);
  if(strstr(compiler.s, "/bin/") == NULL)
    stralloc_cats(&compiler, compiler_strs[type]);

  dump_stralloc("compiler", &compiler);
  if(output_file.len == 0 && (mode == COMPILE_AND_ASSEMBLE || mode == COMPILE || mode == PREPROCESS)) {
    size_t n;
    stralloc_copys(&output_file, str_basename(strlist_at(&params, 0)));

    n = byte_rchr(output_file.s, output_file.len, '.');
    if(n < output_file.len) {
      output_file.len = n + 1;

      switch(mode) {
        case PREPROCESS:
          stralloc_cats(&output_file, (type == SDCC) ? "e" : "pre");
          break;
        case COMPILE_AND_ASSEMBLE:
          stralloc_cats(&output_file, (type == SDCC) ? "o" : "p1");
          break;
        case COMPILE:
          stralloc_cats(&output_file, (type == SDCC) ? "s" : "as");
          break;
        default:
          break;
      }
    }
  }

  if(debug) {
    strlist_push_unique(&defines, "DEBUG=1");
    strlist_push_unique(&defines, "__DEBUG=1");
  } else {
    strlist_push_unique(&defines, "NDEBUG=1");
    strlist_push_unique(&defines, "__NDEBUG=1");
  }

  if(optsize) {
    if(optlevel == 0) optlevel = 9;
  }

#define DUMP_LIST(buf,n,sep,q)   buffer_puts(buf, #n); print_strlist(buf, &n, sep, q);
#define DUMP_VALUE(n,fn,v)   buffer_puts(debug_buf, n ": "); fn(debug_buf, v); buffer_putnlflush(debug_buf);

  DUMP_LIST(debug_buf, defines, "\n\t", "");
  DUMP_LIST(debug_buf, includedirs, "\n\t", "");
  DUMP_LIST(debug_buf, longopts, "\n\t", "");
  DUMP_LIST(debug_buf, opts, "\n\t", "");
  DUMP_LIST(debug_buf, params, "\n\t", "");

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
}

void
execute_cmd() {
 
size_t i,n;

  strlist cmd;
  strlist_init(&cmd, '\0');

  strlist_cat(&cmd, &opts);
  stralloc_0(&chip);
  strlist_cat(&cmd, &longopts);

  n = strlist_count(&includedirs);
  for(i = 0; i < n; ++i) {
    strlist_pushm(&cmd, "-I", strlist_at(&includedirs, i));
  }

  n = strlist_count(&defines);
  for(i = 0; i < n; ++i) {
    strlist_pushm(&cmd, "-D", strlist_at(&defines, i));
  }
  if(type == PICC || type == PICC18 || type == XC8) {
          char nbuf[FMT_ULONG];

    strlist_pushm(&cmd, "--chip=", chip.s, 0);

if(mode != PREPROCESS) {

    if(runtime.len > 0) {
      stralloc_0(&runtime); strlist_pushm(&cmd, "--runtime=", runtime.s, 0);
    }
    if(optimization.len > 0) {
      stralloc_0(&optimization); strlist_pushm(&cmd, "--opt=", optimization.s, 0);
    }
    if(debugger.len > 0) {
      stralloc_0(&debugger); strlist_pushm(&cmd, "--debugger=", debugger.s, 0);
    }
    if(err_format.len > 0) {
      stralloc_0(&err_format); strlist_pushm(&cmd, "--errformat=", err_format.s, 0);
    }
    if(warn_format.len > 0) {
      stralloc_0(&warn_format); strlist_pushm(&cmd, "--warnformat=", warn_format.s, 0);
    }
    if(msg_format.len > 0) {
      stralloc_0(&msg_format); strlist_pushm(&cmd, "--msgformat=", msg_format.s, 0);
    }
    strlist_push(&cmd, "--mode=PRO");

    if(optlevel) {
       nbuf[fmt_ulong(nbuf, optlevel)] = '\0';
      strlist_pushm(&cmd, "--opt=default,+asm,", debug ? "+debug,":"", optsize ? "-speed,+space,":"-space,+speed,", nbuf, NULL);
    }

     if(warn) {
       nbuf[fmt_ulong(nbuf, warn)] = '\0';
      strlist_pushm(&cmd, "--warn=",nbuf,NULL);
    }
    if(debug) strlist_push(&cmd, "-G");

    if(ident_len != 0) {
      nbuf[fmt_ulong(nbuf, ident_len)] = '\0';
      strlist_pushm(&cmd, "-N",nbuf, NULL);
    }
    if(fltbits != 0) {
      nbuf[fmt_ulong(nbuf, fltbits)] = '\0';
      strlist_pushm(&cmd, "--float=",nbuf, NULL);
    }
    if(dblbits != 0) {
      nbuf[fmt_ulong(nbuf, dblbits)] = '\0';
      strlist_pushm(&cmd, "--double=",nbuf, NULL);
    }

  }

    switch(mode) {
      case PREPROCESS: {
          strlist_push(&cmd, "--pre");
          break;
        }
      case COMPILE: {
          strlist_push(&cmd, "-S");
          break;
        }
      case COMPILE_AND_ASSEMBLE: {
          strlist_push(&cmd, "--pass1");
          break;
        }
      default: {
          break;
        }
    }

    if(output_file.len > 0) {
      stralloc outf;
      stralloc_init(&outf);

      if(output_dir.len > 0) {
        stralloc_copy(&outf, &output_dir);
        stralloc_cats(&outf, "/");
      }
      stralloc_cat(&outf, &output_file);
      stralloc_0(&outf);
      strlist_pushm(&cmd, "-O", outf.s, 0);
    } else if(output_dir.len > 0) {
      stralloc_0(&output_dir);
      strlist_pushm(&cmd, "--outdir=", output_dir.s, 0);
    }

  } else if(type == SDCC) {
    stralloc outp, machine;
    stralloc_init(&outp);
    stralloc_init(&machine);

    strlist_push(&cmd, "--use-non-free");

    if(get_machine(&chip, &machine) == 0) {
      stralloc_0(&machine);
      strlist_pushm(&cmd, "-m", machine.s, 0);
    }

    strlist_pushm(&cmd, "-p", chip.s, 0);

    switch(mode) {
      case PREPROCESS: {
          strlist_push(&cmd, "-E");
          break;
        }
      case COMPILE: {
          strlist_push(&cmd, "-S");
          break;
        }
      case COMPILE_AND_ASSEMBLE: {
          strlist_push(&cmd, "-c");
          break;
        }
      default: {
          break;
        }
    }

    if(debug) {
      strlist_pushm(&cmd, "--debug", "--debug-xtra", 0);
    }
    if(output_dir.len > 0) {
      stralloc_copy(&outp, &output_dir);
    }
    if(output_file.len > 0) {
      if(outp.len > 0)
        stralloc_cats(&outp, "/");
      stralloc_cat(&outp, &output_file);
    }
    if(outp.len > 0) {
      stralloc_0(&outp);
      strlist_push(&cmd, "-o");
      strlist_push_sa(&cmd, &outp);
    }
  }

  stralloc_0(&compiler);
  strlist_unshift(&cmd, compiler.s); //"C:\\Program Files (x86)\\Microchip\\xc8\\v1.34\\bin\\xc8.exe");

  strlist_cat(&cmd, &params);
  DUMP_LIST(err_buf, cmd, " ", "\"")

  if(output_dir.len > 0) {
    struct stat st;
    stralloc_0(&output_dir);
    if(stat(output_dir.s, &st) == -1)
      mkdir(output_dir.s, 0755);
  }

  if(strlist_execve(&cmd) == -1) {
    buffer_puts(debug_buf, "ERROR: ");
    buffer_puts(debug_buf, strerror(errno));
    buffer_putnlflush(debug_buf);
  }
}

void
print_strlist(buffer* b, const strlist* sl, const char* separator, const char* quot) {
  ssize_t n = strlist_count(sl);
  int i, need_quote;
  buffer_puts(b, " (#");
  buffer_putlong(b, n);
  buffer_puts(b, "):");
  buffer_puts(b, separator);

  for(i = 0; i < n; ++i) {
    const char* s = strlist_at(sl, i);

    if(str_len(s)) {

      if(i > 0)
        buffer_puts(b, separator);

      need_quote = 0;

      if(str_len(quot)) {
        while(*s) {
          if(*s == ' ' || *s == '"' || *s == '\'' || *s == '\'' || *s == '\n') {
            need_quote = 1;
            break;
          }
          ++s;
        }
      }

      s = strlist_at(sl, i);

      if(need_quote) buffer_puts(b, quot);

      while(*s) {
        if(*s == '\n') buffer_puts(b, "\\n");
        else buffer_put(b, s, 1);
        ++s;
      }
      if(need_quote) buffer_puts(b, quot);
    }

  }
  buffer_puts(b, "\n");
  buffer_flush(b);
}

int
main(int argc, char* argv[]) {
   int i;

  argv0 = str_basename(argv[0]);

  debug_buf = buffer_1;
  err_buf = buffer_2;

#if NDEBUG == 1
  debug_buf->fd = open("/dev/null", O_WRONLY);
#endif
stralloc_init(&output_dir);
stralloc_init(&output_file);
stralloc_init(&map_file);
stralloc_init(&chip);
stralloc_init(&optimization);
stralloc_init(&runtime);
stralloc_init(&debugger);
stralloc_init(&err_format);
stralloc_init(&warn_format);
stralloc_init(&msg_format);

strlist_init(&args, '\0');

  for(i = 1; i < argc; ++i) {
    strlist_push(&args, argv[i]);
  }
  DUMP_LIST(err_buf, args, "\n\t", "'");
  read_arguments();
  execute_cmd();

  return 0;
}
