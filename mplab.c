#include "genmakefile.h"
#include "ini.h"
#include "map.h"

static void
make_fileno(stralloc* sa, int i) {
  stralloc_zero(sa);
  stralloc_copys(sa, "file_");
  stralloc_catulong0(sa, i, 3);
  stralloc_nul(sa);
}

void
output_mplab_project(buffer* b, MAP_T _rules, MAP_T vars, const strlist* include_dirs) {
  MAP_ITER_T it;
  strlist incdirs, srcdirs;
  const char *dir, *s, **p;
  size_t n;
  stralloc sa, file, dirname;

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

  buffer_puts(b, "; Number of rules: ");
  buffer_putulong(b, hmap_count(rules));
  buffer_putsflush(b, "\r\n");

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
    stralloc_nul(&file);

    make_fileno(&sa, i++);

    ini_set(generated_files, sa.s, "no");
    ini_set(other_files, sa.s, "no");
    ini_set_sa(section, &sa, &file);

    stralloc_zero(&file);
    path_dirname(*p, &file);
    stralloc_replacec(&file, '/', '\\');

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
  ini_set(section, "suite_guid", "{38171385-97B2-4EC5-BF2C-C2C027BA5B04}");
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
  MAP_SET(toolcfg, "C6", "255");
  MAP_SET(toolcfg, "DB", "3");
  MAP_SET(toolcfg, "DC", "9");
  MAP_SET(toolcfg, "D1", defines.sa.s);
  MAP_SET(toolcfg, "DF", "1");
  MAP_SET(toolcfg, "DD", "1");
  MAP_SET(toolcfg, "C2", "1");
  MAP_SET(toolcfg, "C3", "1");
  MAP_SET(toolcfg, "DE", "1");
  MAP_SET(toolcfg, "D7", "1");
  MAP_SET(toolcfg, "11E", "0");
  MAP_SET(toolcfg, "121", "0");
  MAP_SET(toolcfg, "122", "0");
  MAP_SET(toolcfg, "123", "0");
  MAP_SET(toolcfg, "124", "0");
  MAP_SET(toolcfg, "125", "0");
  MAP_SET(toolcfg, "11F", "94");
  MAP_SET(toolcfg, "127", "0");
  MAP_SET(toolcfg, "C9", "4,3,1,2");
  MAP_SET(toolcfg, "FE", "39");
  MAP_SET(toolcfg, "EC", "1");
  MAP_SET(toolcfg, "F0", "1");
  MAP_SET(toolcfg, "EF", "0");
  MAP_SET(toolcfg, "EE", "0");
  MAP_SET(toolcfg, "104", "0");
  MAP_SET(toolcfg, "E9", "");
  MAP_SET(toolcfg, "C4", "0");
  MAP_SET(toolcfg, "F2", "");
  MAP_SET(toolcfg, "F3", "");
  MAP_SET(toolcfg, "F4", "");
  MAP_SET(toolcfg, "F8", "0");
  MAP_SET(toolcfg, "F5", "");
  MAP_SET(toolcfg, "F9", "0");
  MAP_SET(toolcfg, "FA", "1");
  MAP_SET(toolcfg, "FB", "0");
  MAP_SET(toolcfg, "C0", "0");
  MAP_SET(toolcfg, "C1", "1");
  MAP_SET(toolcfg, "BD", "0");
  MAP_SET(toolcfg, "BC", "0");
  MAP_SET(toolcfg, "BB", "0");
  MAP_SET(toolcfg, "BF", "0");
  MAP_SET(toolcfg, "BE", "0");
  MAP_SET(toolcfg, "B8", "");
  MAP_SET(toolcfg, "101", "0");
  MAP_SET(toolcfg, "103", "");
  MAP_SET(toolcfg, "102", "0");
  MAP_SET(toolcfg, "BA", "");
  MAP_SET(toolcfg, "FF", "0");
  MAP_SET(toolcfg, "100", "0");
  MAP_SET(toolcfg, "106", "0");
  MAP_SET(toolcfg, "109", "0");
  MAP_SET(toolcfg, "10A", "1");
  MAP_SET(toolcfg, "10B", "0");
  MAP_SET(toolcfg, "10C", "0");
  MAP_SET(toolcfg, "10E", "0");
  MAP_SET(toolcfg, "10F", "1");
  MAP_SET(toolcfg, "110", "0");
  MAP_SET(toolcfg, "118", "0");
  MAP_SET(toolcfg, "116", "0");
  MAP_SET(toolcfg, "117", "");
  MAP_SET(toolcfg, "10D", "0");
  MAP_SET(toolcfg, "114", "-1");
  MAP_SET(toolcfg, "113", "-1");
  MAP_SET(toolcfg, "111", "0");
  MAP_SET(toolcfg, "115", "-1");
  MAP_SET(toolcfg, "F5", "0");
  MAP_SET(toolcfg, "E3", "--output=default%2C-inhx032,--output=+mcof%2C-elf");
  MAP_SET(toolcfg, "E5", "1");
  MAP_SET(toolcfg, "E7", "0");
  MAP_SET(toolcfg, "E8", "1");
  MAP_SET(toolcfg, "126", "1");
  MAP_SET(toolcfg, "F1", "0");
  MAP_SET(toolcfg, "F6", "");
  MAP_SET(toolcfg, "F7", "");
  MAP_SET(toolcfg, "B9", "-1");
  MAP_SET(toolcfg, "107", "0");

  strlist_init(&tcfg, ' ');

  MAP_FOREACH(toolcfg, it) {
    stralloc_zero(&sa);
    stralloc_catb(&sa, it->key, it->key_len - 1);
    stralloc_catc(&sa, '=');
    stralloc_catb(&sa, it->vals.val_chars, it->data_len - 1);

    strlist_push_sa(&tcfg, &sa);
  }
  stralloc_nul(&tcfg.sa);

  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}", tcfg.sa.s);
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}_alt", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}000", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}001", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}002", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}003", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}004", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}005", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}006", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}007", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}008", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}009", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}010", "");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}011", "");

  section = ini_new(&section->next, "ACTIVE_FILE_SETTINGS");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}000_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}001_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}002_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}003_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}004_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}005_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}006_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}007_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}008_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}009_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}010_active", "yes");
  ini_set(section, "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}011_active", "yes");

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