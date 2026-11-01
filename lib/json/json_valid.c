#include "../json.h"

#define JSON_MAX_DEPTH 256

static size_t
skip_ws(const char* x, size_t n, size_t i) {
  while(i < n && (x[i] == ' ' || x[i] == '\t' || x[i] == '\n' || x[i] == '\r'))
    i++;
  return i;
}

static int
is_digit(char c) {
  return c >= '0' && c <= '9';
}

static int
is_hex(char c) {
  return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

/* i is at the opening quote, returns the position after the closing one, 0 if damaged */
static size_t
check_string(const char* x, size_t n, size_t i) {
  for(i++; i < n; i++) {
    unsigned char c = (unsigned char)x[i];

    if(c == '"')
      return i + 1;

    if(c < 0x20)
      return 0;

    if(c == '\\') {
      if(++i >= n)
        return 0;

      switch(x[i]) {
        case '"': case '\\': case '/': case 'b': case 'f': case 'n': case 'r': case 't': break;
        case 'u':
          if(i + 4 >= n || !is_hex(x[i + 1]) || !is_hex(x[i + 2]) || !is_hex(x[i + 3]) || !is_hex(x[i + 4]))
            return 0;
          i += 4;
          break;
        default: return 0;
      }
    }
  }
  return 0;
}

static size_t
check_number(const char* x, size_t n, size_t i) {
  if(i < n && x[i] == '-')
    i++;

  if(i >= n || !is_digit(x[i]))
    return 0;

  if(x[i] == '0')
    i++;
  else
    while(i < n && is_digit(x[i]))
      i++;

  if(i < n && x[i] == '.') {
    if(++i >= n || !is_digit(x[i]))
      return 0;
    while(i < n && is_digit(x[i]))
      i++;
  }

  if(i < n && (x[i] == 'e' || x[i] == 'E')) {
    i++;
    if(i < n && (x[i] == '+' || x[i] == '-'))
      i++;
    if(i >= n || !is_digit(x[i]))
      return 0;
    while(i < n && is_digit(x[i]))
      i++;
  }
  return i;
}

static size_t
check_word(const char* x, size_t n, size_t i, const char* word, size_t len) {
  size_t k;

  if(n - i < len)
    return 0;
  for(k = 0; k < len; k++)
    if(x[i + k] != word[k])
      return 0;
  return i + len;
}

static size_t
check_value(const char* x, size_t n, size_t i, int depth) {
  i = skip_ws(x, n, i);

  if(i >= n || depth > JSON_MAX_DEPTH)
    return 0;

  switch(x[i]) {
    case '"': return check_string(x, n, i);
    case 't': return check_word(x, n, i, "true", 4);
    case 'f': return check_word(x, n, i, "false", 5);
    case 'n': return check_word(x, n, i, "null", 4);
    case '[': {
      i = skip_ws(x, n, i + 1);

      if(i < n && x[i] == ']')
        return i + 1;

      for(;;) {
        if(!(i = check_value(x, n, i, depth + 1)))
          return 0;
        i = skip_ws(x, n, i);
        if(i >= n)
          return 0;
        if(x[i] == ']')
          return i + 1;
        if(x[i] != ',')
          return 0;
        i++;
      }
    }
    case '{': {
      i = skip_ws(x, n, i + 1);

      if(i < n && x[i] == '}')
        return i + 1;

      for(;;) {
        i = skip_ws(x, n, i);
        if(i >= n || x[i] != '"' || !(i = check_string(x, n, i)))
          return 0;
        i = skip_ws(x, n, i);
        if(i >= n || x[i] != ':')
          return 0;
        if(!(i = check_value(x, n, i + 1, depth + 1)))
          return 0;
        i = skip_ws(x, n, i);
        if(i >= n)
          return 0;
        if(x[i] == '}')
          return i + 1;
        if(x[i] != ',')
          return 0;
        i++;
      }
    }
    default: return check_number(x, n, i);
  }
}

/* Tells if the n bytes at x are exactly one JSON value (RFC 8259, white
 * space around it is allowed). The reader of the module accepts a lot more
 * and does not tell when it gave up. */
int
json_valid(const char* x, size_t n) {
  size_t i = check_value(x, n, 0, 0);

  return i && skip_ws(x, n, i) == n;
}
