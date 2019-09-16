#include "../vartab.h"

struct vartab vartab_root = {{NULL}, NULL, NULL, 0};
struct vartab* varstack = &vartab_root;
struct var* var_list = NULL;
