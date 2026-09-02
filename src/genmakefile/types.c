#include "types.h"
#include "../../debug.h"

/**
 * @brief      Dump a config_t to debug_buf
 *
 * @param      cfg   Config
 */
void
config_dump(const config_t* cfg) {
  static const char* const arch_names[] = {"X86", "ARM", "PIC"};
  static const char* const bits_names[] = {"14", "16", "32", "64"};
  static const char* const os_names[] = {"WIN", "MAC", "LINUX"};
  static const char* const systype_names[] = {"NTOS", "UNIX"};

  buffer_puts(debug_buf, "Config");
  buffer_putm_internal(debug_buf, " arch=", arch_names[cfg->mach.arch], NULL);
  buffer_putm_internal(debug_buf, " bits=", bits_names[cfg->mach.bits], NULL);
  buffer_putm_internal(debug_buf, " os=", os_names[cfg->sys.os], NULL);
  buffer_putm_internal(debug_buf, " systype=", systype_names[cfg->sys.type], NULL);

  if(cfg->chip.len) {
    buffer_puts(debug_buf, " chip=\"");
    buffer_putsa(debug_buf, &cfg->chip);
    buffer_putc(debug_buf, '"');
  }

  buffer_putm_internal(debug_buf, " build_type=", 0, NULL);
  buffer_putlong(debug_buf, (long)cfg->build_type);
  buffer_putm_internal(debug_buf, " lang=", 0, NULL);
  buffer_putlong(debug_buf, (long)cfg->lang);

  buffer_putnlflush(debug_buf);
}

/**
 * @brief      Dump an exts_t to debug_buf
 *
 * @param      exts  Extensions
 */
void
exts_dump(const exts_t* exts) {
  buffer_puts(debug_buf, "Exts");
  buffer_putm_internal(debug_buf, " src=", exts->src ? exts->src : "", NULL);
  buffer_putm_internal(debug_buf, " inc=", exts->inc ? exts->inc : "", NULL);
  buffer_putm_internal(debug_buf, " obj=", exts->obj ? exts->obj : "", NULL);
  buffer_putm_internal(debug_buf, " lib=", exts->lib ? exts->lib : "", NULL);
  buffer_putm_internal(debug_buf, " slib=", exts->slib ? exts->slib : "", NULL);
  buffer_putm_internal(debug_buf, " bin=", exts->bin ? exts->bin : "", NULL);
  buffer_putm_internal(debug_buf, " pps=", exts->pps ? exts->pps : "", NULL);
  buffer_putnlflush(debug_buf);
}

/**
 * @brief      Dump a commands_t to debug_buf
 *
 * @param      cmds  Commands
 */
void
commands_dump(const commands_t* cmds) {
  buffer_puts(debug_buf, "Commands");

  if(cmds->preprocess.len) {
    buffer_puts(debug_buf, " preprocess=\"");
    buffer_putsa(debug_buf, &cmds->preprocess);
    buffer_putc(debug_buf, '"');
  }

  if(cmds->compile.len) {
    buffer_puts(debug_buf, " compile=\"");
    buffer_putsa(debug_buf, &cmds->compile);
    buffer_putc(debug_buf, '"');
  }

  if(cmds->lib.len) {
    buffer_puts(debug_buf, " lib=\"");
    buffer_putsa(debug_buf, &cmds->lib);
    buffer_putc(debug_buf, '"');
  }

  if(cmds->link.len) {
    buffer_puts(debug_buf, " link=\"");
    buffer_putsa(debug_buf, &cmds->link);
    buffer_putc(debug_buf, '"');
  }

  if(cmds->link_module.len) {
    buffer_puts(debug_buf, " link_module=\"");
    buffer_putsa(debug_buf, &cmds->link_module);
    buffer_putc(debug_buf, '"');
  }

  if(cmds->mkdir.len) {
    buffer_puts(debug_buf, " mkdir=\"");
    buffer_putsa(debug_buf, &cmds->mkdir);
    buffer_putc(debug_buf, '"');
  }

  if(cmds->delete.len) {
    buffer_puts(debug_buf, " delete=\"");
    buffer_putsa(debug_buf, &cmds->delete);
    buffer_putc(debug_buf, '"');
  }

  buffer_putnlflush(debug_buf);
}
