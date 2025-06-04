#ifndef _MPLAB_H
#define _MPLAB_H

#include "../../lib/map.h"

typedef struct mplab_config_s {
  int warning_level : 4;
  int verbose_messages : 1;
  int optimize_global : 1;
  int optimize_speed : 1;
  int optimize_debug : 1;
  int optimize_assembler : 1;
  int preprocess_assembler : 1;
  int debugger : 8;
  int clear_bss : 1;
  int keep_generated_startup_as : 1;
  int initialize_data : 1;
  int calibrate_oscillator : 1;
  int backup_reset_condition_flags : 1;
  int format_hex_file_for_download : 1;
  int managed_stack : 1;
  int program_default_config_words : 1;
  int link_in_peripheral_library : 1;
  char* additional_command_line_options;
  int memory_model : 1;
  int size_of_double : 1;
  int size_of_float : 1;
} mplab_config_t;

void output_mplab_project(buffer* b, MAP_T* rules, MAP_T* vars, const strlist* include_dirs);

#endif
