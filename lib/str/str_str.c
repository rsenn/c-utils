#include "../str.h"

/* Index of the first occurrence of needle in haystack, str_len(haystack) if
 * there is none (the str_chr convention); an empty needle matches at 0. */
size_t
str_str(const char* haystack, const char* needle) {
  size_t i, j;

  for(i = 0; haystack[i]; ++i) {
    for(j = 0; needle[j] && haystack[i + j] == needle[j]; ++j)
      ;
    if(!needle[j])
      return i;
    if(!haystack[i + j])
      break; /* the rest is too short to hold the needle */
  }
  return needle[0] ? i + str_len(haystack + i) : 0;
}
