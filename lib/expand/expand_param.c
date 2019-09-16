#include "../windoze.h"
#include "../vartab.h"
#include "../stralloc.h"
#include "../fmt.h"
#include "../str.h"
#include "../var.h"
#include "../fnmatch.h"
#include "../tree.h"
#include "../expand.h"
#include "../errmsg.h"

#if WINDOWS_NATIVE
#define getpid() GetProcessId(NULL)
#else
#include <unistd.h>
#endif

union node*
expand_param(
    struct nargparam* param, union node** nptr, struct vartab* varstack, char* argv[], int exitcode, int flags) {
  union node* n = *nptr;
  stralloc value;
  char* str = NULL;
  const char* v = NULL;
  int argc;
  unsigned long vlen = 0;

  for(argc = 0; argv[argc]; ++argc)
    ;
  stralloc_init(&value);

  /* treat special arguments */
  if(param->flag & S_SPECIAL) {
    switch(param->flag & S_SPECIAL) {
      /* $# substitution */
      case S_ARGC: {
        stralloc_catulong0(&value, argc, 0);
        break;
      }

      /* $* substitution */
      case S_ARGV: {
        char** s;

        for(s = argv; *s;) {
          stralloc_cats(&n->narg.stra, *s);
          if(*++s)
            stralloc_catc(&n->narg.stra, ' ');
        }
        break;
      }

      /* $@ substitution */
      case S_ARGVS: {
        int i = 0;

        while(i < argc) {
          param->flag &= ~S_SPECIAL;
          param->flag |= S_ARG;
          param->numb = 1 + i;

          n = expand_param(param, nptr, varstack, argv, exitcode, flags);

          if(++i < argc)
            nptr = &n->list.next;
        }

        return n;
      }

        /* $? substitution */
      case S_EXITCODE: {
        stralloc_catulong0(&value, exitcode, 0);
        break;
      }

      /* $- substitution */
      case S_FLAGS:
        break;

      /* $! substitution */
      case S_BGEXCODE:
        break;

        /* $[0-9] arg subst */
      case S_ARG: {
        if(param->numb == 0) {
          /*  stralloc_cats(&value, sh_argv0); */
        } else if(param->numb - 1 < argc) {
          stralloc_cats(&value, argv[param->numb - 1]);
        }
        break;
      }

        /* $$ arg subst */
      case S_PID: {
        stralloc_catulong0(&value, getpid(), 0);
        break;
      }
    }

    /* special parameters are always set */
    if(value.len) {
      stralloc_nul(&value);
      v = value.s;
    }

    vlen = value.len;
  }
  /* ..and variable substitutions */
  else {
    size_t offset;

    /* look for the variable.
       if the S_NULL flag is set and we have a var which is null
       set v to NULL */
    if((v = var_get(varstack, param->name, &offset))) {
      if(v[offset] == '\0' && (param->flag & S_NULL)) {
        v = NULL;
        vlen = 0;
      } else {
        v = &v[offset];
        vlen = str_len(v);
      }
    }
  }

  /* check for S_STRLEN substitution */
  if(param->flag & S_STRLEN) {
    char lstr[FMT_ULONG];

    n = expand_cat(lstr, fmt_ulong(lstr, vlen), nptr, varstack, flags);

    stralloc_free(&value);

    return n;
  }

  str = str_ndup(v, vlen);

  /* otherwise expand the apropriate variable/word subst */
  switch(param->flag & S_VAR) {
    /* return word if parameter unset (or null) */
    case S_DEFAULT: {
      if(v)
        n = expand_cat(v, vlen, nptr, varstack, flags);
      /* unset, substitute */
      else
        n = expand_arg(&param->word->narg, nptr, varstack, argv, exitcode, flags);
      break;
    }
    /* if parameter unset (or null) then expand word to it
       and substitute paramter */
    case S_ASGNDEF: {
      if(v)
        n = expand_cat(v, vlen, nptr, varstack, flags);
      else {
        n = expand_arg(&param->word->narg, nptr, varstack, argv, exitcode, flags | X_NOSPLIT);
        var_setvsa(param->name, /* BUG */ &n->narg.stra, V_DEFAULT);
      }
      break;
    }

    /* indicate error if null or unset */
    case S_ERRNULL: {
      if(v)
        n = expand_cat(v, vlen, nptr, varstack, flags);
      else {
        union node* tmpnode = NULL;

        n = expand_arg(&param->word->narg, &tmpnode, varstack, argv, exitcode, flags);
        errmsg_warn((n && n->narg.stra.s) ? n->narg.stra.s : "parameter null or not set", 0);
        if(tmpnode)
          tree_free(tmpnode);
      }
      break;
    }

      /* if parameter unset (or null) then substitute null,
         otherwise substitute word */
    case S_ALTERNAT: {
      if(v)
        n = expand_arg(&param->word->narg, nptr, varstack, argv, exitcode, flags);
      break;

        /* remove smallest matching suffix */
      case S_RSSFX: {
        int i;
        stralloc sa;

        if(v && vlen) {
          expand_copysa(param->word, &sa, varstack, argv, exitcode, 0);
          stralloc_nul(&sa);

          for(i = vlen - 1; i >= 0; i--)
            if(fnmatch(sa.s, str + i, FNM_PERIOD) == 0)
              break;

          n = expand_cat(v, (i < 0 ? (int)vlen : i), nptr, varstack, flags);
        }
        break;
      }
    }

      /* remove largest matching suffix */
    case S_RLSFX: {
      unsigned int i;
      stralloc sa;

      if(v && vlen) {
        expand_copysa(param->word, &sa, varstack, argv, exitcode, 0);
        stralloc_nul(&sa);

        for(i = 0; i <= vlen; i++)
          if(fnmatch(sa.s, str + i, FNM_PERIOD) == 0)
            break;

        n = expand_cat(v, (i > vlen ? vlen : i), nptr, varstack, flags);
      }

      break;
    }

      /* remove smallest matching prefix */
    case S_RSPFX: {
      unsigned int i;
      stralloc sa;

      if(v && vlen) {
        expand_copysa(param->word, &sa, varstack, argv, exitcode, 0);
        stralloc_nul(&sa);

        for(i = 1; i <= vlen; i++) {
          str_copyn(str, v, i);
          if(fnmatch(sa.s, (char*)v, FNM_PERIOD) == 0)
            break;
        }

        if(i > vlen)
          i = 0;

        n = expand_cat(v + i, vlen - i, nptr, varstack, flags);
        str_copy(str, v);
      }
      break;
    }

      /* remove largest matching prefix */
    case S_RLPFX: {
      unsigned int i;
      stralloc sa;

      if(v && vlen) {
        expand_copysa(param->word, &sa, varstack, argv, exitcode, 0);
        stralloc_nul(&sa);

        for(i = vlen; i > 0; i--) {
          str_copyn(str, v, i);
          if(fnmatch(sa.s, (char*)v, FNM_PERIOD) == 0)
            break;
        }

        if(i == 0)
          i = vlen;

        n = expand_cat(v + i, vlen - i, nptr, varstack, flags);
        str_copy(str, v);
      }
      break;
    }
  }

  free(str);

  stralloc_free(&value);
  return n;
}
