#include "../cpp.h"
#include "../cpp_internal.h"
#include "../buffer.h"

void
cpp_print_macro(buffer* out, cpp_macro* m) {
  buffer_puts(out, "#define ");
  buffer_puts(out, m->name);

  if(m->params) {
    cpp_macro_param* par;
    buffer_putc(out, '(');

    for(par = m->params; par; par = par->next) {
      if(par != m->params)
        buffer_puts(out, ", ");
      buffer_puts(out, par->name);
    }

    buffer_putc(out, ')');
    /*buffer_puts(out, " \\\n");*/
  }

  cpp_print_tokens(out, m->body, 0);
}
