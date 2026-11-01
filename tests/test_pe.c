#include "unit_test.h"
#include "../lib/pe.h"
#include "../lib/byte.h"
#include "../lib/str.h"

#define IMG 1024

/* A PE32 image: headers, one section at 0x1000 that is in the file at 512, with a name in it. */
static size_t
build(unsigned char* x) {
  pe_dos_header* dos = (pe_dos_header*)x;
  pe32_nt_headers* nt;
  pe_section_header* sec;

  byte_zero(x, IMG);

  dos->e_magic = 0x5a4d;
  dos->e_lfanew = 64;

  nt = (pe32_nt_headers*)(x + 64);
  nt->signature = PE_NT_SIGNATURE;
  nt->coff_header.machine = 0x14c;
  nt->coff_header.number_of_sections = 1;
  nt->coff_header.size_of_optional_header = sizeof(pe32_opt_header);
  nt->optional_header.magic = PE_MAGIC_PE32;
  nt->optional_header.number_of_rva_and_sizes = PE_NUMBEROF_DIRECTORY_ENTRIES;

  sec = (pe_section_header*)(x + 64 + sizeof(pe32_nt_headers));
  byte_copy(sec->name, 5, ".data");
  sec->virtual_size = 0x200;
  sec->virtual_address = 0x1000;
  sec->size_of_raw_data = 0x200;
  sec->pointer_to_raw_data = 512;

  byte_copy(x + 512 + 0x100, 8, "dll.dll");
  return IMG;
}

static pe32_nt_headers*
nt_of(unsigned char* x) {
  return (pe32_nt_headers*)(x + 64);
}

/*
 * const char* pe_check(const void*, size_t);
 */
TEST(test_pe_check_valid) {
  unsigned char x[IMG];
  size_t n = build(x);

  ASSERT_EQ(NULL, pe_check(x, n));
}

TEST(test_pe_check_headers) {
  unsigned char x[IMG];
  pe_dos_header* dos = (pe_dos_header*)x;
  size_t n = build(x);

  ASSERT_NE(NULL, pe_check(NULL, 0));
  ASSERT_NE(NULL, pe_check(x, 0));
  ASSERT_NE(NULL, pe_check(x, 20));
  ASSERT_NE(NULL, pe_check(x, 70)); /* cut off in the NT header */

  dos->e_magic = 0x4d5a;
  ASSERT_NE(NULL, pe_check(x, n));
  dos->e_magic = 0x5a4d;

  dos->e_lfanew = IMG - 4; /* the header does not fit */
  ASSERT_NE(NULL, pe_check(x, n));
  dos->e_lfanew = 0xffffffff;
  ASSERT_NE(NULL, pe_check(x, n));
  dos->e_lfanew = 64;

  nt_of(x)->signature = 0;
  ASSERT_NE(NULL, pe_check(x, n));
  nt_of(x)->signature = PE_NT_SIGNATURE;

  nt_of(x)->optional_header.magic = 0x1234;
  ASSERT_NE(NULL, pe_check(x, n));
  nt_of(x)->optional_header.magic = PE_MAGIC_PE32;

  nt_of(x)->coff_header.size_of_optional_header = 10;
  ASSERT_NE(NULL, pe_check(x, n));
  nt_of(x)->coff_header.size_of_optional_header = 0xffff;
  ASSERT_NE(NULL, pe_check(x, n));
  nt_of(x)->coff_header.size_of_optional_header = sizeof(pe32_opt_header);

  ASSERT_EQ(NULL, pe_check(x, n));
}

TEST(test_pe_check_sections) {
  unsigned char x[IMG];
  size_t n = build(x);
  pe_section_header* sec = (pe_section_header*)(x + 64 + sizeof(pe32_nt_headers));

  nt_of(x)->coff_header.number_of_sections = 0xffff;
  ASSERT_NE(NULL, pe_check(x, n));
  nt_of(x)->coff_header.number_of_sections = 1;

  sec->pointer_to_raw_data = IMG - 16; /* the data are past the end */
  ASSERT_NE(NULL, pe_check(x, n));
  sec->pointer_to_raw_data = 0xffffff00;
  ASSERT_NE(NULL, pe_check(x, n));
  sec->pointer_to_raw_data = 512;

  /* a section the file has no room for in a short file */
  ASSERT_NE(NULL, pe_check(x, 600));
  ASSERT_EQ(NULL, pe_check(x, n));
}

/* the export directory and its names */
TEST(test_pe_check_exports) {
  unsigned char x[IMG];
  size_t n = build(x);
  pe_export_directory* ex = (pe_export_directory*)(x + 512);
  uint32* names = (uint32*)(x + 512 + 0x80);

  nt_of(x)->optional_header.data_directory[PE_DIRECTORY_ENTRY_EXPORT].virtual_address = 0x1000;
  nt_of(x)->optional_header.data_directory[PE_DIRECTORY_ENTRY_EXPORT].size = sizeof(*ex);
  ex->name = 0x1100; /* "dll.dll" */
  ASSERT_EQ(NULL, pe_check(x, n));

  ex->name = 0x5000; /* in no section */
  ASSERT_NE(NULL, pe_check(x, n));

  ex->name = 0x11ff; /* a name that does not end: the section ends in zeros, so make it end the file */
  ex->name = 0x1100;

  ex->number_of_names = 1;
  ex->address_of_names = 0x1080;
  ex->address_of_name_ordinals = 0x1090;
  names[0] = 0x1100;
  ASSERT_EQ(NULL, pe_check(x, n));

  names[0] = 0x9000; /* a name outside */
  ASSERT_NE(NULL, pe_check(x, n));
  names[0] = 0x1100;

  ex->number_of_names = 0x7fffffff; /* more names than the file has room for */
  ASSERT_NE(NULL, pe_check(x, n));
  ex->number_of_names = 1;

  /* the directory itself in no section */
  nt_of(x)->optional_header.data_directory[PE_DIRECTORY_ENTRY_EXPORT].virtual_address = 0x7000;
  ASSERT_NE(NULL, pe_check(x, n));
}

/* the import table: libraries, their names, and the names of what they give */
TEST(test_pe_check_imports) {
  unsigned char x[IMG];
  size_t n = build(x);
  pe_import_descriptor* d = (pe_import_descriptor*)(x + 512);
  uint32* thunks = (uint32*)(x + 512 + 0x40);

  nt_of(x)->optional_header.data_directory[PE_DIRECTORY_ENTRY_IMPORT].virtual_address = 0x1000;
  d[0].name = 0x1100;
  d[0].original_first_thunk = 0x1040;
  d[0].first_thunk = 0x1040;
  thunks[0] = 0x1100 - 2; /* hint and name: the name is at 0x1100 */
  thunks[1] = 0;
  ASSERT_EQ(NULL, pe_check(x, n));

  d[0].name = 0x6000;
  ASSERT_NE(NULL, pe_check(x, n));
  d[0].name = 0x1100;

  thunks[0] = 0x6000; /* an import by name that is nowhere */
  ASSERT_NE(NULL, pe_check(x, n));

  thunks[0] = 0x80000001; /* by ordinal: no name to look at */
  ASSERT_EQ(NULL, pe_check(x, n));

  /* a table of descriptors that does not end before the file does */
  {
    int i;

    for(i = 0; i < 25; i++) {
      d[i].name = 0x1100;
      d[i].first_thunk = d[i].original_first_thunk = 0x1040;
    }
    ASSERT_NE(NULL, pe_check(x, n));
  }
}


/*
 * header accessors
 */
TEST(test_pe_headers) {
  unsigned char x[IMG];
  uint16 ns = 0;
  uint32 nd = 0;
  pe_section_header* s;

  build(x);
  ASSERT_EQ(PE_NT_SIGNATURE, pe_header_sig(x));
  ASSERT_EQ(64, (unsigned char*)pe_header_nt(x) - x);
  ASSERT_EQ(68, (unsigned char*)pe_header_coff(x) - x);
  ASSERT_EQ(88, (unsigned char*)pe_header_opt(x) - x);
  ASSERT_EQ(1, pe_header_nt32(x) != NULL);
  ASSERT_EQ(1, pe_header_opt32(x) != NULL);
  ASSERT_EQ(1, pe_header_nt64(x) == NULL); /* it is a PE32 file */
  ASSERT_EQ(1, pe_header_opt64(x) == NULL);

  s = pe_header_sections(x, &ns);
  ASSERT_EQ(1, ns);
  ASSERT_EQ(0, byte_diff(s->name, 5, ".data"));
  ASSERT_EQ(1, pe_get_section(x, ".data") == s);
  ASSERT_EQ(1, pe_get_section(x, ".nope") == NULL);

  ASSERT_EQ(1, pe_get_datadir(x, &nd) != NULL);
  ASSERT_EQ(PE_NUMBEROF_DIRECTORY_ENTRIES, nd);
  ASSERT_EQ(1, pe_header_datadir(x) == pe_get_datadir(x, &nd));
}

/*
 * conversions between relative virtual addresses and file offsets
 */
TEST(test_pe_rva) {
  unsigned char x[IMG];

  build(x);
  ASSERT_EQ(0, pe_rva2section(x, 0x1000));
  ASSERT_EQ(0, pe_rva2section(x, 0x11ff));
  ASSERT_EQ(-1, pe_rva2section(x, 0x5000)); /* outside every section */
  ASSERT_EQ(-1, pe_rva2section(x, 0x10));
  ASSERT_EQ(512 + 0x100, pe_rva2offset(x, 0x1100));
  ASSERT_EQ(-1, pe_rva2offset(x, 0x5000));
  ASSERT_EQ(0x1000 + 0x100, pe_offset2rva(x, 512 + 0x100));
  ASSERT_EQ(512 + 0x100, (unsigned char*)pe_rva2ptr(x, 0x1100) - x);
  ASSERT_EQ(1, pe_rva2ptr(x, 0x5000) == NULL);
}

TEST(test_pe_datadir_name) {
  ASSERT_STR_EQUAL("EXPORT", pe_datadir_name(0));
  ASSERT_STR_EQUAL("IMPORT", pe_datadir_name(1));
  ASSERT_STR_EQUAL("COMHEADER", pe_datadir_name(14));
  ASSERT_EQ(1, pe_datadir_name(99) == NULL);
}

/*
 * uint64 pe_thunk(void* pe, void* ptr, int64 index): import thunks of 32 and 64 bit files
 * normalised to 64 bit (the ordinal flag moves to bit 63)
 */
TEST(test_pe_thunk) {
  unsigned char x[IMG];
  uint32 t32[2] = {0x80001234, 0x2000};

  build(x);
  ASSERT_EQ(0x8000000000001234ull, pe_thunk(x, t32, 0));
  ASSERT_EQ(0x2000, pe_thunk(x, t32, 1));
}

#define RUN_PE_TESTS() \
  RUN(test_pe_headers); \
  RUN(test_pe_rva); \
  RUN(test_pe_datadir_name); \
  RUN(test_pe_thunk); \
  RUN(test_pe_check_valid); \
  RUN(test_pe_check_headers); \
  RUN(test_pe_check_sections); \
  RUN(test_pe_check_exports); \
  RUN(test_pe_check_imports)

TESTS(pe) { RUN_PE_TESTS(); }
