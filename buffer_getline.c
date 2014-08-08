//#include <sys/types.h>
#include "buffer.h"

int buffer_getline(buffer* b,char* x,size_t len) {
  return buffer_get_token(b,x,len,"\n",1);
}
