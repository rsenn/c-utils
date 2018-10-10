gen-bcc32-makefile() { CC="bcc32c" CFLAGS="-G -O" link="ilink32"  builddir=build/bcc/Debug/ gen_a_deps build/*/*/*.a  2>debug.log |tee Makefile.bcc32; }
