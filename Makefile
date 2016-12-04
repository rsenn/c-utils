#include "fmt.h"

size_t fmt_xmlescape(char* dest,uint32_t ch) {
  char* x;
  size_t n;
/*
From http://en.wikipedia.org/wiki/XML#Valid_characters

Unicode code points in the following ranges are valid in XML 1.0 documents:
  U+0009, U+000A, U+000D: these are the only C0 controls accepted in XML 1.0;
  U+0020–U+D7FF, U+E000–U+FFFD: this excludes some (not all) non-characters in the BMP (all surrogates, U+FFFE and U+FFFF are forbidden);
  U+10000–U+10FFFF: this includes all code points in supplementary planes, including non-characters.
*/
  if(ch==0 || (ch>=0xd780 && ch<=0xdfff) || ch==0xfffe || ch==0xffff || ch>0x10ffff) return 0;
  if((ch&0x7f)<20 && ch!=9 && ch!=0xa && ch!=0xd && ch!=0x85) {
    char buf[6];
    buf[0]='&';
    buf[1]='#';
    buf[2]='x';
    n=3+fmt_xlong(buf+3,ch);
    buf[n++]=';';
    x=buf;
  } else
    switch (ch) {
    case '&':
      x="&amp;"; n=5;
      break;
    case '<':
      x="&lt;"; n=4;
      break;
    default:
      return fmt_utf8(dest,ch);
    }
  if(dest) {
    size_t i;
    for(i=0; i<n; ++i)
      dest[i]=x[i];
  }
  return n;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                          ----------------------------------------------------------------------
template<class Char>
std::unordered_set<Char>
collect_chars(const std::map<Char,std::vector<Char> >& m) {
	std::unordered_set<Char> r;
	for(auto& kv : m) {
		r.insert(kv.first);
		for(auto& ch : kv.second) {
			r.insert(ch);
		}
	}
	/*for(auto& kv : m) {
		r.insert(kv.first);
	}*/
	return r;
}


//-----------------------------------------------------------------------------
static adjacency_t
make_adjacency_matrix() {
	adjacency_t m;

	m['1'] = m['+'] = range_to_v("��2\"qQ");
	m['2'] = m['"'] = range_to_v("1+3*wWqQ");
	m['3'] = m['*'] = range_to_v("2\"4�eE�wW");
	m['4'] = m['�'] = range_to_v("3*5%rReE�");
	m['5'] = m['%'] = range_to_v("4�6&tTrR");
	m['6'] = m['&'] = range_to_v("5%7/zZtT");
	m['7'] = m['/'] = range_to_v("6&8(uUzZ");
	m['8'] = m['('] = range_to_v("7/9)iIuU");
	m['9'] = m[')'] = range_to_v("8(0=oOiI");
	m['0'] = m['='] = range_to_v("9)'?pPoO");
	m['\''] = m['?'] = range_to_v("0=��pP");
	m['^'] = m['`'] = range_to_v("'?��");
	//m['"'] = m['!'] = range_to_v("'��$���");

	m['q'] = m['Q'] = range_to_v( {'1', '2', 'w', 'W', 'A', 'a'} );
	m['w'] = m['W'] = range_to_v( {'q', 'Q', '2', '3', 'e', 'E', 's', 'S', 'a', 'A'} );
	m['e'] = m['E'] = range_to_v( {'q', 'Q', '2', '3', 'e', 'E', 's', 'S', 'a', 'A'} );
	m['r'] = m['R'] = range_to_v("eE45tTfFdD");
	m['t'] = m['T'] = range_to_v("rR56zZgGfF");
	m['z'] = m['Z'] = range_to_v("tT67uUhHgG");
	m['u'] = m['U'] = range_to_v("zZ78iIjJhH");
	m['i'] = m['I'] = range_to_v("uU89oOkKjJ");
	m['o'] = m['O'] = range_to_v("iI90pPlLkK");
	m['p'] = m['P'] = range_to_v("oO0'����lL");
	m['�'] = m['�'] = range_to_v("pP'����");

	//	m['a'] = m['A'] = range_to_v("qQwWsSyY<>");
	m['a'] = m['A'] = range_to_v("qQwWsSyY");
	m['s'] = m['S'] = range_to_v("aAwWeEdDxXyY");
	m['d'] = m['D'] = range_to_v("sSeErRfFcCxX");
	m['f'] = m['F'] = range_to_v("dDrRtTgGvVcC");
	m['g'] = m['g'] = range_to_v("fFtTzZhHbBvV");
	m['h'] = m['H'] = range_to_v("gGzZuUjJnNbB");
	m['j'] = m['J'] = range_to_v("hHuUiIkKmMnN");
	m['k'] = m['k'] = range_to_v("jJiIoOlL,;mM");
	m['l'] = m['L'] = range_to_v("kKoOpP��.:,;");
	m['�'] = m['�'] = range_to_v("lLpP����-_.:");
	m['�'] = m['�'] = range_to_v("����$�-_");
	m['$'] = m['�'] = range_to_v("��");
	m['<'] = m['>'] = range_to_v("aAyY");
	m['y'] = m['Y'] = range_to_v("<>aAsSxX");
	m['x'] = m['X'] = range_to_v("yYsSdDcC ");
	m['c'] = m['C'] = range_to_v("xXdDfFvV ");
	m['v'] = m['V'] = range_to_v("cCfFgGbB ");
	m['b'] = m['B'] = range_to_v("vVgGhHnN ");
	m['n'] = m['N'] = range_to_v("bBhHjJmM ");
	m[#include "likely.h"
#include <stdlib.h>
#ifndef __MINGW32__
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#endif
#include "iarray.h"
#ifdef __dietlibc__
#include <sys/atomic.h>
#else
#define __CAS(ptr,oldval,newval) __sync_val_compare_and_swap(ptr,oldval,newval)
#endif

static iarray_page* new_page(size_t pagesize) {
#ifdef __MINGW32__
  void* x=malloc(pagesize);
  if(x==0) return 0;
#else
  void* x=mmap(0,pagesize,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
  if(x==MAP_FAILED) return 0;
#endif
  return (iarray_page*)x;
}

void* iarray_allocate(iarray* ia,size_t pos) {
  size_t index,prevlen=ia->len;
  /* first, find the linked list of pages */
  iarray_page** p=&ia->pages[pos%(sizeof(ia->pages)/sizeof(ia->pages[0]))];
  /* this is here so we don't munmap and then re-mmap pages when a
   * certain path makes it necessary to mmap several pages into the
   * linked list and we are competing with another thread that does the
   * same thing */
  iarray_page* newpage=0;
  /* since we have a fan-out of 16, on page 0 the elements are 0, 16, 32, ...
   * so we divide pos by the fan-out here */
  size_t realpos=pos;
  pos /= sizeof(ia->pages)/sizeof(ia->pages[0]);
  /* now walk the linked list of pages until we reach the one we want */
  for(index=0; ; index+=ia->elemperpage) {
    if(!*p) {
      if(!newpage)
	if(!(newpage=new_page(ia->bytesperpage))) return 0;
      if(__CAS(p,0,newpage)==0)
	newpage=0;
    }
    if(index+ia->elemperpage>pos)
      break;
    p=&(*p)->next;
  }
  if(newpage)
#ifdef __MINGW32__
    free(newpage);
#else
    munmap(newpage,ia->bytesperpage);
#endif
  {
    size_t l;
    do {
      l=__CAS(&ia->len,prevlen,realpos);
    } while(l<realpos);
  }
  return &(*p)->data[(pos-index)*ia->elemsize];
}
                                                                                                                                                                                                                                                                                     er_puts.o $(BUILDDIR)byte_chr.o $(BUILDDIR)str_diffn.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_copys.o
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)count-depth.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)byte_chr.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)byte_copy.o $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putulong.o $(BUILDDIR)fmt_ulong.o
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)list-r.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putsa.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)byte_copy.o $(BUILDDIR)dir_close.o $(BUILDDIR)dir_open.o $(BUILDDIR)dir_read.o $(BUILDDIR)dir_type.o $(BUILDDIR)fmt_ulong.o $(BUILDDIR)fmt_uint64.o  $(BUILDDIR)str_diffn.o $(BUILDDIR)str_len.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_cats.o $(BUILDDIR)stralloc_copyb.o $(BUILDDIR)stralloc_copys.o $(BUILDDIR)stralloc_init.o $(BUILDDIR)stralloc_nul.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)stralloc_readyplus.o
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)reg2cmd.o: reg2cmd.c
$(BUILDDIR)reg2cmd$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)reg2cmd.o $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putc.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putuint64.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)byte_chr.o $(BUILDDIR)byte_copy.o $(BUILDDIR)fmt_uint64.o $(BUILDDIR)scan_fromhex.o $(BUILDDIR)scan_xlonINDX( 	 ~�J&           (   8   �       �     E �                        1#    ��|��M������M������M���|��M�       �
              
B U 9 3 8 F ~ 1 . O                 1#    ��|��M������M������M���|��M�       �
              
B U 9 3 8 F ~ 1 . O                 1#    ��|��M������M������M���|��M�       �
              
B U 9 3 8 F ~ 1 . O                 1#    !}>��M���@��M���@��M�!}>��M�       �
              
B U B 4 8 4 ~ 1 . O                 1#    !}>��M ��@��M���@��M�!}>��M�       �
              
B U B 4 8 4 ~ 1 . O                 1#    !}>��M���@��M���@��M�!}>��M�       �
              
B U B 4 8 4 ~ 1 . O                 1#    &
K��M���O��M���O��M�&
K��M�       �              
B U C E E 4 ~ 1 . O                 1#    &
K��M���O��M���O��M�&
K��M�       �              
B U C E E 4 ~ 1 . O                 1#    &
K��M���O��M���O��M�&
K��M�       �              
B U C E E 4 ~ 1 . O                 1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                 1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                 1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                 1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                 1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                 1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                 1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                 1#    ?v��M� �z��M� �z��M�?v��M�       O              
B U D 7 F 2 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 �    h V     1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U  4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 h V     1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                       h V     1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 h V     1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M ?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                 1#    ?�2��M� Y7��M� Y7��M�?�2��M�       &              
B U E 4 A 5 ~ 1 . O                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       #include <sys/types.h>

#ifndef _WIN32
#include <unistd.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#include <stdio.h>
#else
#include <sys/types.h>

#include <sys/mman.h>
#endif
#include "open.h"
#include "mmap.h"
#include "uint64.h"

HANDLE _get_osfhandle(int fd);

#if defined(__x86_64__) && defined(__linux) && !defined(__dietlibc__)
#define mmap mmap64
#endif

char* mmap_map(int fd, size_t sz, uint64 offset) {
#if defined(_WIN32)
  HANDLE h = _get_osfhandle(fd), m;
  DWORD szl, szh;
  char* map;
  szl = GetFileSize(h, &szh);
  m=CreateFileMapping(h, 0, PAGE_WRITECOPY, szh, szl, NULL);
  map = 0;
  if(m) {
   map=MapViewOfFile(m, FILE_MAP_COPY, (offset >> 32), offset & 0xffffffff, sz);
   //if(map == NULL)  fprintf(stderr, "MapViewOfFile(%p, %i, 0x%08x, 0x%08x, %lu) = NULL\n",  m, FILE_MAP_COPY, (offset >> 32), offset & 0xffffffff, sz);
  } //else fprintf(stderr, "CreateFileMapping(%p, %i, %i, 0x%08x, 0x%08x, NULL) = NULL\n",  h, 0, PAGE_WRITECOPY, szh, szl);

  CloseHandle(m);
  return map;
#else
  char* map;
  map = (char *)mmap(0, sz, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, offset);
  if(map == (char *) - 1)
    map = 0;
  return map;
#endif
}
                                                                                                                                                                                                                                                                                                                                                                             available.

   array_allocate does not change x to have failed; if you want to do
   that, use array_fail.
*/

void* array_allocate(array* x, uint64 membersize, int64 pos) {
  uint64 wanted;
  if(__unlikely(x->allocated < 0)) return 0; /* array is failed */
  if(__unlikely(pos + 1 < 1)) return 0;
  /* second case of overflow: pos * membersize too large */
  if(__unlikely(!umult64(membersize, pos + 1, &wanted))) return 0;

  if(wanted > (uint64)x->initialized) {
    if(__unlikely(wanted >= (uint64)x->allocated)) {
      /* round up a little */
      if(membersize < 8)
  wanted = (wanted + 127) & (-128ll);  /* round up to multiple of 128 */
      else
  wanted = (wanted + 4095) & (-4096ll);  /* round up to 4k pages */

      if(__unlikely(wanted < 128)) return 0;  /* overflow during rounding */

      if(sizeof(size_t) != sizeof(int64) && __unlikely((size_t)(wanted) != wanted))
  return 0;
      {
  char* tmp = realloc(x->p, wanted);
  if(__unlikely(!tmp)) return 0;
  x->p = tmp;
      }
      x->allocated = wanted;
      byte_zero(x->p + x->initialized, x->allocated - x->initialized);
    }
    x->initialized = (pos + 1) * membersize;
  }

  return x->p + pos * membersize;
}
                                                                                                                                                                                                                                                                                                                                                      c_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c strlist.h strlist_push.c strlist_pushunique.c strlist_at.c strlist_count.c strlist_index_of.c strlist_sort.c
