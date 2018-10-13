#include "lib/getopt.h"

void
get_sources(const char* basedir, strarray* sources) {

    rdir_t rdir;
    if(rdir_open(&rdir, basedir)) {
        const char* s;

        while((s = rdir_read(&rdir))) {
            strarray_push(sources, s);
        }
    }
}


int main(int argc, char* argv[]) {
  return 0;
}
