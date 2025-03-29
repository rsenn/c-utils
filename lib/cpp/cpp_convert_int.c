#include "../cpp_internal.h"

bool
cpp_convert_int(cpp_token* tok) {
  char* p = tok->loc;

  /* Read a binary, octal, decimal or hexadecimal number.*/
  int base = 10;

  if(!case_diffb(p, 2, "0x") && isxdigit(p[2])) {
    p += 2;
    base = 16;
  } else if(!case_diffb(p, 2, "0b") && (p[2] == '0' || p[2] == '1')) {
    p += 2;
    base = 2;
  } else if(*p == '0') {
    base = 8;
  }

  int64_t val = strtoul(p, &p, base);

  /* Read U, L or LL suffixes.*/
  bool l = false, u = false;

  if(str_start(p, "LLU") || str_start(p, "LLu") || str_start(p, "llU") || str_start(p, "llu") || str_start(p, "ULL") || str_start(p, "Ull") || str_start(p, "uLL") ||
     str_start(p, "ull")) {
    p += 3;
    l = u = true;
  } else if(!case_diffb(p, 2, "lu") || !case_diffb(p, 2, "ul")) {
    p += 2;
    l = u = true;
  } else if(str_start(p, "LL") || str_start(p, "ll")) {
    p += 2;
    l = true;
  } else if(*p == 'L' || *p == 'l') {
    p++;
    l = true;
  } else if(*p == 'U' || *p == 'u') {
    p++;
    u = true;
  }

  if(p != tok->loc + tok->len)
    return false;

  /* Infer a type.*/
  cpp_type* ty;

  if(base == 10) {
    if(l && u)
      ty = cpp_ty_ulong;
    else if(l)
      ty = cpp_ty_long;
    else if(u)
      ty = (val >> 32) ? cpp_ty_ulong : cpp_ty_uint;
    else
      ty = (val >> 31) ? cpp_ty_long : cpp_ty_int;
  } else {
    if(l && u)
      ty = cpp_ty_ulong;
    else if(l)
      ty = (val >> 63) ? cpp_ty_ulong : cpp_ty_long;
    else if(u)
      ty = (val >> 32) ? cpp_ty_ulong : cpp_ty_uint;
    else if(val >> 63)
      ty = cpp_ty_ulong;
    else if(val >> 32)
      ty = cpp_ty_long;
    else if(val >> 31)
      ty = cpp_ty_uint;
    else
      ty = cpp_ty_int;
  }

  tok->kind = TK_NUM;
  tok->val = val;
  tok->ty = ty;
  return true;
}
