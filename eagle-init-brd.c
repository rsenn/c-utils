#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#if !defined(_WIN32) && !(defined(__MSYS__) && __MSYS__ == 1)
#include <libgen.h>
#endif

#include "lib/hmap.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/xml.h"

#define END_OF_LINE "; "
//#define END_OF_LINE ";\n"
void after_element(const char*);
void on_attribute_decl(void*, const char*, const char*, int, int, const char*);
void on_characters(void*, const char*, int);
void on_end_element(void*, const char*);
void on_start_element_ns(void*, const char*, const char*, const char*, int, const char**, int, int, const char**);
void on_start_element(void*, const char*, HMAP_DB**);
int xml_callback(xmlreader* r, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs);
int xml_read_node(xmlreader* r, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs);
stralloc element_name, character_buf;
double const unit_factor = 25.4, scale_factor = 0.666666, grid_mils = 100;
double min_x = 0.0, max_x = 0.0, min_y = 0.0, max_y = 0.0;
void
update_minmax_xy(double x, double y) {
  if(x < min_x) min_x = x;
  if(y < min_y) min_y = y;
  if(x > max_x) max_x = x;
  if(y > max_y) max_y = y;
};
xmlnode* xmldoc = NULL;
HMAP_DB* hashmap = NULL;
TUPLE* ptr_tuple = NULL;
HMAP_DB *instances_db = NULL, *parts_db = NULL;
void hmap_foreach(HMAP_DB* hmap, void (*foreach_fn)(void*));
void update_part(const char*, double, double, double);
#define NAMELEN 8
typedef struct part {
  char name[NAMELEN];
  char library[NAMELEN];
  char deviceset[NAMELEN];
  char device[NAMELEN];
  char value[NAMELEN];
  double x, y, rot;
} part_t;
typedef struct instance {
  char part[NAMELEN];
  char gate[NAMELEN];
  double x, y;
  double rot;
} instance_t;


/* ----------------------------------------------------------------------- */
void
dump_part(part_t const* p) {
  buffer_putm(buffer_2, "dump_part{name=", p->name, ",library=", p->library, ",deviceset", p->deviceset, ",device=", p->device, ",value=", p->value);

  buffer_puts(buffer_2, ",x=");
  buffer_putdouble(buffer_2, p->x);
  buffer_puts(buffer_2, ",y=");
  buffer_putdouble(buffer_2, p->y);
  buffer_puts(buffer_2, ",rot=");
  buffer_putdouble(buffer_2, p->rot);
  buffer_putnlflush(buffer_2);
}

/* ----------------------------------------------------------------------- */
void
dump_instance(instance_t const* i) {
  buffer_putm(buffer_2, "dump_instance \"", i->part, ":", i->gate, "\"");
  buffer_puts(buffer_2, " x=");
  buffer_putdouble(buffer_2, i->x);
  buffer_puts(buffer_2, ", y=");
  buffer_putdouble(buffer_2, i->y);
  buffer_puts(buffer_2, ", rot=");
  buffer_putdouble(buffer_2, i->rot);
  buffer_putnlflush(buffer_2);
}

/* ----------------------------------------------------------------------- */
inline static double
round_to_mil(double val, double mil) {
  double factor = (1000.0f / mil);
  return round(val * factor) / factor;
}

/* ----------------------------------------------------------------------- */
size_t
str_copyn(char* out, const char* in, size_t n) {
  strncpy(out, in, n);
  out[n] = '\0';
  return str_len(out);
}

/* ----------------------------------------------------------------------- */
void
each_part(part_t* p) {
  if(p->device[0] != '\0' || p->value[0] != '\0') {

    buffer_putm(buffer_1, "MOVE ", p->name, " (");
    buffer_putdouble(buffer_1, p->x - min_x);
    buffer_putc(buffer_1, ' ');
    buffer_putdouble(buffer_1, p->y - min_y);
    buffer_putc(buffer_1, ')');
    buffer_putnlflush(buffer_1);


    if(fabs(p->rot) >= 0.1) {
      int angle = (int)((p->rot / 90)) * 90.0;
      while(angle < 0) angle += 360;
      while(angle > 360) angle -= 360;

      buffer_puts(buffer_1, "ROTATE =R");
      buffer_putlong(buffer_1, angle % 360);
      buffer_putm(buffer_1, " '", p->name, "'");
      buffer_putnlflush(buffer_1);
    }
  }

#ifdef DEBUG
  dump_part(p);
#endif
}


/* ----------------------------------------------------------------------- */
inline static part_t*
get_part(const char* part) {
  TUPLE* ptr_tuple = NULL;
  part_t* p = NULL;
  hmap_search(parts_db, (char*)part, str_len(part), &ptr_tuple);
  if(ptr_tuple) p = ptr_tuple->vals.val_custom;
  return p;
}

/* ----------------------------------------------------------------------- */
inline static const instance_t*
get_instance(const char* part, const char* gate) {
  stralloc key;
  stralloc_init(&key);
  stralloc_copys(&key, part);
  stralloc_cats(&key, ":");
  stralloc_cats(&key, gate);
  stralloc_nul(&key);
  TUPLE* ptr_tuple = NULL;
  instance_t* i = NULL;
  hmap_search(instances_db, key.s, key.len, &ptr_tuple);
  if(ptr_tuple) i = ptr_tuple->vals.val_custom;
  return i;
}

/* ----------------------------------------------------------------------- */
instance_t*
create_instance(const char* part, const char* gate, double x, double y, double rot) {
  int ret;
  stralloc key;
  instance_t* i;
  stralloc_init(&key);
  stralloc_copys(&key, part);
  stralloc_cats(&key, ":");
  stralloc_cats(&key, gate);
  i = malloc(sizeof(instance_t));
  if(i == NULL) return NULL;
  str_copyn(i->part, part, sizeof(i->part) - 1);
  str_copyn(i->gate, gate, sizeof(i->gate) - 1);
  i->x = x;
  i->y = y;
  i->rot = rot;
  hmap_add(&instances_db, key.s, key.len, 1, HMAP_DATA_TYPE_CUSTOM, i);
  update_part(part, x, y, rot);
#ifdef DEBUG
  dump_instance(i);
#endif
  return i;
}

/* ----------------------------------------------------------------------- */
part_t*
create_part(const char* name, const char* library, const char* deviceset, const char* device, const char* value) {
  if(value == NULL) value = "";
    /*if(deviceset == NULL) deviceset = "";
    if(device == NULL) device = "";*/
  part_t* p;
  p = malloc(sizeof(part_t));
  if(p == NULL) return NULL;
  str_copyn(p->name, name, sizeof(p->name) - 1);
  str_copyn(p->library, library ? library : "", sizeof(p->library) - 1);
  str_copyn(p->deviceset, deviceset ? deviceset : "", sizeof(p->deviceset) - 1);
  str_copyn(p->device, device ? device : "", sizeof(p->device) - 1);
  str_copyn(p->value, value ? value : "", sizeof(p->value) - 1);
  p->x = 0.0;
  p->y = 0.0;
  p->rot = 0.0;
  hmap_add(&parts_db, (char*)name, str_len(name), 1, HMAP_DATA_TYPE_CUSTOM, p);
#ifdef DEBUG
  dump_part(p);
#endif
  return p;
}

/* ----------------------------------------------------------------------- */
void
update_part(const char* name, double x, double y, double rot) {
  part_t* p = get_part(name);
  if(p == NULL) return;

  if(p->x == 0.0 || isnan(p->x)) {
    p->x = x;
  } else {
    p->x += x;
    p->x /= 2;
    p->x = round(p->x * 100) / 100;
  }
  if(p->y == 0.0 || isnan(p->y)) {
    p->y = y;
  } else {
    p->y += y;
    p->y /= 2;
    p->y = round(p->y * 100) / 100;
  }
  if(p->rot == 0.0 || isnan(p->rot)) {
    p->rot = rot;
  } else {
    p->rot += rot; /* p->rot /= 2; */
    p->rot = round(p->rot);
  }
  update_minmax_xy(p->x, p->y);
#ifdef DEBUG
  dump_part(p);
#endif
}

/* ----------------------------------------------------------------------- */
void
attr_list(stralloc* sa, HMAP_DB* hmap) {
  TUPLE* p;
  if(hmap == NULL) return;
  for(p = hmap->list_tuple; p; p = p->next) {
    if(p->data_type == HMAP_DATA_TYPE_CHARS) {
      stralloc_catb(sa, " ", 1);
      stralloc_cats(sa, p->key);
      stralloc_cats(sa, "=\"");
      stralloc_cats(sa, p->vals.val_chars);
      stralloc_cats(sa, "\"");
    }
    if(p->next == hmap->list_tuple) break;
  }
}

/* ----------------------------------------------------------------------- */
void
hmap_foreach(HMAP_DB* hmap, void (*foreach_fn)(void*)) {
  TUPLE* t;
  if(hmap == NULL) return;
  for(t = hmap->list_tuple; t; t = t->next) {
    if(t->data_type == HMAP_DATA_TYPE_CUSTOM) foreach_fn(t->vals.val_custom);
    if(t->next == hmap->list_tuple) break;
  }
}

/* ----------------------------------------------------------------------- */
//void
//print_list(HMAP_DB* hmap) {
//  TUPLE* p;
//  if(hmap == NULL) return;
//  for(p = hmap->list_tuple; p; p = p->next) {
//    if(p->data_type == HMAP_DATA_TYPE_CHARS) {
//      /* printf("index[%d][%p] key[%s], data[%s]\n", p->index, p,  p->key,
//       * p->vals.val_chars); */
//      printf("key=\"%s\",data=\"%s\"\n", p->key, p->vals.val_chars);
//    } else if(p->data_type == HMAP_DATA_TYPE_CUSTOM) {
//      printf("key=\"%s\",data=%p\n", p->key, p->vals.val_custom);
//    }
//    if(p->next == hmap->list_tuple) break;
//  }
//}

/* ----------------------------------------------------------------------- */
int
get_attribute_sa(stralloc* sa, xmlnode* n, const char* name) {
  TUPLE* t;
  if(hmap_search(n->attributes, (char*)name, str_len(name), &t) == HMAP_SUCCESS) {
    stralloc_copyb(sa, t->vals.val_chars, t->data_len);
    if(sa->len > 0 && sa->s[sa->len - 1] == '\0') --sa->len;
    return 1;
  }
  return 0;
}

/* ----------------------------------------------------------------------- */
int
get_attribute_double(double* d, xmlnode* e, const char* name) {
  stralloc sa;
  stralloc_init(&sa);
  if(!get_attribute_sa(&sa, e, name)) return 0;
  stralloc_nul(&sa);
  if(scan_double(sa.s, d) == sa.len) return 1;
  return 0;
}

/* ----------------------------------------------------------------------- */
void
cat_attributes(stralloc* sa, xmlnode* e) {
  TUPLE* a;
  for(a = xml_attributes(e); a; a = hmap_next(e->attributes, a)) {
    const char* value = (const char*)a->vals.val_chars;
    stralloc_cats(sa, "\n  ");
    stralloc_catb(sa, a->key, a->key_len);
    stralloc_cats(sa, "=\"");
    if(value) stralloc_cats(sa, value);
    stralloc_catb(sa, "\"", 1);
    if(a->next == xml_attributes(e)) break;
  }
}

/* ----------------------------------------------------------------------- */
void
process_instance(xmlnode* e) {
  double x = 0.0, y = 0.0, rotate = 0.0;
  stralloc part, gate, rot;
  stralloc_init(&part);
  get_attribute_sa(&part, e, "part");
  stralloc_init(&gate);
  get_attribute_sa(&gate, e, "gate");
  stralloc_init(&rot);
  get_attribute_sa(&rot, e, "rot");
  if(rot.len > 0) {
    const char* r = rot.s;
    while(*r && !isdigit(*r)) ++r;
    scan_double(r, &rotate);
  }
  get_attribute_double(&x, e, "x");
  get_attribute_double(&y, e, "y");
  /*x /= unit_factor;
  y /= unit_factor;*/
  /*x *= scale_factor;
  y *= scale_factor;*/
  instance_t* newinst = create_instance(part.s,
                                        gate.s,
                                        round_to_mil(x * scale_factor / unit_factor, grid_mils),
                                        round_to_mil(y * scale_factor / unit_factor, grid_mils),
                                        rotate);
}

/* ----------------------------------------------------------------------- */
void
process_part(xmlnode* e) {
  stralloc name, library, deviceset, device, value;
  stralloc_init(&name);
  get_attribute_sa(&name, e, "name");
  stralloc_init(&library);
  get_attribute_sa(&library, e, "library");
  stralloc_init(&deviceset);
  get_attribute_sa(&deviceset, e, "deviceset");
  stralloc_init(&device);
  get_attribute_sa(&device, e, "device");
  stralloc_init(&value);
  get_attribute_sa(&value, e, "value");
  part_t* newpart = create_part(name.s, library.s, deviceset.s, device.s, value.s);
}

/* ----------------------------------------------------------------------- */
void
print_element_names(xmlnode* a_node) {
  xmlnode* n = NULL;
  if(a_node->type == XML_DOCUMENT) a_node = a_node->children;
  for(n = a_node; n; n = n->next) {
    if(n->type == XML_ELEMENT) {
      stralloc attrs;
      xmlnode* e = (xmlnode*)n;
      const char* value = xml_content(n);
      stralloc_init(&attrs);
      cat_attributes(&attrs, e);
      stralloc_nul(&attrs);
      if(!str_diff((const char*)e->name, "instance")) {
        process_instance(e);
      } else if(!str_diff((const char*)e->name, "part")) {
        process_part(e);
      } else {
      }
      stralloc_free(&attrs);
      if(n->children) print_element_names(n->children);
    }
  }
}

/* ----------------------------------------------------------------------- */
void
set_element_name(const char* name) {
  stralloc_copys(&element_name, name);
}

/* ----------------------------------------------------------------------- */
const char*
get_element_name() {
  stralloc_nul(&element_name);
  return element_name.s ? element_name.s : "(null)";
}

/* ----------------------------------------------------------------------- */
int read_xmlfile(const char* filename);
int parse_xmlfile(const char* filename, xmlnode** p_doc);


/* ----------------------------------------------------------------------- */
int
parse_xmlfile(const char* filename, xmlnode** p_doc) {
  buffer input;
  buffer_mmapprivate(&input, filename);
  *p_doc = xml_read_tree(&input);
  return !!*p_doc;
}

///* ----------------------------------------------------------------------- */
int
xml_read_node(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  stralloc_nul(name);
  if(reader->closing && id == XML_ELEMENT) {
    on_end_element(reader, name->s);
  } else if(id == XML_TEXT) {
    on_characters(reader, name->s, name->len);
  } else if(id == XML_ELEMENT) {
    on_start_element(reader, name->s, attrs);
  }
  return 1;
}

/* ----------------------------------------------------------------------- */
void
on_start_element(void* ctx, const char* name, HMAP_DB** attrs) {
  int i, numAttrs = 0;
  set_element_name((const char*)name);
  if(attrs) {
    numAttrs = hmap_size(*attrs);
  }
  buffer_putm(buffer_1, "<", name, "> ");
  buffer_putlong(buffer_1, numAttrs);
  buffer_putnlflush(buffer_1);

  TUPLE* t = (*attrs)->list_tuple;
  for(i = 0; i < numAttrs; ++i) {
    char *attr = t->key, *value = t->vals.val_chars;

    buffer_putm(buffer_1, "<", name, "> ");
    buffer_putlong(buffer_1, i);
    buffer_putc(buffer_1, '/');
    buffer_putlong(buffer_1, numAttrs);
    buffer_putm(buffer_1, ": ", attr, "=\"", value, "\"\n");

    hmap_add(&hashmap, attr, str_len(attr), 1, HMAP_DATA_TYPE_CHARS, value, str_len(value));
  }
}

/* ----------------------------------------------------------------------- */
void
on_end_element(void* ctx, const char* name) {
  after_element((const char*)name);
}

/* ----------------------------------------------------------------------- */
// static void
// on_end_element_ns(void* ctx,
//}
/* ----------------------------------------------------------------------- */
void
after_element(const char* name) {
  stralloc saa;
  stralloc_init(&saa);
  attr_list(&saa, hashmap);
  stralloc_nul(&saa);
//  if(saa.len) printf("<%s> attrs:%s\n", get_element_name(), saa.s);
  stralloc_free(&saa);
  hmap_destroy(&hashmap);
  hmap_init(1024, &hashmap);
  stralloc_zero(&character_buf);
}

/* ----------------------------------------------------------------------- */
size_t
str_escapen(char* out, const char* in, size_t n) {
  size_t i;
  for(i = 0; i < n; ++i, ++out) {
    if(in[i] == '\n') {
      *out = '\\';
      *++out = 'n';
    } else {
      *out = in[i];
    }
  }
  *out = '\0';
  return i;
}

/* ----------------------------------------------------------------------- */
void
on_characters(void* ctx, const char* ch, int len) {
  char* chars = str_ndup(ch, len);
  char* escaped = malloc(len * 2 + 1);
  int i;
  str_copyn(chars, (const char*)ch, len);
  str_escapen(escaped, chars, len);
  for(i = len - 1; i >= 0; --i) {
    if(!isspace(escaped[i])) break;
    escaped[i] = '\0';
  }
  if(str_len(escaped) > 0) {
    stralloc_cats(&character_buf, escaped);
    /*printf("<%s> [%s]\n", get_element_name(), escaped); */
  }
  free(escaped);
  free(chars);
}

const char*
mystr_basename(const char* filename) {
  char* s1 = strrchr(filename, '\\');
  char* s2 = strrchr(filename, '/');
  if(s2 > s1) s1 = s2;
  if(s1) return s1 + 1;
  return 0;
}

/* ----------------------------------------------------------------------- */
int
main(int argc, char* argv[]) {
  xmlnode* root_element = NULL;
  const char* filename = "sample.xml";
  /* initialize database */
  hmap_init(1024, &hashmap);
  hmap_init(1024, &instances_db);
  hmap_init(1024, &parts_db);
  if(argc > 1) {
    filename = argv[1];
  } else {
    buffer_putm(buffer_2, "Usage: ", mystr_basename(argv[0]), " <filename>");
    buffer_putnlflush(buffer_2);
    return 1;
  }
  /*   if(read_xmlfile(f)) {*/
  buffer input;
  buffer_mmapprivate(&input, filename);
  // xmlreader rd;
  // xml_read_callback(&rd, xml_callback);
  xmldoc = xml_read_tree(&input);
  /* Get the root element node */
  root_element = xmldoc->children;
  print_element_names(root_element);
//  print_list(instances_db);
  {
    const part_t* tmp = get_part("IC1");
    if(tmp) dump_part(tmp);
  }
  /*hmap_foreach(instances_db, &dump_instance);*/
  /*hmap_foreach(parts_db, &dump_part);*/
  hmap_foreach(parts_db, (void*)&each_part);
  buffer_flush(buffer_1);
  /* free up the resulting document */
  xml_free(xmldoc);
  return 0;
}

