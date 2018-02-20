struct { 
  unsigned char o32, o64;
} pe_offsets[] = {
{   0,   0 }, /* magic */
{   4,   4 }, /* size_of_code */
{   8,   8 }, /* size_of_initialized_data */
{  12,  12 }, /* size_of_uninitialized_data */
{  16,  16 }, /* address_of_entry_point */
{  20,  20 }, /* base_of_code */
{  28,  24 }, /* image_base */
{  32,  32 }, /* section_alignment */
{  36,  36 }, /* file_alignment */
{  40,  40 }, /* major_operating_system_version */
{  42,  42 }, /* minor_operating_system_version */
{  44,  44 }, /* major_image_version */
{  46,  46 }, /* minor_image_version */
{  48,  48 }, /* major_subsystem_version */
{  50,  50 }, /* minor_subsystem_version */
{  52,  52 }, /* reserved1 */
{  56,  56 }, /* size_of_image */
{  60,  60 }, /* size_of_headers */
{  64,  64 }, /* checksum */
{  70,  70 }, /* dll_characteristics */
{  72,  72 }, /* size_of_stack_reserve */
{  76,  80 }, /* size_of_stack_commit */
{  80,  88 }, /* size_of_heap_reserve */
{  84,  96 }, /* size_of_heap_commit */
{  88, 104 }, /* loader_flags */
{  92, 108 }, /* number_of_rva_and_sizes */
{  24,  -1 }, /* base_of_data */
};

pe_file_header*
pe_filehdr_ptr(const pe_file* pe) {
  const pe_dos_header* = pe->x;
  size_t offset;
  uint32_unpack(&pe_dos_header->e_lfanew, &offset);
  return (pe_file_header*)(pe->x + offset);
}

void*
pe_opthdr_ptr(const pe_file* pe) {
  unsigned char* p = pe_filehdr_ptr(pe);
  p += sizeof(pe_file_header) + sizeof(uint32);
  return p;
}

uint32
pe_opthdr_offset(const pe_file* pe, pe_opthdr_field fieldl) {


}
