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

#define RUN_ELF_TESTS() \
  RUN(test_elf_check_valid); \
  RUN(test_elf_check_header); \
  RUN(test_elf_check_tables); \
  RUN(test_elf_check_symbols); \
  RUN(test_elf_check_program_headers); \
  RUN(test_elf_check_no_sections)

TESTS(elf) { RUN_ELF_TESTS(); }
