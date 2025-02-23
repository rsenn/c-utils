#include "../tokenizer_internal.h"

int
tokenizer_next(tokenizer* t, token* out) {
  char* s = t->buf;
  int c = 0;

  out->value = 0;

  if(t->peeking) {
    *out = t->peek_token;
    t->peeking = 0;
    return 1;
  }

  for(;;) {
    c = tokenizer_getc(t);
    if(c == TOKENIZER_EOF)
      break;

    /* components of multi-line comment marker might be terminals
     * themselves */
    if(sequence_follows(t, c, t->marker[MT_MULTILINE_COMMENT_START])) {
      ignore_until(t, t->marker[MT_MULTILINE_COMMENT_END], str_len(t->marker[MT_MULTILINE_COMMENT_START]));
      continue;
    }

    if(sequence_follows(t, c, t->marker[MT_SINGLELINE_COMMENT_START])) {
      ignore_until(t, "\n", str_len(t->marker[MT_SINGLELINE_COMMENT_START]));
      continue;
    }

    if(is_sep(c)) {
      if(s != t->buf && c == '\\' && !isspace(s[-1])) {

        if((c = tokenizer_getc(t)) == '\n')
          continue;

        tokenizer_ungetc(t, c);
        c = '\\';
      } else if(is_plus_or_minus(c) && s > t->buf + 1 && (s[-1] == 'E' || s[-1] == 'e') && is_valid_float_until(t->buf, s - 1)) {
        goto process_char;
      } else if(c == '.' && s != t->buf && is_valid_float_until(t->buf, s) == 1) {
        goto process_char;
      } else if(c == '.' && s == t->buf) {
        int jump = 0;
        c = tokenizer_getc(t);

        if(isdigit(c))
          jump = 1;

        tokenizer_ungetc(t, c);
        c = '.';

        if(jump)
          goto process_char;
      }

      tokenizer_ungetc(t, c);
      break;
    }

    if((t->flags & TF_PARSE_WIDE_STRINGS) && s == t->buf && c == 'L') {
      c = tokenizer_getc(t);

      tokenizer_ungetc(t, c);
      tokenizer_ungetc(t, 'L');

      if(c == '\'' || c == '\"')
        break;
    }

  process_char:;
    s = assign_bufchar(t, s, c);

    if(t->column + 1 >= MAX_TOK_LEN) {
      out->type = TT_OVERFLOW;
      return apply_coords(t, out, s, 0);
    }
  }

  if(s == t->buf) {
    int wide = 0;

    if(c == TOKENIZER_EOF) {
      out->type = TT_EOF;

      return apply_coords(t, out, s, 1);
    }

    c = tokenizer_getc(t);

    if((t->flags & TF_PARSE_WIDE_STRINGS) && c == 'L') {
      c = tokenizer_getc(t);
      // assert(c == '\'' || c == '\"');
      wide = 1;
      goto string_handling;
    } else if(c == '.' && sequence_follows(t, c, "...")) {
      str_copy(t->buf, "...");
      out->type = TT_ELLIPSIS;

      return apply_coords(t, out, s + 3, 1);
    }

    for(int i = 0; i < t->custom_count; i++)
      if(sequence_follows(t, c, t->custom_tokens[i])) {
        const char* p = t->custom_tokens[i];
        while(*p) {
          s = assign_bufchar(t, s, *p);
          p++;
        }
        *s = 0;
        out->type = TT_CUSTOM + i;
        return apply_coords(t, out, s, 1);
      }

  string_handling:
    s = assign_bufchar(t, s, c);
    *s = 0;
    // s = assign_bufchar(t, s, 0);

    if(c == '"' || c == '\'')
      if(t->flags & TF_PARSE_STRINGS)
        return get_string(t, c, out, wide);

    out->type = TT_SEP;
    out->value = c;

    if(c == '\n') {
      apply_coords(t, out, s, 1);
      t->line++;
      t->column = 0;
      return 1;
    }

    return apply_coords(t, out, s, 1);
  }

  // s = assign_bufchar(t, s, 0);
  *s = 0;
  out->type = categorize(t->buf);

  return apply_coords(t, out, s, out->type != TT_UNKNOWN);
}
