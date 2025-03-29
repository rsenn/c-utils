#include "../cpp_internal.h"
#include "../buffer.h"

/* Print tokens to stdout. Used for -E. */
void
cpp_print_tokens(buffer* out, cpp_token* tok, bool line_directives) {
  int line = 1;
  const char* filename = "";
  int line_no = 0;

  for(; tok->kind != TK_EOF; tok = tok->next) {
    if(line > 1 && tok->at_bol)
      buffer_putc(out, '\n');

    if(line <= 1 || tok->at_bol) {
      ++line_no;

      if(!str_equal(filename, tok->filename) || line_no != tok->line_no) {
        buffer_puts(out, "#line ");
        buffer_putulong(out, tok->line_no);
        buffer_putm_internal(out, " \"", tok->filename, "\"\n", 0);

        line_no = tok->line_no;
        filename = tok->filename;
      }
    }

    if(tok->has_space && !tok->at_bol) {
      /*char *p = tok->loc, *x = tok->file->contents;

      while(p > x) {
        if(!isspace(*(p - 1)))
          break;

        --p;
      }

      if(p < tok->loc)
        buffer_put(out, p, tok->loc - p);*/

      buffer_putspace(out);
    }

    buffer_put(out, tok->loc, tok->len);
    line++;
  }

  buffer_putnlflush(out);
}
