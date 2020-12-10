#include "../xml.h"
#include "../scan.h"
#include "../byte.h"

size_t
xml_scan_tag(const char* x, size_t len, size_t* toklen) {
  size_t i, j;
  i = (*x == '<') ? 1 : 0;
  if(i < len && x[i] == '/')
    i++;
  i += scan_noncharsetnskip(x + i, "\r\n\t />", len - i);
  if(i < len && x[i] == '/')
    i++;
  j = i + scan_whitenskip(x + i, len - i);
  if(x[j] == '>')
    j++;
  *toklen = i;
  return j;
}
