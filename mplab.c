#include "genmakefile.h"
#include "ini.h"
#include "map.h"
#include "mplab.h"

static void
make_fileno(stralloc* sa, int i) {
  stralloc_zero(sa);
  stralloc_copys(sa, "file_");
  stralloc_catuint0(sa, i, 3);
  stralloc_nul(sa);
}

static void
set_str(MAP_T map, const char* key, const char* value) {
  MAP_SET(map, (char*)key, (char*)value);
}

static void
set_int(MAP_T map, const char* key, int value) {
  stralloc v;
  stralloc_init(&v);
  stralloc_catint(&v, value);
  stralloc_nul(&v);

  set_str(map, key, v.s);
}

static int
get_suite() {
  if(!case_diffs(tools.compiler, "xc8") || str_start(tools.compiler, "xc8"))
    return 0;
  if(!case_diffs(tools.compiler, "htc") || str_start(tools.compiler, "htc"))
    return 1;
  if(!case_diffs(tools.compiler, "sdcc") || str_start(tools.compiler, "sdcc"))
    return 2;
  return 0;
}
static const char* compiler_suites[] = {
    "38171385-97B2-4EC5-BF2C-C2C027BA5B04", //!< xc8 guid
    "507D93FD-16F1-4270-980F-0C7C0207E6D3", //!< htc guid
    "DBB37D42-EAE0-43C3-A936-355514C2170D"  //!< sdcc guid
};

static const char* compiler_settings[] = {
    "F42384DA-C7ED-4A02-880F-0F5E88735CE2", //!< xc8 guid
    "3FF1D5F2-E530-4850-9F70-F61D55BD1AC9", //!< htc guid
    "43F7CD89-1294-495E-9525-F268225EB77A"  //!< sdcc guid
};

static const char*
suite_guid(stralloc* sa) {
  int suite = get_suite();
  stralloc_zero(sa);
  stralloc_catm_internal(sa, "{", compiler_suites[suite], "}", 0);
  stralloc_nul(sa);
  return sa->s;
}

static const char*
make_tool_key(stralloc* sa, const char* key) {
  int suite = get_suite();

  stralloc_zero(sa);
  stralloc_catm_internal(sa, "TS{", compiler_settings[suite], "}", key, 0);
  stralloc_nul(sa);
  return sa->s;
}

static inline int
is_debug() {
  return cfg.build_type == BUILD_TYPE_RELWITHDEBINFO || cfg.build_type == BUILD_TYPE_DEBUG;
}
static inline int
opt_size() {
  return cfg.build_type == BUILD_TYPE_MINSIZEREL;
}
static inline int
opt_speed() {
  return cfg.build_type == BUILD_TYPE_RELWITHDEBINFO || cfg.build_type == BUILD_TYPE_RELEASE;
}

static void set_debug(MAP_T map);

void
output_mplab_project(buffer* b, MAP_T _rules, MAP_T vars, const strlist* include_dirs) {
  MAP_ITER_T it;
  MAP_T toolcfg;
  strlist incdirs, srcdirs;
  const char *dir, *s, **p;
  size_t n;
  stralloc sa, file, dirname;
  mplab_config_t mplab_cfg = {.warning_level = (is_debug() ? 3 : -3),
                              .verbose_messages = 1,
                              .optimize_global = opt_size() || opt_speed(),
                              .optimize_speed = opt_speed(),
                              .optimize_debug = is_debug(),
                              .optimize_assembler = 1,
                              .preprocess_assembler = 1,
                              .debugger = 39,
                              .clear_bss = 1,
                              .keep_generated_startup_as = 1,
                              .initialize_data = 0,
                              .calibrate_oscillator = 0,
                              .backup_reset_condition_flags = 0,
                              .format_hex_file_for_download = 1,
                              .managed_stack = 1,
                              .program_default_config_words = 0,
                              .link_in_peripheral_library = 0,
                              .additional_command_line_options = "--output=default,-inhx032 --output=+mcof,-elf",
                              .memory_model = 1,
                              .size_of_double = 1,
                              .size_of_float = 1};

  ini_section_t *ini, *section, *cat_subfolders, *file_subfolders, *generated_files, *other_files, *file_info,
      *active_file_settings, *tool_settings;

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
  ini_set(section, "BuildDirPolicy", 1 ? "BuildDirIsProjectDir" : "BuildDirIsSourceDir");
  ini_set(section, "dir_src", "");

  stralloc_zero(&sa);

  stralloc_copys(&sa, "..\\..\\bin\\");
  stralloc_catb(&sa, tools.compiler, 3);
  stralloc_catc(&sa, '-');
  stralloc_cat(&sa, &cfg.chip);
  stralloc_nul(&sa);

  ini_set(section, "dir_bin", sa.s);
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
  unsigned int i = 0, num_sources = 0;

  /*  buffer_puts(b, "; Number of rules: ");
    buffer_putuint(b, hmap_count(rules));
    buffer_putsflush(b, "\r\n");*/

  stralloc_zero(&incdirs.sa);

  strarray_foreach(&srcs, p) {

    s = *p;

    if(!is_source(s) && num_sources == 0) {
      num_sources = i;
    }

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

    debug_sa("sa", &sa);
    debug_sa("dirs.build", &dirs.build.sa);
    debug_sa("dirs.work", &dirs.work.sa);
    debug_sa("dirs.out", &dirs.out.sa);

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
  vdefs = get_var("CPPFLAGS");
  strlist_foreach(vdefs, s, n) {
    n = byte_chr(s, n, ' ');
    if(str_start(s, "-D") || str_start(s, "-d")) {
      s += 2;
      n -= 2;

      if(s[str_chr(s, '$')] == '\0')
        strlist_pushb_unique(&defines, s, n);
    }
  }
  stralloc_nul(&defines.sa);

  section = tool_settings = ini_new(&section->next, "TOOL_SETTINGS");
  section = active_file_settings = ini_new(&section->next, "ACTIVE_FILE_SETTINGS");

  if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
    set_debug(toolcfg);

  set_int(toolcfg, "DB", mplab_cfg.warning_level); //!< Warning level
  set_str(toolcfg, "D1", defines.sa.s);
  set_int(toolcfg, "DF", mplab_cfg.verbose_messages);     //!< verbose messages
  set_int(toolcfg, "DD", mplab_cfg.optimize_global);      //!< optimize global
  set_int(toolcfg, "C2", mplab_cfg.optimize_speed);       //!< optimize speed
  set_int(toolcfg, "C3", mplab_cfg.optimize_debug);       //!< optmize debug
  set_int(toolcfg, "DE", mplab_cfg.optimize_assembler);   //!< optimize assembler
  set_int(toolcfg, "D7", mplab_cfg.preprocess_assembler); //!< preprocess assembler

  set_int(toolcfg, "FE", mplab_cfg.debugger);                      //!< debugger: 39 = PicKit3, 31 = Auto
  set_int(toolcfg, "EC", mplab_cfg.clear_bss);                     //!< clear BSS
  set_int(toolcfg, "F0", mplab_cfg.keep_generated_startup_as);     //!< keep generated startup.as
  set_int(toolcfg, "EF", mplab_cfg.initialize_data);               //!< initialize data
  set_int(toolcfg, "F8", mplab_cfg.calibrate_oscillator);          //!< calibrate oscillator
  set_int(toolcfg, "F9", mplab_cfg.backup_reset_condition_flags);  //!< backup reset co ndition flags
  set_int(toolcfg, "FA", mplab_cfg.format_hex_file_for_download);  //!< format hex file for download
  set_int(toolcfg, "C1", mplab_cfg.managed_stack);                 //!< managed stack
  set_int(toolcfg, "10E", mplab_cfg.program_default_config_words); //!< program default config words
  set_int(toolcfg, "110", mplab_cfg.link_in_peripheral_library);   //!< link in peripheral library

  stralloc_zero(&sa);
  stralloc_copys(&sa, mplab_cfg.additional_command_line_options);
  stralloc_replaces(&sa, ",", "%2C");
  stralloc_replaces(&sa, " ", ",");
  stralloc_nul(&sa);

  set_str(toolcfg, "E3", sa.s);                     //!< additional command line options
  set_int(toolcfg, "E5", mplab_cfg.memory_model);   //!< memory model: 0=small, 1=large
  set_int(toolcfg, "E8", mplab_cfg.size_of_double); //!< size of double: 0=24, 1=32
  set_int(toolcfg, "126", mplab_cfg.size_of_float); //!< size of float: 0=24, 1=32
                                                    /*  set_int(toolcfg, "F1", 0);
                                                      set_str(toolcfg, "F6", "");
                                                      set_str(toolcfg, "F7", "");
                                                      set_int(toolcfg, "B9", -1);
                                                      set_int(toolcfg, "107", 0);
                                                    */
  strlist_init(&tcfg, ' ');

  MAP_FOREACH(toolcfg, it) {
    stralloc_zero(&sa);
    stralloc_catb(&sa, it->key, it->key_len - 1);
    stralloc_catc(&sa, '=');
    stralloc_catb(&sa, it->vals.val_chars, it->data_len - 1);

    strlist_push_sa(&tcfg, &sa);
  }
  stralloc_nul(&tcfg.sa);

  ini_set(tool_settings, make_tool_key(&sa, ""), tcfg.sa.s);

  if(get_suite() <= 1) {

    for(i = 0; i < num_sources; i++) {
      size_t len;
      stralloc_zero(&file);
      stralloc_catlong0(&file, i, 3);

      len = file.len;
      stralloc_nul(&file);

      ini_set(tool_settings, make_tool_key(&sa, file.s), "");

      stralloc_cats(&file, "_alt");
      stralloc_nul(&file);
      ini_set(tool_settings, make_tool_key(&sa, file.s), "yes");

      file.len = len;
      stralloc_cats(&file, "_active");
      stralloc_nul(&file);
      ini_set(active_file_settings, make_tool_key(&sa, file.s), "yes");
    }
  } else {
    ini_set(section, make_tool_key(&sa, "_alt"), "yes");
  }

  /*  ini_set(section, make_tool_key(&sa, "000_active"), "yes");
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
    ini_set(section, make_tool_key(&sa, "011_active"), "yes");*/

  section = ini_new(&section->next, "INSTRUMENTED_TRACE");
  ini_set_long(section, "enable", 0);
  ini_set_long(section, "transport", 0);
  ini_set_long(section, "format", 0);

  section = ini_new(&section->next, "CUSTOM_BUILD");
  ini_set(section, "Pre-Build", "");
  ini_set_long(section, "Pre-BuildEnabled", 1);
  ini_set(section, "Post-Build", "");
  ini_set_long(section, "Post-BuildEnabled", 1);

  ini_write(b, ini);

  stralloc_free(&sa);
  stralloc_free(&file);
}

static void
set_debug(MAP_T map) {
  set_str(map, "C6", "127");
  set_str(map, "DB", "3");
  set_str(map, "DC", "9");
  /// set_str(map, "D1",
  /// "HAVE_COMPARATOR=1,SOFTSER_TIMER=2,UART_BAUD=38400,USE_SOFTPWM=1,USE_SOFTSER=1,USE_TIMER0=1,USE_TIMER1=1,USE_TIMER2=1,_XTAL_FREQ=20000000,__XC=1,__18f2550=1");
  set_str(map, "DF", "1");
  set_str(map, "DD", "1");
  set_str(map, "C2", "0");
  set_str(map, "C3", "1");
  set_str(map, "DE", "1");
  set_str(map, "D7", "1");
  set_str(map, "11E", "0");
  set_str(map, "121", "0");
  set_str(map, "122", "0");
  set_str(map, "123", "0");
  set_str(map, "124", "0");
  set_str(map, "125", "0");
  set_str(map, "11F", "94");
  set_str(map, "127", "0");
  set_str(map, "C9", "4,3,1,2");
  set_str(map, "FE", "39");
  set_str(map, "EC", "1");
  set_str(map, "F0", "1");
  set_str(map, "EF", "0");
  set_str(map, "EE", "0");
  set_str(map, "104", "0");
  set_str(map, "E9", "");
  set_str(map, "C4", "0");
  set_str(map, "F2", "");
  set_str(map, "F3", "");
  set_str(map, "F4", "");
  set_str(map, "F8", "0");
  set_str(map, "F5", "0");
  set_str(map, "F9", "0");
  set_str(map, "FA", "1");
  set_str(map, "FB", "0");
  set_str(map, "C0", "0");
  set_str(map, "C1", "1");
  set_str(map, "BD", "0");
  set_str(map, "BC", "0");
  set_str(map, "BB", "0");
  set_str(map, "BF", "0");
  set_str(map, "BE", "0");
  set_str(map, "B8", "");
  set_str(map, "101", "0");
  set_str(map, "103", "");
  set_str(map, "102", "0");
  set_str(map, "BA", "");
  set_str(map, "FF", "0");
  set_str(map, "100", "0");
  set_str(map, "106", "0");
  set_str(map, "109", "0");
  set_str(map, "10A", "1");
  set_str(map, "10B", "0");
  set_str(map, "10C", "0");
  set_str(map, "10E", "0");
  set_str(map, "10F", "1");
  set_str(map, "110", "1");
  set_str(map, "118", "0");
  set_str(map, "116", "0");
  set_str(map, "117", "");
  set_str(map, "10D", "0");
  set_str(map, "114", "-1");
  set_str(map, "113", "-1");
  set_str(map, "111", "0");
  set_str(map, "115", "-1");
  set_str(map, "F5", "0");
  set_str(map, "E3", "--output=default%2C-inhx032,--output=+mcof%2C-elf");
  set_str(map, "E5", "1");
  set_str(map, "E7", "0");
  set_str(map, "E8", "1");
  set_str(map, "126", "1");
  set_str(map, "F1", "0");
  set_str(map, "F6", "");
  set_str(map, "F7", "");
  set_str(map, "B9", "-1");
  set_str(map, "107", "0");
}