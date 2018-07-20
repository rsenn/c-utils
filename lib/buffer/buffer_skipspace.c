#include "../buffer.h"

int
buffer_skipspace(buffer* b) {
  char c;
  int n = 0;
  for(;;) {
   if(buffer_peekc(b, &c) < 0) return -1;
   if(!isspace(c)) break;
   buffer_skipc(b); 
   ++n; 
  }
  return n;
}
