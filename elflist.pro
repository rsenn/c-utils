TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

*msvc* {
  DEFINES += INLINE=__inline
  DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
  DEFINES += _CRT_SECURE_NO_WARNINGS=1

  QMAKE_CFLAGS_WARN_ON = -W3
}

#mingw | mingw32 | mingw64 | msvc {
win32 | winn64 {
  DEFINES += USE_READDIR=0

} else {
   DEFINES += USE_READDIR=1
}

#win32|win64|msvc|mingw32:CONFIG(release, debug|release):  LIBS += -ladvapi32 -lws2_32
msvc: LIBS += advapi32.lib ws2_32.lib
mingw32|mingw64: LIBS += -ladvapi32 -lws2_32

mingw | mingw32 | mingw64 {
  QMAKE_LFLAGS += -static-libgcc -static-libstdc++
}

!*msvc* {
  DEFINES += INLINE=inline

  #!*mingw*: DEFINES += USE_READDIR=1

#  #QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter
}

INCLUDEPATH += .

HEADERS = lib/buffer.h lib/byte.h lib/elf.h lib/fmt.h lib/mmap.h lib/open.h lib/range.h lib/str.h


SOURCES = elflist.c lib/buffer/buffer_1.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnc.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_putptr.c lib/buffer/buffer_puts.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putspad.c lib/buffer/buffer_putxint640.c lib/buffer/buffer_stubborn.c lib/byte/byte_copy.c lib/byte/byte_fill.c lib/byte/byte_zero.c lib/elf/elf_dynamic_section.c lib/elf/elf_get_section.c lib/elf/elf_get_section_r.c lib/elf/elf_get_symtab.c lib/elf/elf_get_symtab_r.c lib/elf/elf_get_value.c lib/elf/elf_header.c lib/elf/elf_program_headers.c lib/elf/elf_section_find.c lib/elf/elf_section_headers.c lib/elf/elf_section_index.c lib/elf/elf_section_offset.c lib/elf/elf_section_size.c lib/elf/elf_section_typename.c lib/elf/elf_shstrtab.c lib/elf/elf_symbol_r.c lib/fmt/fmt_xint64.c lib/mmap/mmap_private.c lib/mmap/mmap_unmap.c lib/open/open_read.c lib/range/range_init.c lib/str/str_basename.c lib/str/str_diff.c lib/str/str_len.c lib/str/str_rchr.c

DEFINES += HAVE_ERRNO_H=1
