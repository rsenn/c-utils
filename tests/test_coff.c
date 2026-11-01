#include "unit_test.h"
#include "../lib/coff.h"
#include "../lib/byte.h"
#include "../lib/str.h"

#define IMG 512
static size_t
build(unsigned char*x){ byte_zero(x,IMG); coff_file_header*h=(void*)x; h->machine=0x14c; h->number_of_sections=1; h->pointer_to_symbol_table=100; h->number_of_symbols=2; h->size_of_optional_header=0;
 coff_section_header*s=(void*)(x+20); byte_copy(s->name,5,".text"); s->size_of_raw_data=16; s->pointer_to_raw_data=60; s->pointer_to_linenumbers=80; s->number_of_linenumbers=2;
 coff_symtab_entry*sy=(void*)(x+100); byte_copy(sy[0].e.name,4,"main"); sy[0].e.value=0; sy[0].e.scnum=1; sy[0].e.sclass=COFF_C_EXT;
 /* second symbol: long name in the string table */ coff_symtab_entry*s1=(coff_symtab_entry*)(x+100+18); s1->e.zeroes=0; s1->e.offset=4; s1->e.scnum=1; s1->e.sclass=COFF_C_STATIC;
 unsigned char*st=x+100+36; st[0]=4+16; byte_copy(st+4,15,"long_symbol_nam"); return 100+36+20; }

TEST(test_coff_headers) {
  unsigned char x[IMG];
  uint16 ns = 0;

  build(x);
  ASSERT_EQ(1, coff_header_file(x) == (coff_file_header*)x);
  ASSERT_EQ(20, (unsigned char*)coff_header_opt(x) - x);
  ASSERT_EQ(20, (unsigned char*)coff_header_sections(x, &ns) - x); /* no optional header */
  ASSERT_EQ(1, ns);
  ASSERT_EQ(20, (unsigned char*)coff_get_section(x, 1) - x);
  ASSERT_EQ(1, coff_get_section(x, 0) == NULL); /* sections are numbered from 1 */
  ASSERT_EQ(1, coff_get_section(x, 2) == NULL);
}

TEST(test_coff_symbols) {
  unsigned char x[IMG];
  uint32 n = 0, size = 0;
  range r;

  build(x);
  ASSERT_EQ(100, (unsigned char*)coff_get_symtab(x, &n) - x);
  ASSERT_EQ(2, n);
  r = coff_symbol_table(x);
  ASSERT_EQ(18, r.elem_size);
  ASSERT_EQ(36, (char*)r.end - (char*)r.start);
  ASSERT_EQ(100, (unsigned char*)coff_index_symtab(x, 0) - x);
  ASSERT_EQ(118, (unsigned char*)coff_index_symtab(x, 1) - x);
  ASSERT_EQ(1, coff_index_symtab(x, 2) == NULL);
  ASSERT_EQ(1, coff_index_symtab(x, -1) == NULL);

  ASSERT_EQ(136, (const unsigned char*)coff_get_strtab(x, &size) - x);
  ASSERT_EQ(20, size);
  ASSERT_EQ(0, byte_diff(coff_symbol_name(x, coff_index_symtab(x, 0)), 4, "main"));
  ASSERT_STR_EQUAL("long_symbol_nam", coff_symbol_name(x, coff_index_symtab(x, 1)) );
}

TEST(test_coff_bad_string_offset) {
  unsigned char x[IMG];
  coff_symtab_entry* bad;

  build(x);
  bad = coff_index_symtab(x, 1);
  bad->e.offset = 9999; /* a corrupt file: this used to read far outside the file */
  ASSERT_STR_EQUAL("<bad string table offset>", coff_symbol_name(x, bad));
}

TEST(test_coff_line_numbers) {
  unsigned char x[IMG];
  range r;

  build(x);
  r = coff_line_numbers(x, coff_get_section(x, 1));
  ASSERT_EQ(6, r.elem_size);
  ASSERT_EQ(12, (char*)r.end - (char*)r.start);
  ASSERT_EQ(80, (unsigned char*)r.start - x);
}

TEST(test_coff_class_names) {
  ASSERT_STR_EQUAL("EXT", coff_sclass_name(COFF_C_EXT));
  ASSERT_STR_EQUAL("STATIC", coff_sclass_name(COFF_C_STATIC));
  ASSERT_STR_EQUAL("FILE", coff_sclass_name(COFF_C_FILE));
  ASSERT_EQ(1, coff_sclass_name(200) == NULL);
  ASSERT_STR_EQUAL("external symbol", coff_storage_class(COFF_C_EXT));
  ASSERT_STR_EQUAL("automatic variable", coff_storage_class(COFF_C_AUTO));
  ASSERT_EQ(1, coff_storage_class(200) == NULL);
}

#define RUN_COFF_TESTS() \
  RUN(test_coff_headers); \
  RUN(test_coff_symbols); \
  RUN(test_coff_bad_string_offset); \
  RUN(test_coff_line_numbers); \
  RUN(test_coff_class_names)

TESTS(coff) { RUN_COFF_TESTS(); }
