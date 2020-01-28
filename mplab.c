#include "genmakefile.h"
#include "ini.h"
#include "map.h"
#include "mplab.h"

static void
make_fileno(stralloc* sa, int i) {
  stralloc_zero(sa);
  stralloc_copys(sa, "file_");
  stralloc_catulong0(sa, i, 3);
  stralloc_nul(sa);
}

static void
set_value(MAP_T map, const char* key, long value) {
  stralloc v;
  stralloc_init(&v);
  stralloc_catlong(&v, value);
  stralloc_nul(&v);

  MAP_SET(map, key, v.s);
}

static const char*
suite_guid(stralloc* sa) {
  int xc8 = !case_diffs(tools.compiler, "xc8");
  stralloc_zero(sa);
  stralloc_catm_internal(
      sa, "{", (xc8 ? "38171385-97B2-4EC5-BF2C-C2C027BA5B04" : "507D93FD-16F1-4270-980F-0C7C0207E6D3"), "}" , 0);
  stralloc_nul(sa);
  return sa->s;
}

static const char*
make_tool_key(stralloc* sa, const char* key) {
  int xc8 = !case_diffs(tools.compiler, "xc8");
  stralloc_zero(sa);
  stralloc_catm_internal(
      sa, "TS{", (xc8 ? "F42384DA-C7ED-4A02-880F-0F5E88735CE2" : "3FF1D5F2-E530-4850-9F70-F61D55BD1AC9"), "}", key, 0);
  stralloc_nul(sa);
  return sa->s;
}

void
output_mplab_project(buffer* b, MAP_T _rules, MAP_T vars, const strlist* include_dirs) {
  MAP_ITER_T it;
  strlist incdirs, srcdirs;
  const char *dir, *s, **p;
  size_t n;
  stralloc sa, file, dirname;
  mplab_config_t mplab_cfg = {3, 1, 1, 1, 1, 1, 1, 39, 1,
                              1, 0, 0, 0, 1, 1, 0, 0,  "--output=default,-inhx032 --output=+mcof,-elf",
                              1, 1, 1};

  ini_section_t *ini, *section, *cat_subfolders, *file_subfolders, *generated_files, *other_files, *file_info;

  stralloc_init(&sa);
  stralloc_init(&file);
  stralloc_init(&dirname);

  section = ini_new(&ini, "HEADER");
  ini_set(section, "magic_cookie", "{66E99B07-E706-4689-9E80-9B2582898A13}");
  ini_set(section, "file_version", "1.0");

  stralloc_copys(&sa, "PIC");
  stralloc_cat(&sa, &cfg.chip);
  stralloc_upper(&sa);
  stralloc_nul(&sa);

  ini_set(section, "device", sa.s);
  /*
    debug_sa("dirs.work", &dirs.work.sa);
    debug_sa("dirs.build", &dirs.build.sa);
    debug_sa("dirs.out", &dirs.out.sa);*/

  strlist_init(&incdirs, ';');
  strlist_init(&srcdirs, ';');

  strlist_foreach_s(include_dirs, dir) {

    stralloc_zero(&sa);
    path_relative(dir, dirs.out.sa.s, &sa);
    if(stralloc_endb(&sa, "/", 1))
      sa.len -= 1;

    if(stralloc_endb(&sa, "/.", 2))
      sa.len -= 2;
    stralloc_replacec(&sa, '/', '\\');

    strlist_push_sa(&incdirs, &sa);
  }
  stralloc_nul(&incdirs.sa);

  section = ini_new(&section->next, "PATH_INFO");
  ini_set(section, "BuildDirPolicy", "BuildDirIsSourceDir");
  ini_set(section, "dir_src", "");
  ini_set(section, "dir_bin", "build/htc-16f876a");
  ini_set(section, "dir_tmp", "");
  ini_set(section, "dir_sin", "");
  ini_set(section, "dir_inc", incdirs.sa.s);
  ini_set(section, "dir_lib", "");
  ini_set(section, "dir_lkr", "");

  section = ini_new(&section->next, "CAT_FILTERS");
  ini_set(section, "filter_src", "*.c;*.as;*.asm;*.usb");
  ini_set(section, "filter_inc", "*.h;");
  ini_set(section, "filter_obj", "*.obj;*.p1;*.hex");
  ini_set(section, "filter_lib", "*.lib;*.lpp");
  ini_set(section, "filter_lkr", "*.unknown");

  section = cat_subfolders = ini_new(&section->next, "CAT_SUBFOLDERS");
  ini_set(cat_subfolders, "subfolder_src", "");
  ini_set(cat_subfolders, "subfolder_inc", "");
  ini_set(cat_subfolders, "subfolder_obj", "");
  ini_set(cat_subfolders, "subfolder_lib", "");
  ini_set(cat_subfolders, "subfolder_lkr", "");

  section = file_subfolders = ini_new(&section->next, "FILE_SUBFOLDERS");
  section = generated_files = ini_new(&section->next, "GENERATED_FILES");
  section = other_files = ini_new(&section->next, "OTHER_FILES");

  section = ini_new(&section->next, "FILE_INFO");
  unsigned long i = 0;

/*  buffer_puts(b, "; Number of rules: ");
  buffer_putulong(b, hmap_count(rules));
  buffer_putsflush(b, "\r\n");*/

  stralloc_zero(&incdirs.sa);

  strarray_foreach(&srcs, p) {

    stralloc_zero(&sa);
    stralloc_copy(&sa, &dirs.this.sa);
    stralloc_catc(&sa, '/');
    stralloc_cats(&sa, *p);
    stralloc_nul(&sa);

    stralloc_zero(&dirname);
    path_dirname(*p, &dirname);
    stralloc_nul(&dirname);

    s = dirname.s;
    n = str_rchrs(dirname.s, "/\\", 2);
    if(s[n])
      s += n;

    if(!str_equal(s, "."))
      strlist_push_unique(is_source(*p) ? &srcdirs : &incdirs, s);

    path_relative(sa.s, dirs.build.sa.s, &file);
    stralloc_replacec(&file, '/', '\\');
    stralloc_nul(&file);

    make_fileno(&sa, i++);

    ini_set(generated_files, sa.s, "no");
    ini_set(other_files, sa.s, "no");
    ini_set_sa(section, &sa, &file);

    stralloc_zero(&file);
    path_dirname(*p, &file);

    ini_set_sa(file_subfolders, &sa, &file);

    stralloc_free(&file);
  }

  stralloc_nul(&incdirs.sa);
  stralloc_nul(&srcdirs.sa);

  ini_set(cat_subfolders, "subfolder_inc", incdirs.sa.s);
  ini_set(cat_subfolders, "subfolder_src", srcdirs.sa.s);

  /*
    hmap_foreach(rules, it) {
      target* t = (target*)it->vals.val_chars;

      if(!is_object(t->name) || stralloc_ends(&t->prereq.sa, exts.pps))
        continue;

      debug_target(t);


      ini_set(file_subfolders, sa.s, ".");
      ini_set(generated_files, sa.s, "no");
      ini_set(other_files, sa.s, "no");
      ini_set_sa(section, &sa, &t->prereq.sa);

      i++;
    }*/
  section = ini_new(&section->next, "SUITE_INFO");
  ini_set(section, "suite_guid", suite_guid(&sa));
  ini_set(section, "suite_state", "");

  MAP_T toolcfg;
  MAP_NEW(toolcfg);
  strlist defines, tcfg, *vdefs;

  strlist_init(&defines, ',');
  strlist_init(&tcfg, ',');
  vdefs = get_var("DEFS");

  strlist_foreach(vdefs, s, n) {
    n = byte_chr(s, n, ' ');

    if(str_start(s, "-D")) {
      s += 2;
      n -= 2;
    }
    strlist_pushb(&defines, s, n);
  }
  stralloc_nul(&defines.sa);

  section = ini_new(&section->next, "TOOL_SETTINGS");
  set_value(toolcfg, "C6", 255);
  set_value(toolcfg, "DB", mplab_cfg.warning_level); //!< Warning level
  set_value(toolcfg, "DC", 9);
  MAP_SET(toolcfg, "D1", defines.sa.s);
  set_value(toolcfg, "DF", mplab_cfg.verbose_messages);     //!< verbose messages
  set_value(toolcfg, "DD", mplab_cfg.optimize_global);      //!< optimize global
  set_value(toolcfg, "C2", mplab_cfg.optimize_speed);       //!< optimize speed
  set_value(toolcfg, "C3", mplab_cfg.optimize_debug);       //!< optmize debug
  set_value(toolcfg, "DE", mplab_cfg.optimize_assembler);   //!< optimize assembler
  set_value(toolcfg, "D7", mplab_cfg.preprocess_assembler); //!< preprocess assembler
  set_value(toolcfg, "11E", 0);
  set_value(toolcfg, "121", 0);
  set_value(toolcfg, "122", 0);
  set_value(toolcfg, "123", 0);
  set_value(toolcfg, "124", 0);
  set_value(toolcfg, "125", 0);
  set_value(toolcfg, "11F", 94);
  set_value(toolcfg, "127", 0);
  MAP_SET(toolcfg, "C9", "4,3,1,2");
  set_value(toolcfg, "FE", mplab_cfg.debugger);                  //!< debugger: 39 = PicKit3, 31 = Auto
  set_value(toolcfg, "EC", mplab_cfg.clear_bss);                 //!< clear BSS
  set_value(toolcfg, "F0", mplab_cfg.keep_generated_startup_as); //!< keep generated startup.as
  set_value(toolcfg, "EF", mplab_cfg.initialize_data);           //!< initialize data
  set_value(toolcfg, "EE", 0);
  set_value(toolcfg, "104", 0);
  MAP_SET(toolcfg, "E9", "");
  set_value(toolcfg, "C4", 0);
  MAP_SET(toolcfg, "F2", "");
  MAP_SET(toolcfg, "F3", "");
  MAP_SET(toolcfg, "F4", "");
  set_value(toolcfg, "F8", mplab_cfg.calibrate_oscillator); //!< calibrate oscillator
  MAP_SET(toolcfg, "F5", "");
  set_value(toolcfg, "F9", mplab_cfg.backup_reset_condition_flags); //!< backup reset co ndition flags
  set_value(toolcfg, "FA", mplab_cfg.format_hex_file_for_download); //!< format hex file for download
  set_value(toolcfg, "FB", 0);
  set_value(toolcfg, "C0", 0);
  set_value(toolcfg, "C1", mplab_cfg.managed_stack); //!< managed stack
  set_value(toolcfg, "BD", 0);
  set_value(toolcfg, "BC", 0);
  set_value(toolcfg, "BB", 0);
  set_value(toolcfg, "BF", 0);
  set_value(toolcfg, "BE", 0);
  MAP_SET(toolcfg, "B8", "");
  set_value(toolcfg, "101", 0);
  MAP_SET(toolcfg, "103", "");
  set_value(toolcfg, "102", 0);
  MAP_SET(toolcfg, "BA", "");
  set_value(toolcfg, "FF", 0);
  set_value(toolcfg, "100", 0);
  set_value(toolcfg, "106", 0);
  set_value(toolcfg, "109", 0);
  set_value(toolcfg, "10A", 1);
  set_value(toolcfg, "10B", 0);
  set_value(toolcfg, "10C", 0);
  set_value(toolcfg, "10E", mplab_cfg.program_default_config_words); //!< program default config words
  set_value(toolcfg, "10F", 1);
  set_value(toolcfg, "110", mplab_cfg.link_in_peripheral_library); //!< link in peripheral library
  set_value(toolcfg, "118", 0);
  set_value(toolcfg, "116", 0);
  MAP_SET(toolcfg, "117", "");
  set_value(toolcfg, "10D", 0);
  set_value(toolcfg, "114", -1);
  set_value(toolcfg, "113", -1);
  set_value(toolcfg, "111", 0);
  set_value(toolcfg, "115", -1);
  set_value(toolcfg, "F5", 0);
  stralloc_zero(&sa);
  stralloc_copys(&sa, mplab_cfg.additional_command_line_options);
  stralloc_replaces(&sa, ",", "%2C");
  stralloc_replaces(&sa, " ", ",");
  stralloc_nul(&sa);
  MAP_SET(toolcfg, "E3", sa.s);                     //!< additional command line options
  set_value(toolcfg, "E5", mplab_cfg.memory_model); //!< memory model: 0=small, 1=large
  set_value(toolcfg, "E7", 0);
  set_value(toolcfg, "E8", mplab_cfg.size_of_double); //!< size of double: 0=24, 1=32
  set_value(toolcfg, "126", mplab_cfg.size_of_float); //!< size of float: 0=24, 1=32
  set_value(toolcfg, "F1", 0);
  MAP_SET(toolcfg, "F6", "");
  MAP_SET(toolcfg, "F7", "");
  set_value(toolcfg, "B9", -1);
  set_value(toolcfg, "107", 0);

  strlist_init(&tcfg, ' ');

  MAP_FOREACH(toolcfg, it) {
    stralloc_zero(&sa);
    stralloc_catb(&sa, it->key, it->key_len - 1);
    stralloc_catc(&sa, '=');
    stralloc_catb(&sa, it->vals.val_chars, it->data_len - 1);

    strlist_push_sa(&tcfg, &sa);
  }
  stralloc_nul(&tcfg.sa);

  ini_set(section, make_tool_key(&sa, ""), tcfg.sa.s);
  ini_set(section, make_tool_key(&sa, "_alt"), "yes");
  ini_set(section, make_tool_key(&sa, "000"), "");
  ini_set(section, make_tool_key(&sa, "001"), "");
  ini_set(section, make_tool_key(&sa, "002"), "");
  ini_set(section, make_tool_key(&sa, "003"), "");
  ini_set(section, make_tool_key(&sa, "004"), "");
  ini_set(section, make_tool_key(&sa, "005"), "");
  ini_set(section, make_tool_key(&sa, "006"), "");
  ini_set(section, make_tool_key(&sa, "007"), "");
  ini_set(section, make_tool_key(&sa, "008"), "");
  ini_set(section, make_tool_key(&sa, "009"), "");
  ini_set(section, make_tool_key(&sa, "010"), "");
  ini_set(section, make_tool_key(&sa, "011"), "");

  section = ini_new(&section->next, "ACTIVE_FILE_SETTINGS");
  ini_set(section, make_tool_key(&sa, "000_active"), "yes");
  ini_set(section, make_tool_key(&sa, "001_active"), "yes");
  ini_set(section, make_tool_key(&sa, "002_active"), "yes");
  ini_set(section, make_tool_key(&sa, "003_active"), "yes");
  ini_set(section, make_tool_key(&sa, "004_active"), "yes");
  ini_set(section, make_tool_key(&sa, "005_active"), "yes");
  ini_set(section, make_tool_key(&sa, "006_active"), "yes");
  ini_set(section, make_tool_key(&sa, "007_active"), "yes");
  ini_set(section, make_tool_key(&sa, "008_active"), "yes");
  ini_set(section, make_tool_key(&sa, "009_active"), "yes");
  ini_set(section, make_tool_key(&sa, "010_active"), "yes");
  ini_set(section, make_tool_key(&sa, "011_active"), "yes");

  section = ini_new(&section->next, "INSTRUMENTED_TRACE");
  ini_set(section, "enable", "0");
  ini_set(section, "transport", "0");
  ini_set(section, "format", "0");

  section = ini_new(&section->next, "CUSTOM_BUILD");
  ini_set(section, "Pre-Build", "");
  ini_set(section, "Pre-BuildEnabled", "1");
  ini_set(section, "Post-Build", "");
  ini_set(section, "Post-BuildEnabled", "1");

  ini_write(b, ini);

  stralloc_free(&sa);
  stralloc_free(&file);
}