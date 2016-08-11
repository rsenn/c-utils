#include "intelhex.h"
#include <cstdio>
#include <libgen.h>


uint16_t
get16(intelhex::hex_data& hex, unsigned int address) {
  return hex.get(address) | (hex.get(address+1) << 8);
}

void
set16(intelhex::hex_data& hex, unsigned int address, uint16_t value) {
  hex.set(address, value & 0xff);
  hex.set(address+1, value >> 8);
}

int main(int argc, char* argv[])
{
  intelhex::hex_data hex;

  if (argc > 1) {
    std::string file = argv[1];
    std::string filename = basename(argv[1]);
    int newval = 0xFF72;
    if(argc > 2)
      newval = std::stoul(argv[2], nullptr, 16);

    hex.load(file);
    printf("Highest address: 0x%04X\n", hex.max_address());

    uint16_t config_reg = get16(hex, 0x400E);
    /*for (const auto& p : hex) {
      printf("address: 0x%04X size: 0x%04X\n", p.first, p.second.size());
    }*/
    printf("old value: 0x%04X\n", config_reg);
    printf("new value: 0x%04X\n", config_reg);
    set16(hex, 0x400E, newval);

    hex.write((filename+".copy").c_str());
  }

  return 0;
}