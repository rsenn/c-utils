#include "../fmt.h"
#include "../var.h"
#include "../buffer.h"

/* dump a variable entry
 * ----------------------------------------------------------------------- */
void
var_dump(struct var* var) {
  char numbuf[FMT_XLONG * 2];
  size_t n;

  /* var struct address */
  n = fmt_xlong(numbuf, (size_t)var);
  buffer_putnspace(buffer_1, 8 - n);
  buffer_put(buffer_1, numbuf, n);
  buffer_putspace(buffer_1);

  /* variable name */
  if(var->len > 24) {
    buffer_put(buffer_1, var->sa.s, 21);
    buffer_puts(buffer_1, "...");
  } else {
    buffer_put(buffer_1, var->sa.s, var->len);
    buffer_putnspace(buffer_1, 24 - var->len);
  }

  /* variable value */
  n = var_vlen(var->sa.s);

  if(n) {
    unsigned int i, l, rl, rn;
    rn = var->sa.len - var->offset;
    rl = (rn > 24 ? 21 : rn);
    l = (n > 24 ? 21 : n);

    for(i = 0; i < rl; i++) {
      if(var->sa.s[var->offset + i] != '\n' && var->sa.s[var->offset + i] != '\t')
        buffer_put(buffer_1, &var->sa.s[var->offset + i], 1);
      else
        buffer_putspace(buffer_1);
    }

    if(rl < rn) {
      buffer_puts(buffer_1, "...");
      rl += 3;
      l += 3;
    }

    n = 24 - rl;
  } else
    n = 24;

  buffer_putnspace(buffer_1, n);

  /* name length */
  n = fmt_ulong(numbuf, var->len);
  buffer_putnspace(buffer_1, 5 - n);
  buffer_put(buffer_1, numbuf, n);

  /* value offset */
  n = fmt_ulong(numbuf, var->offset);
  buffer_putnspace(buffer_1, 5 - n);
  buffer_put(buffer_1, numbuf, n);

  /* total length */
  n = fmt_ulong(numbuf, var->sa.len);
  buffer_putnspace(buffer_1, 5 - n);
  buffer_put(buffer_1, numbuf, n);

  /* vartab level */
  n = fmt_ulong(numbuf, var->table->level);
  buffer_putnspace(buffer_1, 4 - n);
  buffer_put(buffer_1, numbuf, n);

  /* vartab bucket */
  n = fmt_ulong(numbuf, (var->rndhash % VARTAB_BUCKETS));
  buffer_putnspace(buffer_1, 5 - n);
  buffer_put(buffer_1, numbuf, n);

  buffer_putspace(buffer_1);

  /* lexical hash */
  n = fmt_xlonglong(numbuf, var->lexhash);
  buffer_putnspace(buffer_1, 16 - n);
  buffer_put(buffer_1, numbuf, n);
  buffer_putspace(buffer_1);

  /* randomized hash */
  n = fmt_xlonglong(numbuf, var->rndhash);
  buffer_putnspace(buffer_1, 16 - n);
  buffer_put(buffer_1, numbuf, n);
  buffer_putnlflush(buffer_1);
}
