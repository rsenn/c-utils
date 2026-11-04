#ifndef _MPLAB_H
#define _MPLAB_H

#define MAP_USE_HMAP 1
#include "../../lib/map.h"

typedef struct mplab_config_s {
  int warning_level : 4;
  unsigned verbose_messages : 1, optimize_global : 1, optimize_speed : 1, optimize_debug : 1, optimize_assembler : 1,
      preprocess_assembler : 1;
  int debugger : 8;
  unsigned clear_bss : 1, keep_generated_startup_as : 1, initialize_data : 1, calibrate_oscillator : 1,
      backup_reset_condition_flags : 1, format_hex_file_for_download : 1, managed_stack : 1,
      program_default_config_words : 1, link_in_peripheral_library : 1;
  char* additional_command_line_options;
  unsigned memory_model : 1, size_of_double : 1, size_of_float : 1;
} mplab_config_t;

void output_mplab_project(buffer* b, MAP_T* rule_map, MAP_T* vars, const strlist* include_dirs);

#endif
