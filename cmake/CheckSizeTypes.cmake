include(CheckIncludeFile)
include(CheckTypeSize)

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stddef.h HAVE_STDDEF_H)
check_include_file(vcruntime.h HAVE_VCRUNTIME_H)

set(SIZE_TYPES_INCLUDE_FILES "")

if(HAVE_SYS_TYPES_H)
  set(SIZE_TYPES_INCLUDE_FILES
      "#include <sys/types.h>\n${SIZE_TYPES_INCLUDE_FILES}")
endif(HAVE_SYS_TYPES_H)
if(HAVE_INTTYPES_H)
  set(SIZE_TYPES_INCLUDE_FILES
      "#include <inttypes.h>\n${SIZE_TYPES_INCLUDE_FILES}")
endif(HAVE_INTTYPES_H)
if(HAVE_STDINT_H)
  set(SIZE_TYPES_INCLUDE_FILES
      "#include <stdint.h>\n${SIZE_TYPES_INCLUDE_FILES}")
endif(HAVE_STDINT_H)
if(HAVE_STDDEF_H)
  set(SIZE_TYPES_INCLUDE_FILES
      "#include <stddef.h>\n${SIZE_TYPES_INCLUDE_FILES}")
endif(HAVE_STDDEF_H)
if(HAVE_VCRUNTIME_H)
  set(SIZE_TYPES_INCLUDE_FILES
      "#include <vcruntime.h>\n${SIZE_TYPES_INCLUDE_FILES}")
endif(HAVE_VCRUNTIME_H)
