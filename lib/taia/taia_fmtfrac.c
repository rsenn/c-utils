#include "../taia.h"

/* the 18 digit fraction of t (9 digits nano, 9 digits atto), leading zeros,
 * no decimal point and no terminating NUL; s may be NULL */
unsigned int
taia_fmtfrac(char* s, const tai6464* t) {
  unsigned long n;
  int i;

  if(s) {
    n = t->atto;
    for(i = 17; i >= 9; --i) {
      s[i] = '0' + (char)(n % 10);
      n /= 10;
    }
    n = t->nano;
    for(i = 8; i >= 0; --i) {
      s[i] = '0' + (char)(n % 10);
      n /= 10;
    }
  }
  return 18;
}
