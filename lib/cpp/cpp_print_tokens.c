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
      buffer_putnlflush(out);

    if(tok->origin) {
      filename = "";
    } else if(line <= 1 || tok->at_bol) {
      ++line_no;
      int same_file = filename == tok->filename || str_equal(filename, tok->filename);
      int diff = tok->line_no - line_no;

      if(!line_directives || (same_file && (diff > 0 && diff < 2))) {
        if(same_file && diff > 0) {
          if(!line_directives && diff > 1)
            diff = 1;

          buffer_putnc(out, '\n', diff);
          buffer_flush(out);
        }

      } else if(!same_file || line_no != tok->line_no) {
        /*   buffer_puts(out, "#line ");
           buffer_putulong(out, tok->line_no);
           buffer_putm_internal(out, " \"", tok->filename, "\"\n", 0);*/
        buffer_puts(out, "# ");
        buffer_putulong(out, tok->line_no);
        buffer_putm_internal(out, " \"", tok->filename, "\"\n", 0);
        buffer_flush(out);
      }

      line_no = tok->line_no;
      filename = tok->filename;
    }

    if(tok->has_space) {
      if(tok->at_bol) {
        cpp_token* tok2 = tok->origin ? tok->origin : tok;
        char *p = tok2->loc, *x = tok2->file->contents;

        while(p > x) {
          if(!isspace(*(p - 1)) || *(p - 1) == '\n')
            break;

          --p;
        }

        int sz = tok2->loc - p;

        if(sz > 0)
          buffer_put(out, p, sz);
      } else
        buffer_putspace(out);
    }

    buffer_put(out, tok->loc, tok->len);
    line++;
  }

  buffer_putnlflush(out);
}
