#include "../cpp_internal.h"
#include "../memstream.h"

/* rec_level -1 serves as a magic value to signal we're using
   cpp_macro_expand from the if-evaluator code, which means activating
   the "define" macro */
int
cpp_macro_expand(cpp_t* cpp, tokenizer* t, buffer* out, const char* name, unsigned rec_level, char* visited[]) {
  int is_define = !str_diff(name, "defined");
  size_t i;
  cpp_macro* m;

  if(is_define && rec_level != -1)
    m = NULL;
  else
    m = cpp_macro_get(cpp, name);

  if(!m) {
    buffer_puts(out, name);
    return 1;
  }

  if(rec_level == -1)
    rec_level = 0;

  if(rec_level >= MAX_RECURSION) {
    error("max recursion level reached", t, 0);
    return 0;
  }

#ifdef DEBUG_CPP
  buffer_puts(buffer_2, "lvl ");
  buffer_putulong(buffer_2, rec_level);
  buffer_putm_internal(buffer_2, ": expanding macro ", name, m->str_contents_buf ? " (" : 0, m->str_contents_buf, ")", NULL);
  buffer_putnlflush(buffer_2);
#endif

  if(rec_level == 0 && str_diff(t->filename, "<macro>")) {
    cpp->last_file = t->filename;
    cpp->last_line = t->line;
  }

  if(!str_diff(name, "__FILE__")) {
    buffer_puts(out, "\"");
    buffer_puts(out, cpp->last_file);
    buffer_puts(out, "\"");
    return 1;
  } else if(!str_diff(name, "__LINE__")) {
    buffer_putlong(out, cpp->last_line);
    return 1;
  }

  if(visited[rec_level])
    alloc_free(visited[rec_level]);

  visited[rec_level] = str_dup(name);
  cpp->tchain[rec_level] = t;

  {
    token tok;
    unsigned num_args = MACRO_ARGCOUNT(m);
    cpp_file* argvalues = alloc_zero((MACRO_VARIADIC(m) ? num_args + 1 : num_args) * sizeof(cpp_file));

    for(i = 0; i < num_args; i++)
      argvalues[i].f = memstream_open(&argvalues[i].buf, &argvalues[i].len);

    /* replace named arguments in the contents of the macro call */
    if(FUNCTIONLIKE(m)) {
      int ret;

      if((ret = tokenizer_peek(t)) != '(') {
        /* function-like macro shall not be expanded if not followed by '(' */
        if(ret == TOKENIZER_EOF && rec_level > 0 && (ret = cpp_tchain_parens_follows(cpp, rec_level - 1)) != -1) {
          // warning("Replacement text involved subsequent text", t, 0);
          t = cpp->tchain[ret];
        } else {
          buffer_puts(out, name);
          goto cleanup;
        }
      }

      ret = x_tokenizer_next(t, &tok);
      assert(ret && is_char(&tok, '('));

      {
        unsigned curr_arg = 0, need_arg = 1, parens = 0;
        int ws_count, varargs = 0;

        if(!tokenizer_skip_chars(t, " \t", &ws_count))
          return 0;

        if(num_args == 1 && MACRO_VARIADIC(m))
          varargs = 1;

        while(1) {
          int ret = tokenizer_next(t, &tok);

          if(!ret)
            return 0;

          if(tok.type == TT_EOF) {
            buffer_putsflush(buffer_2, "warning TOKENIZER_EOF\n");
            break;
          }

          if(!parens && is_char(&tok, ',') && !varargs) {
            if(need_arg && !ws_count) {
              /* empty argument is OK */
            }

            need_arg = 1;

            if(!varargs)
              curr_arg++;

            if(curr_arg + 1 == num_args && MACRO_VARIADIC(m)) {
              varargs = 1;
            } else if(curr_arg >= num_args) {
              error("too many arguments for function macro", t, &tok);
              return 0;
            }

            if(!(ret = tokenizer_skip_chars(t, " \t", &ws_count)))
              return ret;

            continue;
          } else if(is_char(&tok, '(')) {
            ++parens;
          } else if(is_char(&tok, ')')) {
            if(!parens) {
              if(curr_arg + num_args && curr_arg < num_args - 1) {
                error("too few args for function macro", t, &tok);
                return 0;
              }

              break;
            }

            --parens;
          } else if(is_char(&tok, '\\')) {
            if(tokenizer_peek(t) == '\n')
              continue;
          }

          need_arg = 0;
          emit_token(argvalues[curr_arg].f, &tok, t->buf);
        }
      }
    }

    for(i = 0; i < num_args; i++) {
      argvalues[i].f = buffer_reopen(argvalues[i].f, &argvalues[i].buf, &argvalues[i].len);

      tokenizer_from_file(&argvalues[i].t, argvalues[i].f);

#ifdef DEBUG_CPP
      buffer_putm_internal(buffer_2, "macro argument ", 0);
      buffer_putlong(buffer_2, (long)i);
      buffer_puts(buffer_2, ": ");
      buffer_put(buffer_2, argvalues[i].buf, argvalues[i].len);
      buffer_putnlflush(buffer_2);
#endif
    }

    if(is_define) {
      if(MAP_GET(cpp->macros, argvalues[0].buf, argvalues[0].len + 1))
        buffer_puts(out, "1");
      else
        buffer_puts(out, "0");
    }

    if(!m->str_contents_buf)
      goto cleanup;

    {
      cpp_macro_info* mcs;
      buffer* output;
      cpp_file cwae = {0}; /* contents_with_args_expanded */
      int hash_count = 0, ws_count = 0;
      tokenizer t2;
      cwae.f = memstream_open(&cwae.buf, &cwae.len);
      output = cwae.f;

      tokenizer_from_file(&t2, &m->str_contents);

      while(1) {
        int ret;

        if(!(ret = tokenizer_next(&t2, &tok)))
          return 0;

        if(tok.type == TT_EOF)
          break;

        if(tok.type == TT_IDENTIFIER) {
          size_t arg_nr;
          char* id;
          flush_whitespace(output, &ws_count);
          id = t2.buf;

          if(MACRO_VARIADIC(m) && !str_diff(t2.buf, "__VA_ARGS__"))
            id = "...";

          arg_nr = macro_arglist_pos(m, id);

          if(arg_nr != (size_t)-1) {
            tokenizer_rewind(&argvalues[arg_nr].t);

            if(hash_count == 1)
              ret = cpp_stringify(cpp, &argvalues[arg_nr].t, output);
            else
              while(1) {
                ret = tokenizer_next(&argvalues[arg_nr].t, &tok);

                if(!ret)
                  return ret;

                if(tok.type == TT_EOF)
                  break;

                emit_token(output, &tok, argvalues[arg_nr].t.buf);
              }

            hash_count = 0;
          } else {
            if(hash_count == 1) {
            hash_err:
              error("'#' is not followed by macro parameter", &t2, &tok);
              return 0;
            }

            emit_token(output, &tok, t2.buf);
          }

        } else if(is_char(&tok, '#')) {
          if(hash_count)
            goto hash_err;

          while(1) {
            ++hash_count;

            /* in a real cpp we'd need to look for '\\' first */
            while(tokenizer_peek(&t2) == '\n') {
              x_tokenizer_next(&t2, &tok);
            }

            if(tokenizer_peek(&t2) == '#')
              x_tokenizer_next(&t2, &tok);
            else
              break;
          }
          if(hash_count == 1)
            flush_whitespace(output, &ws_count);
          else if(hash_count > 2) {
            error("only two '#' characters allowed for macro expansion", &t2, &tok);
            return 0;
          }

          if(hash_count == 2)
            ret = tokenizer_skip_chars(&t2, " \t\n", &ws_count);
          else
            ret = tokenizer_skip_chars(&t2, " \t", &ws_count);

          if(!ret)
            return ret;
          ws_count = 0;

        } else if(is_whitespace_token(&tok)) {
          ws_count++;
        } else {
          if(hash_count == 1)
            goto hash_err;

          flush_whitespace(output, &ws_count);
          emit_token(output, &tok, t2.buf);
        }
      }

      flush_whitespace(output, &ws_count);

      /* we need to expand macros after the macro arguments have been inserted */
      if(1) {
        size_t mac_cnt = 0;
        cwae.f = buffer_reopen(cwae.f, &cwae.buf, &cwae.len);
#ifdef DEBUG_CPP
        buffer_putm_internal(buffer_2, "contents with args expanded: ", cwae.buf, NULL);
        buffer_putnlflush(buffer_2);
#endif
        tokenizer_from_file(&cwae.t, cwae.f);
        while(1) {
          int ret = tokenizer_next(&cwae.t, &tok);
          if(!ret)
            return ret;
          if(tok.type == TT_EOF)
            break;
          if(tok.type == TT_IDENTIFIER && cpp_macro_get(cpp, cwae.t.buf))
            ++mac_cnt;
        }

        tokenizer_rewind(&cwae.t);
        mcs = alloc_zero(mac_cnt * sizeof(cpp_macro_info));

        {
          size_t mac_iter = 0;
          cpp_macro_get_info(cpp, &cwae.t, mcs, &mac_iter, 0, 0, "null", visited, rec_level);
          /* some of the macros might not expand at this stage (without braces)*/
          while(mac_cnt && mcs[mac_cnt - 1].name == 0)
            --mac_cnt;
        }

        {
          size_t i;
          int depth = 0;

          for(i = 0; i < mac_cnt; ++i)
            if(mcs[i].nest > depth)
              depth = mcs[i].nest;

          while(depth > -1) {
            for(i = 0; i < mac_cnt; ++i)
              if(mcs[i].nest == depth) {
                cpp_macro_info* mi = &mcs[i];
                size_t j;
                int diff;
                off_t cwae_pos;
                token utok;
                cpp_file t2 = {0}, tmp = {0};

                tokenizer_rewind(&cwae.t);

                for(j = 0; j < mi->first + 1; ++j)
                  tokenizer_next(&cwae.t, &utok);

                t2.f = memstream_open(&t2.buf, &t2.len);

                if(!cpp_macro_expand(cpp, &cwae.t, t2.f, mi->name, rec_level + 1, visited))
                  return 0;

                t2.f = buffer_reopen(t2.f, &t2.buf, &t2.len);
                tokenizer_from_file(&t2.t, t2.f);

                /* manipulating the stream in case more stuff has been consumed */
                cwae_pos = tokenizer_ftello(&cwae.t);
                tokenizer_rewind(&cwae.t);
#ifdef DEBUG_CPP
                buffer_putm_internal(buffer_2, "merging ", cwae.buf, " with ", t2.buf, NULL);
                buffer_putnlflush(buffer_2);
#endif
                diff = mem_tokenizers_join(&cwae, &t2, &tmp, mi->first, cwae_pos);
                free_file_container(&cwae);
                free_file_container(&t2);
                cwae = tmp;
#ifdef DEBUG_CPP
                buffer_putm_internal(buffer_2, "result: ", cwae.buf, NULL);
                buffer_putnlflush(buffer_2);
#endif
                if(diff == 0)
                  continue;

                for(j = 0; j < mac_cnt; ++j) {
                  cpp_macro_info* mi2 = &mcs[j];
                  if(j == i)
                    continue;
                  /* modified element mi can be either inside, after or before
                     another macro. the after case doesn't affect us. */
                  if(mi->first >= mi2->first && mi->last <= mi2->last) {
                    /* inside m2 */
                    mi2->last += diff;
                  } else if(mi->first < mi2->first) {
                    /* before m2 */
                    mi2->first += diff;
                    mi2->last += diff;
                  }
                }
              }

            --depth;
          }
        }

        tokenizer_rewind(&cwae.t);

        while(1) {
          cpp_macro* ma;

          tokenizer_next(&cwae.t, &tok);

          if(tok.type == TT_EOF)
            break;

          if(tok.type == TT_IDENTIFIER && tokenizer_peek(&cwae.t) == TOKENIZER_EOF && (ma = cpp_macro_get(cpp, cwae.t.buf)) && FUNCTIONLIKE(ma) && cpp_tchain_parens_follows(cpp, rec_level) != -1) {
            int ret = cpp_macro_expand(cpp, &cwae.t, out, cwae.t.buf, rec_level + 1, visited);

            if(!ret)
              return ret;

          } else
            emit_token(out, &tok, cwae.t.buf);
        }

        alloc_free(mcs);
      }

      free_file_container(&cwae);
    }
  cleanup:
    for(i = 0; i < num_args; i++) {
      memstream_free(argvalues[i].f);
      // alloc_free(argvalues[i].buf);
    }

    alloc_free(argvalues);
  }
  return 1;
}
