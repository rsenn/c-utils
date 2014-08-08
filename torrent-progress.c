#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "buffer.h"
#include "open.h"
#include "mmap.h"

#define BLOCK_SIZE 4096

static char buffer_1_out[BUFFER_OUTSIZE];
static buffer infile = BUFFER_INIT((void*)read, -1, 0,0);
static buffer buffer_1 = BUFFER_INIT((void*)write, 2, buffer_1_out, BUFFER_OUTSIZE);

int check_block_zero(char *b, unsigned long n)
{
  unsigned long i;
  for(i = 0; i < n; i++)
  {
    if(b[i] != '\0')
      return 0;
  }
  return 1;
}

int get_block(char *b)
{
  return buffer_get(&infile, b, BLOCK_SIZE);
}

static int fraction = 1, space = 1;

int main(int argc, char *argv[])
{
  int ai;

  for(ai = 1; ai < argc; ++ai) {
    char* av = argv[ai];

    if(av[0] != '-')
      break;

    switch(av[1]) {
      case 'F': fraction = 0;  break;
      case 'S': space = 0;  break;
      case '-': ++ai; goto next;
      default: 
          goto next;
    }
  }
next:
  for(; ai < argc; ++ai) {
    unsigned long fsize;
    unsigned int blocks;
    int zero_blocks = 0;
    int nonzero_blocks;
    unsigned int percent;;
    unsigned int bi;
    char *m = mmap_private(argv[ai], &fsize);
    /*struct stat st;
    infile.fd = open_read(argv[1]);
    	 buffer_puts(&buffer_1,"fd #");
    	 buffer_putulong(&buffer_1,infile.fd);;
    	 buffer_putnlflush(&buffer_1);
    				fstat(infile.fd, &st);
    fsize = st.st_size;*/
    blocks = fsize / BLOCK_SIZE;
    //buffer_puts(&buffer_1,"fsize #"); buffer_putulong(&buffer_1,fsize);; buffer_puts(&buffer_1,", blocks #");buffer_putulong(&buffer_1,blocks); buffer_putnlflush(&buffer_1);

    for(bi = 0; bi < blocks; bi++)
    {
//get_block(m);


      zero_blocks += check_block_zero(&m[bi*BLOCK_SIZE], BLOCK_SIZE);
//fprintf(stderr, "block #%lu\n", bi); fflush(stderr);
    }
//	 buffer_putulong(&buffer_1,blocks);
//	 buffer_putnlflush(&buffer_1);
    nonzero_blocks = blocks - zero_blocks;
    percent = (float)nonzero_blocks * 10000 / blocks;

    buffer_puts(&buffer_1,argv[ai]);
    buffer_puts(&buffer_1,(space?": ":":"));
    if(!fraction)
      percent += 50;
    buffer_putulong(&buffer_1,percent/100);
    if(fraction) {
      buffer_puts(&buffer_1,".");
      buffer_putulong(&buffer_1,percent%100);
    }
    buffer_putnlflush(&buffer_1);

    mmap_unmap(m, fsize);;
  }

  return 0;
}
