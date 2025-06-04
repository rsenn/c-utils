#ifndef _CMAKE_H
#define _CMAKE_H

#define MAP_USE_HMAP 1
#include "../../lib/buffer.h"
#include "../../lib/map.h"
#include "../../lib/strlist.h"

void output_cmake_project(buffer* b, MAP_T* rule_map, MAP_T* vars, const strlist* include_dirs, const strlist* link_dirs);

#endif
