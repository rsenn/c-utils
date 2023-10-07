#include "lib/buffer.h"
#include "lib/pe.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/fmt.h"

static size_t size;
static void* base;
static const char* subsystems[] = {
    "UNKNOWN",
    "NATIVE",
    "WINDOWS_GUI",
    "WINDOWS_CUI",
    "OS2_CUI",
    "POSIX_CUI",
    "NATIVE_WINDOWS",
    "WINDOWS_CE_GUI",
    "EFI_APPLICATION",
    "EFI_BOOT_SERVICE_DRIVER",
    "EFI_RUNTIME_DRIVER",
    "EFI_ROM",
    "XBOX",
    "WINDOWS_BOOT_APPLICATION",
};
static const char* section = ".text";

int
process64(void* base) {
  pe64_opt_header* opthdr = pe_header_opt64(base);
  pe_section_header* scthdr;
  uint16 subsys = uint16_get(&opthdr->subsystem);

  if(!(scthdr = pe_get_section(base, section))) {
    buffer_putm_internal(buffer_2, "no such section: ", section, NULL);
    buffer_putnlflush(buffer_2);
    return -1;
  }

  buffer_putm_internal(buffer_2, "subsystem: ", subsystems[subsys], NULL);
  buffer_putnlflush(buffer_2);

  if(subsys == 3)
    uint16_pack((void*)&opthdr->subsystem, 2);
}

int
process32(void* base) {
  pe32_opt_header* opthdr = pe_header_opt32(base);
  pe_section_header* scthdr;
  uint16 subsys = uint16_get(&opthdr->subsystem);

  if(!(scthdr = pe_get_section(base, section))) {
    buffer_putm_internal(buffer_2, "no such section: ", section, NULL);
    buffer_putnlflush(buffer_2);
    return -1;
  }

  buffer_putm_internal(buffer_2, "subsystem: ", subsystems[subsys], NULL);
  buffer_putnlflush(buffer_2);

  if(subsys == 3)
    uint16_pack((void*)&opthdr->subsystem, 2);
}

int
pewrsec(const char* file) {
  int ret;
  pe_file_t* header;

  header = base = mmap_shared(file, &size);

  ret = PE_TYPE(base) == PE_MAGIC_PE64 ? process64(base) : process32(base);

  mmap_unmap(base, size);

  return ret;
}

int
main(int argc, char* argv[]) {

  const char* fn = NULL;

  if(argc > 1)
    fn = argv[1];
  if(argc > 2)
    section = argv[2];

  return pewrsec(fn);
}
