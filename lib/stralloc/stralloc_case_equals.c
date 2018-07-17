#include "../stralloc.h"

unsigned int stralloc_case_equals(const stralloc *sa, const char *s) {
  return stralloc_case_diffs(sa, s) == 0;
}
