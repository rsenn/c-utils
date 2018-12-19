#ifndef EXPAND_H
#define EXPAND_H

#include "stralloc.h"

#define IFS_DEFAULT " \t\n"

#define S_TABLE 0x0003
#define S_UNQUOTED (0 << 0)
#define S_DQUOTED (1 << 0)
#define S_SQUOTED (2 << 0)
#define S_ARITH (3 << 0)

#define S_BQUOTE (1 << 2)

/* substitution types */
#define S_SPECIAL 0x00f8
#define S_ARGC (1 << 3)     /* $# */
#define S_ARGV (2 << 3)     /* $* */
#define S_ARGVS (3 << 3)    /* $@ */
#define S_EXITCODE (4 << 3) /* $? */
#define S_FLAGS (5 << 3)    /* $- */
#define S_BGEXCODE (6 << 3) /* $! */
#define S_ARG (7 << 3)      /* $[0-9] */
#define S_PID (8 << 3)      /* $$ */

#define S_VAR 0x0f00
#define S_DEFAULT (0 << 8)  /* ${parameter:-word} */
#define S_ASGNDEF (1 << 8)  /* ${parameter:=word} */
#define S_ERRNULL (2 << 8)  /* ${parameter:?[word]} */
#define S_ALTERNAT (3 << 8) /* ${parameter:+word} */
#define S_RSSFX (4 << 8)    /* ${parameter%word} */
#define S_RLSFX (5 << 8)    /* ${parameter%%word} */
#define S_RSPFX (6 << 8)    /* ${parameter#word} */
#define S_RLPFX (7 << 8)    /* ${parameter##word} */

#define S_STRLEN 0x1000
#define S_NULL 0x2000 /* treat set but null as unset (:) */
#define S_NOSPLIT 0x4000
#define S_ESCAPED 0x8000
/* a char within here-doc delim is escaped */
#define S_GLOB 0x10000

/* expansion modes */
/*#define X_SPLIT       0x00*/
#define X_NOSPLIT 0x01

/*#define X_NOESCAPE    0x00*/
#define X_ESCAPE 0x02

/*#define X_NOGLOB      0x00*/
#define X_GLOB 0x04

/*#define X_UNQUOTED    0x00*/
#define X_QUOTED 0x08

extern char expand_ifs[4];

union node;
struct narg;

void expand_argv(union node*, char** argv);
union node* expand_arith(struct nargarith*, union node** nptr, int flags);
void expand_escape(stralloc*, const char* b, unsigned int n);
void expand_unescape(stralloc*);

#ifdef VARTAB_H
union node* expand_arg(struct narg*, union node** nptr, struct vartab*, char* argv[], int exitcode, int flags);
union node* expand_cat(const char*, unsigned int, union node** nptr, struct vartab*, int flags);
void expand_catsa(union node*, stralloc*, struct vartab*, char* argv[], int exitcode, int flags);
union node* expand_glob(union node**, struct vartab* varstack, int flags);
union node* expand_param(struct nargparam*, union node**, struct vartab*, char* argv[], int exitcode, int flags);
int expand_vars(union node*, union node**, struct vartab*, char* argv[], int exitcode);
int expand_args(union node*, union node**, struct vartab*, char* argv[], int exitcode, int flags);
void expand_copysa(union node*, stralloc*, struct vartab*, char* argv[], int exitcode, int flags);
union node* expand_command(struct nargcmd*, union node**, struct vartab*, int flags);
#endif

#endif /* EXPAND_H */
