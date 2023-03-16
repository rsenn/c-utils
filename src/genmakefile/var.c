#include "var.h"

MAP_T vars;

/**
 * @defgroup var functions
 * @{
 */
/**
 * @brief var_isset
 * @param name
 * @return
 */
int
var_isset(const char* name) {
  return !!MAP_GET(vars, name, str_len(name) + 1);
}

/**
 * @brief var_list  Find or create variable
 * @param name
 * @return
 */
var_t*
var_list(const char* name, char pathsep_args) {
  MAP_PAIR_T t;
  if(!MAP_SEARCH(vars, name, str_len(name) + 1, &t)) {
    var_t var;
    var.serial = 0;
    strlist_init(&var.value, (name[0] >= 'A' && name[0] <= 'Z') ? ' ' : pathsep_args);
    MAP_INSERT(vars, name, str_len(name) + 1, &var, sizeof(strlist));
    MAP_SEARCH(vars, name, str_len(name) + 1, &t);
  }
  return (var_t*)MAP_ITER_VALUE(t);
}

/**
 * @brief var
 * @param name
 * @return
 */
const char*
var_get(const char* name) {
  var_t* v = var_list(name, ' ');
  strlist_nul(&v->value);
  return v->value.sa.s;
}

var_t*
var_setb(const char* name, const char* value, size_t vlen) {
  var_t* var;
  var = var_list(name, ' ');
  stralloc_zero(&var->value.sa);
  stralloc_copyb(&var->value.sa, value, vlen);

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(buffer_2, "var_setb(", name, ", \"", 0);
  buffer_put(buffer_2, value, vlen);
  buffer_puts(buffer_2, "\")");
  buffer_putnlflush(buffer_2);
#endif
  return var;
}

/**
 * @brief var_set  Set variable
 * @param name
 * @param value
 * @return
 */
var_t*
var_set(const char* name, const char* value) {
  return var_setb(name, str_len(name), value);
}

void
var_unset(const char* name) {
  if(var_isset(name))
    MAP_DELETE(vars, (void*)name, str_len(name) + 1);
}

/**
 * @brief var_push  Add value to
 * variable
 * @param name
 * @param value
 */
void
var_push(const char* name, const char* value) {
  var_t* var = var_list(name, ' ');
  strlist_push_unique(&var->value, value);
}

/**
 * @brief var_push_sa
 * @param name
 * @param value
 */
void
var_push_sa(const char* name, stralloc* value) {
  var_t* var = var_list(name, ' ');
  strlist_push_unique_sa(&var->value, value);
}
