#include "intelhex.h"
#include <cstdio>
#include <cstring>
#include <libgen.h>


uint16_t
get16(intelhex::hex_data& hex, unsigned int address) {
  return hex.get(address) | (hex.get(address+1) << 8);
}

void
set16(intelhex::hex_data& hex, unsigned int address, uint16_t value) {
  hex.set(address, value & 0xff);
  hex.set(address+1, value >> 8);
  hex.compact();
}

int main(int argc, char* argv[])
{
  intelhex::hex_data hex;

  if (argc > 1) {
    std::string file = argv[1];
    std::string filename = basename(argv[1]);
    int newval = -1; //0xFF72;
    if(argc > 2) {
      const char* p = argv[2];
      if(!strncmp(p, "0x", 2)) p += 2;
      newval = std::stol(p, nullptr, 16);
printf("newval: %04X\n", newval);
    }

    hex.load(file);
    printf("Highest address: 0x%04X\n", hex.max_address());

    uint16_t config_reg = get16(hex, 0x400E);
    /*for (const auto& p : hex) {
      printf("address: 0x%04X size: 0x%04X\n", p.first, p.second.size());
    }*/
    printf("old value: 0x%04X\n", config_reg);
    if(newval != -1) {
    intelhex::hex_data::iterator it = hex.end();
    --it;
    
    printf("block: %08X:%08X\n", it->first, it->second.size());
    
    hex.erase(it->first);
  set16(hex, 0x400E, newval);
	  //hex.set(0x4010, 0x3f);
	  printf("new value: 0x%04X\n", newval);
	  hex.write(filename.c_str());
	}

  }

  return 0;
}