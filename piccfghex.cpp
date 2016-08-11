#include "intelhex.h"
#include <iostream>
#include <iterator>
#include <cstdio>
#include <cstring>
#include <libgen.h>


uint16_t
get16(intelhex::hex_data& hex, unsigned int address) {
    return hex.get(address) | (hex.get(address+1) << 8);
}

void
set16(intelhex::hex_data& hex, unsigned int address, uint16_t value) {
      intelhex::hex_data::iterator it, prev;
      
    hex.set(address, value & 0xff);
    hex.set(address+1, value >> 8);
    /*
    it = hex.begin();
    for(; prev = it, ++it != hex.end();) {
      if(it->first > address+1) {
         break;
      }
    }
  std::copy(prev->second.begin(),prev->second.end(),std::ostream_iterator<int>(std::cout<< " " ));

  printf("new block: 0x%04X size: 0x%04X \n", prev->first, prev->second.size());  
    prev->second[1]=(value >> 8);
    */
//    hex[address+1] = value >> 8;
// hex.compact();
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

        for (const auto& p : hex) {
            printf("address: 0x%04X size: 0x%04X\n", p.first, p.second.size());
        }

        uint16_t config_reg = get16(hex, 0x400E);

        printf("old value: 0x%04X\n", config_reg);

        intelhex::hex_data::iterator it = --hex.end();
        
        printf("block: %08X:%08X\n", it->first, it->second.size());

        if(newval != -1) {


            hex.erase(it->first, it->first+1);
            set16(hex, 0x400E, newval);
            //hex.set(0x4010, 0x3f);
            printf("new value: 0x%04X\n", newval);
            hex.write(filename.c_str());
        }

    }

    return 0;
}