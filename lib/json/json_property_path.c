#include "../json_internal.h"
#include "../scan.h"

/* Follows a path such as "candidates[0].content.parts[0].text" through
 * objects and arrays. Returns the value, or one of type JSON_UNDEFINED if
 * any step is missing. A key can be quoted in brackets, ["a.b"], when it
 * has dots or brackets in it. */
jsonval
json_property_path(jsonval val, const char* path) {
  while(*path && val.type != JSON_UNDEFINED) {
    if(*path == '.') {
      path++;
    } else if(*path == '[') {
      path++;

      if(*path == '"') {
        stralloc key;
        size_t n = str_chr(path + 1, '"');

        stralloc_init(&key);
        stralloc_catb(&key, path + 1, n);
        stralloc_nul(&key);
        val = json_property_get_str(val, key.s);
        stralloc_free(&key);
        path += n + 2;
      } else {
        unsigned long i = 0;
        size_t n = scan_ulong(path, &i);

        path += n;
        val = val.type == JSON_ARRAY ? json_property_get_int(val, (int64)i) : json_undefined();
      }

      if(*path == ']')
        path++;
    } else {
      stralloc key;
      size_t n = str_chrs(path, ".[", 2);

      stralloc_init(&key);
      stralloc_catb(&key, path, n);
      stralloc_nul(&key);
      val = json_property_get_str(val, key.s);
      stralloc_free(&key);
      path += n;
    }
  }
  return val;
}
