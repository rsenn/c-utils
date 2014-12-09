#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "buffer.h"
#include "open.h"
#include "mmap.h"

#define BLOCK_SIZE 262144
#ifdef __APPLE__
typedef off_t offset_type;
#else
typedef off64_t offset_type;;
#endif

int64 filesize(int fd) {
  offset_type pos, end;
  int64 sz;
  //if(_llseek(fd, 0, 0, &pos, SEEK_CUR) < 0)  return -1;
  if((pos = lseek64(fd, 0, SEEK_CUR)) < 0) return -1;
  //if(_llseek(fd, 0, 0, &end, SEEK_END) < 0) return -1;
  if((end = lseek64(fd, 0, SEEK_END)) < 0) return -1;

  sz = end;
  lseek64(fd, pos, SEEK_SET);
  //_llseek(fd, pos >> 32, pos & 0xffffffff,  &pos, SEEK_SET);
  return sz;
}

static char buffer_1_out[BUFFER_OUTSIZE];
static buffer infile = BUFFER_INIT((void*)read, -1, 0,0);
static buffer buffer_1 = BUFFER_INIT((void*)write, 1, buffer_1_out, BUFFER_OUTSIZE);

int check_block_zero(char *b, size_t n) {
  size_t i;
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

static int verbose=0, fraction = 1, space = 1;




int main(int argc, char *argv[]) {
  int ai;

  for(ai = 1; ai < argc; ++ai) {
    char* av = argv[ai];

    if(av[0] != '-')
      break;

    switch(av[1]) {
    case 'F': fraction = 0; break;
    case 'v': verbose++; break;
    case 'S': space = 0; break;
    case '-': ++ai; goto next;
    default:
      goto next;
    }
  }
next:
  for(; ai < argc; ++ai) {
    uint64 all_blocks=0, zero_blocks = 0, nonzero_blocks;
    unsigned int percent;;
    unsigned int bi;
    int fd = open_read(argv[ai]);
    int64 fsize = filesize(fd);


    int map_blocks = 128;
    int map_size = (BLOCK_SIZE * map_blocks);
    int64 i;



    uint64 iterations = (fsize + map_size + 1) / map_size;
    uint64 remain = fsize;

    if(verbose)
        fprintf(stderr, "memory map size: %uMB (0x%016u) iterations: %i (end offset: 0x%08X)\n", 
								map_size/1048576, map_size, (int)iterations, (uint32_t)fsize);

    //(uint64)map_size * iterations);
    //mmap_private(argv[ai], &fsize);



    /*struct stat st;
    infile.fd = open_read(argv[1]);
    	 buffer_puts(&buffer_1,"fd #");
    	 buffer_putulong(&buffer_1,infile.fd);;
    	 buffer_putnlflush(&buffer_1);
    				fstat(infile.fd, &st);
    fsize = st.st_size;*/


    //buffer_puts(&buffer_1,"fsize #"); buffer_putulong(&buffer_1,fsize);; buffer_puts(&buffer_1,", blocks #");buffer_putulong(&buffer_1,blocks); buffer_putnlflush(&buffer_1);

    for(i = 0; i < iterations; i++) {

      size_t msz =  (remain >= map_size ? map_size : remain);
      uint64 mofs =  (uint64)map_size * i;


      char *m = mmap_map(fd, msz, mofs);

      int blocks = msz / BLOCK_SIZE;

      all_blocks += blocks;
      int z = 0;
      //int remain = msz - (blocks * BLOCK_SIZE);
      for(bi = 0; bi < blocks; bi++)
      {
        //get_block(m);

        z += check_block_zero(&m[bi*BLOCK_SIZE], BLOCK_SIZE);
        //fprintf(stderr, "block #%lu\n", bi); fflush(stderr);
      }
    
    if(verbose)
      fprintf(stderr, "mmap at 0x%zx, size 0x%zx%s\n", (size_t)mofs, (size_t)msz, (z < blocks ? "" : " zero"));
      
      zero_blocks += z;

      mmap_unmap(m, msz);

      if(remain >= map_size)
        remain -= map_size;
    }
    //	 buffer_putulong(&buffer_1,blocks);
    //	 buffer_putnlflush(&buffer_1);
    nonzero_blocks = all_blocks - zero_blocks;
    percent = (float)nonzero_blocks * 10000 / all_blocks;

    buffer_puts(&buffer_1,argv[ai]);
    buffer_puts(&buffer_1,(space?": ":":"));

    if(!fraction) percent += 50;

    buffer_putulong(&buffer_1,percent/100);
    
    if(fraction) {
      buffer_puts(&buffer_1,".");
      buffer_putulong(&buffer_1,percent%100);
    }
    buffer_putnlflush(&buffer_1);
  }
  return 0;
}
