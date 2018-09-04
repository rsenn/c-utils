#include "../var.h"
#include "../vartab.h"
#include "../buffer.h"

/* ----------------------------------------------------------------------- */
void
vartab_dump(struct vartab* vartab) {
  unsigned int i;
  struct var* var;

  /*          buffer_puts(buffer_1, "44: ");
            buffer_putulong(buffer_1, (long)(vartab->table[44]));
            buffer_putnlflush(buffer_1);
            buffer_puts(buffer_1, "46: ");
            buffer_putulong(buffer_1, (long)(vartab->table[46]));
            buffer_putnlflush(buffer_1);*/

  buffer_puts(
      buffer_1,
      "address  name                     value                   nlen offs vlen lev buck lexhash          rndhash\n");
  buffer_puts(buffer_1,
              "--------------------------------------------------------------------------------------------------------"
              "-----------\n");

  if(vartab) {
    for(; vartab; vartab = vartab->parent) {
      buffer_puts(buffer_1, "level: ");
      buffer_putulong(buffer_1, vartab->level);
      buffer_puts(buffer_1,
                  "\n=================================================================================================="
                  "=================");
      buffer_putnlflush(buffer_1);

      for(i = 0; i < (unsigned int)VARTAB_BUCKETS; i++) {
        for(var = vartab->table[i]; var; var = var->bnext) {
          var_dump(var);
        }
      }
    }
  } else {
    for(var = var_list; var; var = var->gnext) var_dump(var);
  }
}
