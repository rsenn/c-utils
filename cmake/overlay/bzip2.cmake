cmake_minimum_required(VERSION 3.5)
project(bzip2 C)

add_library(bz2 STATIC blocksort.c bzlib.c compress.c crctable.c decompress.c huffman.c randtable.c)
target_compile_definitions(bz2 PRIVATE _FILE_OFFSET_BITS=64)

install(TARGETS bz2 ARCHIVE DESTINATION lib)
install(FILES bzlib.h DESTINATION include)
