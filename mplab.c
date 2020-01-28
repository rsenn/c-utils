#include "genmakefile.h"
#include "ini.h"
#include "map.h"

void
output_mplab_project(buffer* b, MAP_T _rules, MAP_T vars, const strlist* include_dirs) {
  MAP_ITER_T it;
  strlist incdirs;
  const char *dir, *s, **p;
  size_t n;
  stralloc sa, file;

  ini_section_t *ini, *section, *file_subfolders, *generated_files, *other_files, *file_info;

  stralloc_init(&sa);
  stralloc_init(&file);

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

  strlist_foreach_s(include_dirs, dir) {

    stralloc_zero(&sa);
    path_relative(dir, dirs.build.sa.s, &sa);
    stralloc_replacec(&sa, '/', '\\');
    if(stralloc_ends(&sa, "\\."))
      sa.len -= 2;
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

  section = ini_new(&section->next, "CAT_SUBFOLDERS");
  ini_set(section, "subfolder_src", "");
  ini_set(section, "subfolder_inc", "");
  ini_set(section, "subfolder_obj", "");
  ini_set(section, "subfolder_lib", "");
  ini_set(section, "subfolder_lkr", "");

  section = file_subfolders = ini_new(&section->next, "FILE_SUBFOLDERS");
  section = generated_files = ini_new(&section->next, "GENERATED_FILES");
  section = other_files = ini_new(&section->next, "OTHER_FILES");

  section = ini_new(&section->next, "FILE_INFO");
  unsigned long i = 0;

  buffer_puts(b, "; Number of rules: ");
  buffer_putulong(b, hmap_count(rules));
  buffer_putnlflush(b);

  strarray_foreach(&srcs, p) {

 //   debug_s("source", s);

    path_relative(*p, &dirs.work, &file);
    stralloc_nul(&file);

    stralloc_zero(&sa);
    stralloc_copys(&sa, "file_");
    stralloc_catulong0(&sa, i, 3);
    stralloc_nul(&sa);

    ini_set(generated_files, sa.s, "no");
    ini_set(other_files, sa.s, "no");
    ini_set_sa(section, &sa, &file);

    stralloc_zero(&file);
    path_dirname(*p, &file);

    ini_set_sa(file_subfolders, &sa, &file);

    stralloc_free(&file);

    i++;
  }
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

  section = ini_new(&section->next, "TOOL_SETTINGS");
  ini_set(
      section,
      "TS{F42384DA-C7ED-4A02-880F-0F5E88735CE2}",
      "C6=255 DB=0 DC=9 D1=USE_HD44780_LCD=1 DF=0 DD=1 C2=0 C3=1 DE=1 D7=1 11E=0 121=0 122=0 123=0 124=0 125=0 11F=94 "
      "127=0 C9=3,2,1 FE=38 EC=1 F0=0 EF=1 EE=0 104=0 E9= C4=0 F2= F3= F4= F8=1 F5= F9=0 FA=1 FB=0 C0=0 C1=0 BD=0 BC=0 "
      "BB=0 BF=0 BE=0 B8= 101=0 103= 102=0 BA= FF=0 100=0 106=0 109=0 10A=1 10B=0 10C=0 10E=0 10F=0 110=1 118=0 116=0 "
      "117= 10D=0 114=-1 113=-1 111=0 115=-1 F5=0 E5=0 E7=0 E8=1 126=1 F1=0 F6= F7= B9=-1 107=0 ");
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