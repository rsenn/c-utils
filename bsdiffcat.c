#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/errmsg.h"
#include "lib/uint32.h"
#include "lib/uint64.h"

typedef struct {
  char magic[8];
  uint64 new_file_length;
  uint32 new_file_segment_length;
  uint32 compressed_header_block_length;
  uint64 patch_data_block_length;
} bsdiff_header;

void
debug_int(const char* name, int64 value) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putxlonglong(buffer_2, value);
  buffer_putnlflush(buffer_2);
}

int
bsdiff_read_header(buffer* b, bsdiff_header* hdr) {
  char buffer[8];

  if(buffer_get(b, hdr->magic, 8) != 8) return 0;

  if(byte_diff(hdr->magic, 7, "BSDIFF4")) return 0;

  if(buffer_get(b, buffer, 8) != 8) return 0;
  uint64_unpack(buffer, &hdr->new_file_length);

  if(buffer_get(b, buffer, 4) != 4) return 0;
  uint32_unpack(buffer, &hdr->new_file_segment_length);

  if(buffer_get(b, buffer, 4) != 4) return 0;
  uint32_unpack(buffer, &hdr->compressed_header_block_length);

  if(buffer_get(b, buffer, 8) != 8) return 0;
  uint64_unpack(buffer, &hdr->patch_data_block_length);

  return 1;
}

int
main(int argc, char* argv[]) {
  buffer input;
  bsdiff_header h;
  int exitcode = 0;

  if(buffer_mmapread(&input, argv[1]) == 0) {

    if(bsdiff_read_header(&input, &h)) {

      debug_int("new_file_length", h.new_file_length);
      debug_int("new_file_segment_length", h.new_file_segment_length);
      debug_int("compressed_header_block_length", h.compressed_header_block_length);
      debug_int("patch_data_block_length", h.patch_data_block_length);

    } else {
      errmsg_infosys(str_basename(argv[0]), ": ", "read header", ": ", NULL);
      exitcode = 2;
    }
  } else {
      errmsg_infosys(str_basename(argv[0]), ": ", "open file", ": ", NULL);
      exitcode = 1;
  }

  buffer_close(&input);
  return exitcode;
}
