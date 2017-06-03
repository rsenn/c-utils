#include "intelhex.h"
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <iomanip>

#include <cstdio>
#include <cstring>
#include <experimental/filesystem>
#include <filesystem> // Microsoft-specific implementation header file name  
using namespace std::experimental::filesystem::v1;  

#ifdef __GNUC__
#include <libgen.h>
#endif

using std::string;
using std::vector;

uint16_t
get16(intelhex::hex_data& hex, unsigned int address) {
    return hex.get(address) | (hex.get(address+1) << 8);
}

template<class T, class S=string>
string
implode(const vector<T>& v, const S& s = ",") {
  std::stringstream ss;
  ss << std::hex << std::uppercase << std::setfill('0');
  for(auto&& p : v) {
    if(&p != &v.front()) ss << s;
    ss << std::setw(2) << static_cast<unsigned>(p);
  }
  return ss.str();
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

    if(argc > 1) {
        std::string file = argv[1];
        path p(file);
        std::string filename = p.filename().generic_string();
        int newval = -1; //0xFF72;
        if(argc > 2) {
            const char* p = argv[2];
            if(!strncmp(p, "0x", 2)) p += 2;
            newval = std::stol(p, nullptr, 16);
            printf("newval: %04X\n", newval);
        }

        hex.load(file);

        for(const auto& p : hex) {
            printf("address: 0x%04X size: 0x%04X\n", p.first, p.second.size());
        }

/*        uint16_t config_reg = get16(hex, 0x400E);
        printf("old value: 0x%04X\n", config_reg);
*/

		if(hex.size()) {
		uint32_t addr, v;
		  intelhex::hex_data::iterator it = --hex.end();

		  addr = it->first / 2;
		  v = get16(hex, addr);
		  printf("block %08X[%d]: %s\n", addr, it->second.size(), implode(it->second, ' ').c_str());

		  if(newval != -1) {

//			  hex.erase(it->first, it->first+1);
			  set16(hex, addr, newval);
			  //hex.set(0x4010, 0x3f);
			  printf("new value: 0x%04X\n", newval);
			  hex.write(filename.c_str());
			}
		}
    }

    return 0;
}
