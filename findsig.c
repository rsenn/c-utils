#include "lib/alloc.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/errmsg.h"
#include "lib/open.h"
#include "lib/path.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/seek.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define PRINT_EXTRA

int
is_hex_char(char c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

uint8_t*
pat2bytes(char* pat_str, int* pat_len) {
  int pat_str_len = str_len(pat_str);
  uint8_t* pat_buf = alloc(pat_str_len);

  int nbytes = 0;

  int i = 0;
  while(i < pat_str_len) {
    char c = pat_str[i];
    if(c == '?') {
      pat_buf[nbytes] = 0;
      nbytes++;
      i++;
    } else if(is_hex_char(c)) {
      sscanf(pat_str + i, "%2hhx", pat_buf + nbytes);
      nbytes++;
      i += 2;
    } else {
      i++;
    }
  }
  alloc_re(&pat_buf, 0, nbytes);
  *pat_len = nbytes;
  return pat_buf;
}

char*
pat2mask(char* pat_str, int* mask_len) {
  int pat_str_len = str_len(pat_str);
  char* mask = alloc(pat_str_len);

  int nbytes = 0;

  int i = 0;
  while(i < pat_str_len) {
    char c = pat_str[i];
    if(c == '?') {
      mask[nbytes] = '?';
      nbytes++;
      i++;
    } else if(is_hex_char(c)) {
      mask[nbytes] = 'x';
      nbytes++;
      i += 2;
    } else {
      i++;
    }
  }
  alloc_re(&mask, 0, nbytes);
  *mask_len = nbytes;
  return mask;
}

size_t
get_file_size(FILE* f) {
  fseek(f, 0, SEEK_END);
  size_t sz = ftell(f);
  rewind(f);
  return sz;
}

uint8_t*
read_file(FILE* f, size_t* file_sz) {
  size_t sz = get_file_size(f);
  uint8_t* buf = malloc(sz);
  fread(buf, sz, 1, f);
  *file_sz = sz;
  return buf;
}

long
find_sig(uint8_t* file_buf, size_t file_sz, uint8_t* pat_buf, int pat_len, char* mask) {
  if(pat_len > file_sz) {
    return -1;
  }

  // naive searching algorithm (slow)
  for(long i = 0; i < file_sz; i++) {
    int found = 1;
    for(int j = 0; j < pat_len; j++) {
      if(mask[j] != '?' && file_buf[i + j] != pat_buf[j]) {
        found = 0;
        break;
      }
    }
    if(found) {
      return i;
    }
  }

  return -1;
}

int
main(int argc, char** argv) {
  if(argc < 3) {
    printf("usage: ./findsig [file] [pattern]\n");
    printf("\tpattern format example: \"05 ff 0a bb ? ? 12 34 56 ? ?\"\n");
    printf("\taddress returned is at the START of the pattern\n");
    return 1;
  }

  FILE* f = fopen(argv[1], "rb");
  if(!f) {
    printf("fatal: file could not be opened\n");
    return 1;
  }

  size_t file_sz = 0;
  uint8_t* file_buf = read_file(f, &file_sz);

  int pat_len = 0;
  uint8_t* pat_buf = pat2bytes(argv[2], &pat_len);

  int mask_len = 0;
  char* mask = pat2mask(argv[2], &mask_len);

#ifdef PRINT_EXTRA
  printf("mask: ");
  for(int i = 0; i < mask_len; i++) {
    putchar(mask[i]);
  }
  putchar('\n');

  printf("pattern bytes: ");
  for(int i = 0; i < pat_len; i++) {
    printf("\\x%02x", pat_buf[i]);
  }
  putchar('\n');
#endif

  long addr = find_sig(file_buf, file_sz, pat_buf, pat_len, mask);
  if(addr == -1) {
    printf("pattern not found\n");
  } else {
    printf("found at addr: 0x%lx\n", addr);
    printf("matched bytes: ");
    for(int i = addr; i < addr + pat_len; i++) {
      printf("\\x%02x", file_buf[i]);
    }
    putchar('\n');
  }

  free(file_buf);
  free(pat_buf);
  free(mask);
  fclose(f);
}
