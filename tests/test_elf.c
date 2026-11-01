#include "unit_test.h"
#include "../lib/elf.h"
#include "../lib/byte.h"
#include "../lib/str.h"

#define IMG 1024

/* A small ELF64 file: header, a section name table, and two sections. */
static size_t
build(unsigned char* x) {
  elf64_ehdr* h = (elf64_ehdr*)x;
  elf64_shdr* sh;
  const char names[] = "\0.shstrtab\0.strtab\0.symtab";
  const size_t nlen = sizeof(names);
  elf64_sym* sym;
  const char syms[] = "\0main";

  byte_zero(x, IMG);

  h->e_ident[0] = 0x7f;
  h->e_ident[1] = 'E';
  h->e_ident[2] = 'L';
  h->e_ident[3] = 'F';
  h->e_ident[4] = 2; /* 64 bit */
  h->e_ident[5] = 1; /* little endian */
  h->e_ident[6] = 1;
  h->e_type = 3;
  h->e_machine = 62;
  h->e_version = 1;
  h->e_ehsize = sizeof(elf64_ehdr);
  h->e_shentsize = sizeof(elf64_shdr);
  h->e_shnum = 4;
  h->e_shstrndx = 1;

  /* 64: section names, 128: strtab for symbols, 160: symbols, 256: headers */
  byte_copy(x + 64, nlen, names);
  byte_copy(x + 128, sizeof(syms), syms);
  sym = (elf64_sym*)(x + 160);
  sym[1].st_name = 1;
  sym[1].st_info = 0x12;

  h->e_shoff = 256;
  sh = (elf64_shdr*)(x + 256);

  sh[1].sh_name = 1;
  sh[1].sh_type = ELF_SHT_STRTAB;
  sh[1].sh_offset = 64;
  sh[1].sh_size = nlen;

  sh[2].sh_name = 11;
  sh[2].sh_type = ELF_SHT_STRTAB;
  sh[2].sh_offset = 128;
  sh[2].sh_size = sizeof(syms);

  sh[3].sh_name = 19;
  sh[3].sh_type = ELF_SHT_SYMTAB;
  sh[3].sh_offset = 160;
  sh[3].sh_size = 2 * sizeof(elf64_sym);
  sh[3].sh_link = 2;
  sh[3].sh_entsize = sizeof(elf64_sym);

  return 256 + 4 * sizeof(elf64_shdr);
}

/*
 * const char* elf_check(const void*, size_t);
 */
TEST(test_elf_check_valid) {
  unsigned char x[IMG];
  size_t n = build(x);

  ASSERT_EQ(NULL, elf_check(x, n));
  ASSERT_EQ(NULL, elf_check(x, IMG));
}

TEST(test_elf_check_header) {
  unsigned char x[IMG];
  size_t n = build(x);

  ASSERT_NE(NULL, elf_check(NULL, 0));
  ASSERT_NE(NULL, elf_check(x, 0));
  ASSERT_NE(NULL, elf_check(x, 10));
  ASSERT_NE(NULL, elf_check(x, 40)); /* cut off in the header */

  build(x);
  x[1] = 'X';
  ASSERT_NE(NULL, elf_check(x, n));

  build(x);
  x[4] = 7; /* class */
  ASSERT_NE(NULL, elf_check(x, n));

  build(x);
  x[5] = 2; /* big endian, not the byte order of this machine */
  ASSERT_NE(NULL, elf_check(x, n));
}

/* every number in the file that points somewhere is checked */
TEST(test_elf_check_tables) {
  unsigned char x[IMG];
  size_t n = build(x);
  elf64_ehdr* h = (elf64_ehdr*)x;
  elf64_shdr* sh = (elf64_shdr*)(x + 256);

  /* the file ends before the section headers do */
  ASSERT_NE(NULL, elf_check(x, n - 1));
  ASSERT_NE(NULL, elf_check(x, 256));

  h->e_shoff = IMG; /* table outside */
  ASSERT_NE(NULL, elf_check(x, n));
  h->e_shoff = (uint64)-1; /* and the sum of offset and size does not wrap */
  ASSERT_NE(NULL, elf_check(x, n));
  h->e_shoff = 256;

  h->e_shnum = 0xffff;
  ASSERT_NE(NULL, elf_check(x, n));
  h->e_shnum = 4;

  h->e_shentsize = 8;
  ASSERT_NE(NULL, elf_check(x, n));
  h->e_shentsize = sizeof(elf64_shdr);

  h->e_shstrndx = 4;
  ASSERT_NE(NULL, elf_check(x, n));
  h->e_shstrndx = 3; /* a section that is no string table */
  ASSERT_NE(NULL, elf_check(x, n));
  h->e_shstrndx = 1;

  /* a section outside the file */
  sh[2].sh_offset = IMG - 2;
  ASSERT_NE(NULL, elf_check(x, n));
  sh[2].sh_offset = 128;
  sh[2].sh_size = (uint64)-1;
  ASSERT_NE(NULL, elf_check(x, n));
  sh[2].sh_size = 6;

  /* a name outside the name table */
  sh[2].sh_name = 100;
  ASSERT_NE(NULL, elf_check(x, n));
  sh[2].sh_name = 11;

  /* a string table that does not end */
  x[128 + 5] = 'x';
  ASSERT_NE(NULL, elf_check(x, n));
  x[128 + 5] = 0;

  ASSERT_EQ(NULL, elf_check(x, n));
}

TEST(test_elf_check_symbols) {
  unsigned char x[IMG];
  size_t n = build(x);
  elf64_shdr* sh = (elf64_shdr*)(x + 256);
  elf64_sym* sym = (elf64_sym*)(x + 160);

  sym[1].st_name = 1000; /* outside the string table */
  ASSERT_NE(NULL, elf_check(x, n));
  sym[1].st_name = 1;

  sh[3].sh_link = 0; /* no string table */
  ASSERT_NE(NULL, elf_check(x, n));
  sh[3].sh_link = 9;
  ASSERT_NE(NULL, elf_check(x, n));
  sh[3].sh_link = 2;

  ASSERT_EQ(NULL, elf_check(x, n));
}

TEST(test_elf_check_program_headers) {
  unsigned char x[IMG];
  size_t n = build(x);
  elf64_ehdr* h = (elf64_ehdr*)x;
  elf64_phdr* ph = (elf64_phdr*)(x + 600);

  h->e_phoff = 600;
  h->e_phentsize = sizeof(elf64_phdr);
  h->e_phnum = 1;
  ph->p_type = 1;
  ph->p_offset = 0;
  ph->p_filesz = n;
  ASSERT_EQ(NULL, elf_check(x, IMG));

  ph->p_filesz = IMG + 1; /* a segment longer than the file */
  ASSERT_NE(NULL, elf_check(x, IMG));
  ph->p_filesz = 10;

  h->e_phoff = IMG - 10; /* the table does not fit */
  ASSERT_NE(NULL, elf_check(x, IMG));
  h->e_phoff = 600;

  h->e_phentsize = 4;
  ASSERT_NE(NULL, elf_check(x, IMG));
}

/* no sections at all is a valid file */
TEST(test_elf_check_no_sections) {
  unsigned char x[IMG];
  elf64_ehdr* h;

  build(x);
  h = (elf64_ehdr*)x;
  h->e_shoff = 0;
  h->e_shnum = 0;
  h->e_shstrndx = 0;
  ASSERT_EQ(NULL, elf_check(x, sizeof(elf64_ehdr)));
}


/*
 * accessors of the section headers and tables
 */
TEST(test_elf_sections) {
  unsigned char x[IMG];
  size_t sz = 0;
  void* b = x;
  range r;

  build(x);
  ASSERT_EQ(0x7f, elf_header_ident(b)[0]);
  ASSERT_EQ('E', elf_header_ident(b)[1]);
  ASSERT_EQ(256, (unsigned char*)elf_header_sections(b) - x);
  r = elf_section_headers(b);
  ASSERT_EQ(4 * sizeof(elf64_shdr), (char*)r.end - (char*)r.start);
  ASSERT_STR_EQUAL(".shstrtab", elf_shstrtab(b) + 1);

  ASSERT_STR_EQUAL("", elf_section_name(b, 0));
  ASSERT_STR_EQUAL(".shstrtab", elf_section_name(b, 1));
  ASSERT_STR_EQUAL(".symtab", elf_section_name(b, 3));
  ASSERT_EQ(64, (unsigned char*)elf_section_offset(b, 1) - x);
  ASSERT_EQ(6, elf_section_size(b, 2));
  ASSERT_EQ(48, elf_section_size(b, 3));

  ASSERT_EQ(3, elf_section_find(b, ".symtab"));
  ASSERT_EQ(2, elf_section_find(b, ".strtab"));
  ASSERT_EQ(-1, elf_section_find(b, ".nope"));
  ASSERT_EQ(3, elf_section_index(b, ELF_SHT_SYMTAB));
  ASSERT_EQ(1, elf_section_index(b, ELF_SHT_STRTAB)); /* the first of that type */

  ASSERT_EQ(128, (unsigned char*)elf_get_section(b, ".strtab", &sz) - x);
  ASSERT_EQ(6, sz);
  r = elf_get_section_r(b, ".symtab");
  ASSERT_EQ(48, (char*)r.end - (char*)r.start);
  ASSERT_EQ(160, (unsigned char*)elf_get_symtab(b, &sz) - x);
  ASSERT_EQ(48, sz);
  r = elf_get_symtab_r(b);
  ASSERT_EQ(48, (char*)r.end - (char*)r.start);
  r = elf_section(b, x + 256 + 3 * sizeof(elf64_shdr)); /* a section header gives its data */
  ASSERT_EQ(48, (char*)r.end - (char*)r.start);
  ASSERT_EQ(4, elf_get_value(b, x, offsetof(elf32_ehdr, e_shnum), 2, offsetof(elf64_ehdr, e_shnum), 2));
}

TEST(test_elf_section_typename) {
  ASSERT_STR_EQUAL("PROGBITS", elf_section_typename(ELF_SHT_PROGBITS));
  ASSERT_STR_EQUAL("SYMTAB", elf_section_typename(ELF_SHT_SYMTAB));
  ASSERT_STR_EQUAL("STRTAB", elf_section_typename(ELF_SHT_STRTAB));
  /* a corrupt file can carry any number: this used to index the table unchecked */
  ASSERT_STR_EQUAL("<unknown>", elf_section_typename(0xdeadbeef));
}

/*
 * program headers and the conversions between file offsets and virtual addresses
 */
TEST(test_elf_segments) {
  unsigned char x[IMG];
  elf64_ehdr* h = (elf64_ehdr*)x;
  elf64_phdr* p = (elf64_phdr*)(x + 512);
  range file, ph;

  build(x);
  ph = elf_program_headers(x);
  ASSERT_EQ(0, (char*)ph.end - (char*)ph.start); /* none yet */

  h->e_phoff = 512;
  h->e_phentsize = sizeof(elf64_phdr);
  h->e_phnum = 1;
  p->p_type = ELF_PT_LOAD;
  p->p_offset = 0x40;
  p->p_vaddr = 0x400040;
  p->p_filesz = 0x100;
  p->p_memsz = 0x100;
  ph = elf_program_headers(x);
  ASSERT_EQ(sizeof(elf64_phdr), (char*)ph.end - (char*)ph.start);

  file.start = x;
  file.end = x + IMG;
  ASSERT_EQ(512, (unsigned char*)elf_find_segment_offset(file, 0x50) - x);
  ASSERT_EQ(512, (unsigned char*)elf_find_segment_vaddr(file, 0x400050) - x);
  ASSERT_EQ(1, elf_find_segment_offset(file, 0x9999) == NULL);
  ASSERT_EQ(1, elf_find_segment_vaddr(file, 0x1) == NULL);
  ASSERT_EQ(0x50, elf_address_to_offset(file, 0x400050));
  ASSERT_EQ(0x400050, elf_offset_to_addr(file, 0x50));
  ASSERT_EQ(-1, elf_address_to_offset(file, 0x1));
  ASSERT_EQ(-1, elf_offset_to_addr(file, 0x9999));
}

TEST(test_elf_dynamic_none) {
  unsigned char x[IMG];
  range d;

  build(x);
  d = elf_dynamic_section(x); /* no .dynamic in this file */
  ASSERT_EQ(1, d.start == d.end);
}

#define RUN_ELF_TESTS() \
  RUN(test_elf_sections); \
  RUN(test_elf_section_typename); \
  RUN(test_elf_segments); \
  RUN(test_elf_dynamic_none); \
  RUN(test_elf_check_valid); \
  RUN(test_elf_check_header); \
  RUN(test_elf_check_tables); \
  RUN(test_elf_check_symbols); \
  RUN(test_elf_check_program_headers); \
  RUN(test_elf_check_no_sections)

TESTS(elf) { RUN_ELF_TESTS(); }
