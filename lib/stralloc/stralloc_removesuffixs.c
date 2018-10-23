#include "../stralloc.h"

int
stralloc_removesuffixs(stralloc* sa, const char* suffix) {
  return stralloc_removesuffixb(sa, suffix, str_len(suffix));
}
