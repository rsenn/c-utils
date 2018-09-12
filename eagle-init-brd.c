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
#include "lib/strlist.h"
#include "lib/xml.h"

#include "round.c"

#define END_OF_LINE "; "
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
static strlist cmds;

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
  buffer_putdouble(buffer_2, p->x, 1);
  buffer_puts(buffer_2, ",y=");
  buffer_putdouble(buffer_2, p->y, 1);
  buffer_puts(buffer_2, ",rot=");
  buffer_putdouble(buffer_2, p->rot, 0);
  buffer_putnlflush(buffer_2);
}

/* ----------------------------------------------------------------------- */
void
dump_instance(instance_t const* i) {
  buffer_putm(buffer_2, "dump_instance \"", i->part, ":", i->gate, "\"");
  buffer_puts(buffer_2, " x=");
  buffer_putdouble(buffer_2, i->x, 2);
  buffer_puts(buffer_2, ", y=");
  buffer_putdouble(buffer_2, i->y, 2);
  buffer_puts(buffer_2, ", rot=");
  buffer_putdouble(buffer_2, i->rot, 0);
  buffer_putnlflush(buffer_2);
}

/* ----------------------------------------------------------------------- */
double
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
output_move(const char* name, double x, double y) {
  stralloc cmd;
  stralloc_init(&cmd);

  stralloc_catm(&cmd, "MOVE ", name, " (");
  stralloc_catdouble(&cmd, x, 2);
  stralloc_catc(&cmd, ' ');
  stralloc_catdouble(&cmd, y, 2);
  stralloc_catc(&cmd, ')');

  strlist_push_sa(&cmds, &cmd);
  stralloc_free(&cmd);
}

/* ----------------------------------------------------------------------- */
void
output_rotate(const char* name, long angle) {
  stralloc cmd;
  stralloc_init(&cmd);

  stralloc_cats(&cmd, "ROTATE =R");
  stralloc_catlong(&cmd, angle % 360);
  stralloc_catm(&cmd, " '", name, "'");

  strlist_push_sa(&cmds, &cmd);
  stralloc_free(&cmd);
}

/* ----------------------------------------------------------------------- */
void
each_part(part_t* p) {
  if(p->device[0] != '\0' || p->value[0] != '\0') {

    output_move(p->name, p->x - min_x, p->y - min_y);

    if(fabs(p->rot) >= 0.1) {
      int angle = (int)((p->rot / 90)) * 90.0;
      while(angle < 0) angle += 360;
      while(angle > 360) angle -= 360;

      output_rotate(p->name, angle);
    }
  }

#ifdef DEBUG
  dump_part(p);
#endif
}

/* ----------------------------------------------------------------------- */
part_t*
get_part(const char* part) {
  TUPLE* ptr_tuple = NULL;
  part_t* p = NULL;
  hmap_search(parts_db, (char*)part, str_len(part), &ptr_tuple);
  if(ptr_tuple) p = ptr_tuple->vals.val_custom;
  return p;
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
hmap_foreach(HMAP_DB* hmap, void (*foreach_fn)(void*)) {
  TUPLE* t;
  if(hmap == NULL) return;
  for(t = hmap->list_tuple; t; t = t->next) {
    if(t->data_type == HMAP_DATA_TYPE_CUSTOM) foreach_fn(t->vals.val_custom);
    if(t->next == hmap->list_tuple) break;
  }
}

/* ----------------------------------------------------------------------- */
int
get_attribute_double(double* d, xmlnode* e, const char* name) {
  char* value;
  if(!(value = xml_get_attribute(e, name))) return 0;
  return value[scan_double(value, d)] != '\0';
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
  xml_get_attribute_sa(e, &part, "part");
  stralloc_init(&gate);
  xml_get_attribute_sa(e, &gate, "gate");
  stralloc_init(&rot);
  xml_get_attribute_sa(e, &rot, "rot");
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
  xml_get_attribute_sa(e, &name, "name");
  stralloc_init(&library);
  xml_get_attribute_sa(e, &library, "library");
  stralloc_init(&deviceset);
  xml_get_attribute_sa(e, &deviceset, "deviceset");
  stralloc_init(&device);
  xml_get_attribute_sa(e, &device, "device");
  stralloc_init(&value);
  xml_get_attribute_sa(e, &value, "value");
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
int read_xmlfile(const char* filename);
int parse_xmlfile(const char* filename, xmlnode** p_doc);

/* ----------------------------------------------------------------------- */
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

  buffer input;
  buffer_mmapprivate(&input, filename);

  xmldoc = xml_read_tree(&input);

  /* Get the root element node */
  root_element = xmldoc->children;
  print_element_names(root_element);

  hmap_foreach(parts_db, (void*)&each_part);
  buffer_flush(buffer_1);

  stralloc out;
  stralloc_init(&out);
  strlist_joins(&cmds, &out, "; ");
  buffer_putsa(buffer_1, &out);
  buffer_putnlflush(buffer_1);

  /* free up the resulting document */
  xml_free(xmldoc);
  return 0;
}
