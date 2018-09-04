#include <ctype.h>

#define var_isname(c) (isalpha((c)) || isdigit((c)) || (c) == '_')

/* check if it is a valid variable name
 * ----------------------------------------------------------------------- */
int
var_valid(const char* v) {
  register const char* s = v;

  if(isdigit(*v)) return 0;

  for(;;) {
    if(!var_isname(*s++)) return 0;
    if(!*s || *s == '=') return 1;
  }
}
