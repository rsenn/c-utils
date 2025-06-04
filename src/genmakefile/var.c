#include "var.h"
#include "../../lib/str.h"
#include "../../genmakefile.h"

MAP_T vars;
linklib_fmt *format_linklib_fn = 0, *format_linkdir_fn = &format_linkdir_switch;

/**
 * @brief      Output library name (+".lib")
 *
 * @param      libname  Library name
 * @param      out      Output
 */
void
format_linklib_lib(const char* libname, stralloc* out) {
  stralloc_cats(out, libpfx);
  stralloc_cats(out, libname);
  stralloc_cats(out, exts.lib);
}

/**
 * @brief      Output library name (+ leading "-l")
 *
 * @param      libname  Library name
 * @param      out      Output
 */
void
format_linklib_switch(const char* libname, stralloc* out) {
  stralloc_cats(out, "-l");
  stralloc_cats(out, libname);
  stralloc_replaces(out, "lib", "");
}

/**
 * @brief      { function_description }
 *
 * @param      libname  Library name
 * @param      out      Output
 */
void
format_linklib_dummy(const char* libname, stralloc* out) {}

/**
 * @brief      Output library directory (+ leading "-L")
 *
 * @param[in]  libdir  The libdir
 * @param      out     Output
 */
void
format_linkdir_switch(const char* libdir, stralloc* out) {
  stralloc_cats(out, "-L");
  stralloc_cats(out, libdir);
}

/**
 * @defgroup   var functions
 * @{
 * /
 * /**
 *
 * @brief      Checks whether variable is set
 *
 * @param      name  Variable name
 *
 * @return     1 if set, 0 if not
 */
int
var_isset(const char* name) {
  return !!MAP_GET(vars, name, str_len(name) + 1);
}

/**
 * @brief      Find or create variable
 *
 * @param      name  Variable name
 * @param      psa   Path separator for arguments
 *
 * @return    var_t struct
 */
var_t*
var_list(const char* name, char psa) {
  MAP_PAIR_T t;

  if(!MAP_SEARCH(vars, name, str_len(name) + 1, &t)) {
    var_t var;
    char s = (name[0] >= 'A' && name[0] <= 'Z') ? ' ' : psa;

    var.serial = 0;

    strlist_init(&var.value, s);

    MAP_INSERT2(vars, name, str_len(name) + 1, &var, sizeof(strlist));
    MAP_SEARCH(vars, name, str_len(name) + 1, &t);
  }

  return (var_t*)MAP_ITER_VALUE(t);
}

/**
 * @brief      Get variable value by name
 *
 * @param[in]  name  Variable name
 *
 * @return     Value
 */
const char*
var_get(const char* name) {
  var_t* v = var_list(name, ' ');
  strlist_nul(&v->value);
  return v->value.sa.s;
}

/**
 * @brief      Set variable value
 *
 * @param[in]  name   Variable name
 * @param[in]  value  New value
 * @param[in]  vlen   Value length
 *
 * @return     var_t struct
 */
var_t*
var_setb(const char* name, const char* value, size_t vlen) {
  var_t* var;
  var = var_list(name, ' ');
  stralloc_zero(&var->value.sa);
  stralloc_copyb(&var->value.sa, value, vlen);

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, "var_setb(", name, ", \"", 0);
  buffer_put(buffer_2, value, vlen);
  buffer_puts(buffer_2, "\")");
  buffer_putnlflush(buffer_2);
#endif

  return var;
}

/**
 * @brief       Set variable
 *
 * @param      name   Variable name
 * @param      value  Value
 *
 * @return     var_t struct
 */
var_t*
var_set(const char* name, const char* value) {
  return var_setb(name, value, str_len(value));
}

/**
 * @brief     Unset variable
 *
 * @param[in]  name  Variable name
 */
void
var_unset(const char* name) {
  if(var_isset(name))
    MAP_DELETE(vars, (void*)name, str_len(name) + 1);
}

/**
 * @brief      Add value to variable
 *
 * @param      name   Variable name
 * @param      value  Value
 */
void
var_push(const char* name, const char* value) {
  var_t* var = var_list(name, ' ');
  strlist_push_unique(&var->value, value);
}

/**
 * @brief      Add value to variable
 *
 * @param      name   Variable name
 * @param      value  Value
 */
void
var_push_sa(const char* name, stralloc* value) {
  var_t* var = var_list(name, ' ');
  strlist_push_unique_sa(&var->value, value);
}

/**
 * @brief      Add library spec to variable
 *
 * @param      name  Variable name
 * @param      lib   Library name
 */
void
push_lib(const char* name, const char* lib) {
  stralloc sa;

  stralloc_init(&sa);

  if(format_linklib_fn) {
    format_linklib_fn(lib, &sa);
    var_push_sa(name, &sa);
  }

  stralloc_free(&sa);
}

/**
 * @brief      Add library directory to variable
 *
 * @param      name  Variable name
 * @param      dir   Directory
 */
void
push_linkdir(const char* name, const char* dir) {
  stralloc sa;

  stralloc_init(&sa);

  if(format_linkdir_fn) {
    format_linkdir_fn(dir, &sa);
    var_push_sa(name, &sa);
  }

  stralloc_free(&sa);
}

/**
 * @brief      Add library to DEFS and LIBS variable
 *
 * @param[in]  lib   Library name
 */
void
with_lib(const char* lib) {
  stralloc def, lib64;

  stralloc_init(&def);
  stralloc_init(&lib64);

  stralloc_copys(&def, "-DHAVE_");

  if(str_find(lib, "lib") == str_len(lib))
    stralloc_cats(&def, "LIB");

  stralloc_cats(&def, lib);
  stralloc_cats(&def, "=1");
  byte_upper(def.s, def.len);
  var_push_sa("DEFS", &def);
  stralloc_copys(&lib64, lib);
  stralloc_cats(&lib64, "$(L64)");
  stralloc_nul(&lib64);
  push_lib("LIBS", lib64.s);

  stralloc_free(&def);
  stralloc_free(&lib64);
}

/**
 * @brief      Pushes a define.
 *
 * @param[in]  def  Definition name
 */
void
push_define(const char* def) {
  stralloc sa;
  stralloc_init(&sa);

  stralloc_copys(&sa, "-D");
  stralloc_cats(&sa, def);
  stralloc_nul(&sa);
  var_push_sa("DEFS", &sa);

  stralloc_free(&sa);
}

/**
 * @brief Substitues
 * @param name [description]
 * @param out  [description]
 * @param in   [description]
 */
void
var_subst_sa(const char* name, stralloc* out, const stralloc* in, const char* prefix, const char* suffix) {
  if(!prefix)
    prefix = "$(";
  if(!suffix)
    suffix = ")";

  const char* value;

  stralloc_zero(out);

  if((value = var_get(name)) && value[0]) {
    size_t vlen = str_len(value);

    vlen = byte_trimr(value, vlen, " \t\v\n", 3);

    for(size_t i = 0; i < in->len;) {
      if(i + vlen <= in->len) {
        if(byte_equal(&in->s[i], vlen, value)) {
          stralloc_cats(out, prefix);
          stralloc_cats(out, name);
          stralloc_cats(out, suffix);

          i += vlen;
          continue;
        }
      }

      stralloc_catc(out, in->s[i++]);
    }
  }
}

/**
 * @}
 */
