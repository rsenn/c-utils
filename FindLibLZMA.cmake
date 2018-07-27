# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindLibLZMA
# -----------
#
# Find the XML processing library (liblzma).

# Result variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``LIBLZMA_FOUND``
#   true if liblzma headers and libraries were found
# ``LIBLZMA_INCLUDE_DIR``
#   the directory containing LibLZMA headers
# ``LIBLZMA_INCLUDE_DIRS``
#   list of the include directories needed to use LibLZMA
# ``LIBLZMA_LIBRARIES``
#   LibLZMA libraries to be linked
# ``LIBLZMA_DEFINITIONS``
#   the compiler switches required for using LibLZMA
# ``LIBLZMA_VERSION_STRING``
#   the version of LibLZMA found (since CMake 2.8.8)
#
# Cache variables
# ^^^^^^^^^^^^^^^
#
# The following cache variables may also be set:
#
# ``LIBLZMA_INCLUDE_DIR``
#   the directory containing LibLZMA headers
# ``LIBLZMA_LIBRARY``
#   path to the LibLZMA library

# use pkg-config to get the directories and then use these values
# in the find_path() and find_library() calls
find_package(PkgConfig QUIET)
PKG_CHECK_MODULES(PC_LIBLZMA QUIET liblzma)
set(LIBLZMA_DEFINITIONS ${PC_LIBLZMA_CFLAGS_OTHER})

find_path(LIBLZMA_INCLUDE_DIR NAMES libxml/xpath.h
   HINTS
   ${PC_LIBLZMA_INCLUDEDIR}
   ${PC_LIBLZMA_INCLUDE_DIRS}
   PATH_SUFFIXES liblzma
   )

# CMake 3.9 and below used 'LIBLZMA_LIBRARIES' as the name of
# the cache entry storing the find_library result.  Use the
# value if it was set by the project or user.
if(DEFINED LIBLZMA_LIBRARIES AND NOT DEFINED LIBLZMA_LIBRARY)
  set(LIBLZMA_LIBRARY ${LIBLZMA_LIBRARIES})
endif()

find_library(LIBLZMA_LIBRARY NAMES lzma liblzma
   HINTS
   ${PC_LIBLZMA_LIBDIR}
   ${PC_LIBLZMA_LIBRARY_DIRS}
   )

set(LIBLZMA_INCLUDE_DIRS ${LIBLZMA_INCLUDE_DIR} ${PC_LIBLZMA_INCLUDE_DIRS})
set(LIBLZMA_LIBRARIES ${LIBLZMA_LIBRARY})

mark_as_advanced(LIBLZMA_INCLUDE_DIR LIBLZMA_LIBRARY LIBLZMA_XMLLINT_EXECUTABLE)
