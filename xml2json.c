#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include "lib/str.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/strlist.h"
#include "lib/json.h"
#include "lib/array.h"
#include "lib/textcode.h"
#include "lib/errmsg.h"
#include "lib/open.h"
#include "lib/getopt.h"

static jsonval xml_to_json_obj(xmlnode* node);

static int one_line = 0, indent = 2, compact = 0, numbers = 0, no_quote = 0;
static stralloc indent_str = {"  ", 1, 0};
static const char* children_property = "children";
static const char* tag_property = "tagName";
static const char* class_property = "className";
static char quote_char[2] = {'"', '\0'};

static void
max_depth_fn(jsonval* v, int* arg, int depth) {
    if(depth > *arg)
        *arg = depth;
}

/**
 * @brief      Gets depth of JSON node tree.
 * @param      v     JSON value
 * @return     The depth.
 */
static int
get_depth(jsonval* v) {
    int max_depth = -1;
    json_recurse(v, max_depth_fn, &max_depth);
    return max_depth;
}

/**
 * @brief      { function_description }
 * @param      p      { parameter_description }
 * @param      v      { parameter_description }
 * @param[in]  depth  The depth
 * @param[in]  index  The index
 */
static void
pretty_printer(jsonfmt* p, jsonval* v, int depth, int index) {
    int valdepth = v ? get_depth(v) : -1;

    p->newline = valdepth > 1
                 ? "\n"
                 : " "; // (!one_line && valdepth > 1 && ((index > -1) || index < -2) && index > 0) ? "\n" : "";
    p->indent = indent_str.s;
    p->spacing = " ";
    p->separat = valdepth > 1 ? ",\n" : ", ";
    p->precision = 5;
    p->depth = depth;
    p->index = index;
    p->quote = quote_char;

    if(v == NULL) {
        p->quote = no_quote ? "\0" : quote_char;
        return;
    }

    if(numbers && json_isnumber(*v))
        p->quote = "\0";

    if(compact) {
        p->newline = p->indent = p->spacing = "";
        p->separat = ",";
        return;
    }
}

/**
 * @brief      { function_description }
 * @param      list  The list
 * @return     { description_of_the_return_value }
 */
static jsonval
xmllist_to_jsonarray(xmlnode* list) {
    jsonval arr = json_array();
    xmlnode* n;
    for(n = list; n; n = n->next) {
        jsonval v = json_undefined();
        if(n->type == XML_TEXT) {
            if(!n->name[0] || str_is(n->name, isspace))
                continue;
            v = json_string(n->name);
        } else {
            if(n->name[0] == '/')
                continue;
            v = xml_to_json_obj(n);
        }
        json_push(&arr, v);
    }
    return arr;
}

static jsonval
xml_style_json(char* x, size_t n) {
    static const char* const whitespace = " \t\v\r\n\0";
    strlist style;
    jsonval r = json_object();
    strlist_init(&style, ';');
    style.sa.s = x;
    style.sa.len = n;
    strlist_foreach(&style, x, n) {
        size_t np = byte_chr(x, n, ':');
        size_t nv = np == n ? 0 : n - (np + 1);
        char* s = &x[np + 1];
        char* value = (s && *s == '\0') ? s : byte_trim(s, &nv, whitespace, 6);
        char* prop = byte_trim(x, &np, whitespace, 6);
        np = byte_camelize(prop, np);
        if(np > 0) {
            jsonval name = json_stringn(prop, np);
            jsonval val = json_undefined();
            charbuf b;
            char* ptr = value;
            value[nv] = '\0';
            charbuf_froms(&b, &ptr);

            if(!numbers || !json_parse_num(&val, &b))
                val = json_stringn(value, nv);

            json_set_property(&r, name, val);
        }
    }
    strlist_free(&style);
    return r;
}

/**
 * @return     1 on succes
 */
static int
hmap_to_jsonobj(HMAP_DB* db, jsonval* obj) {
    if(db && db->list_tuple) {
        TUPLE* t;
        hmap_foreach(db, t) {
            const char* prop = t->key;
            jsonval v = json_undefined();
            if(str_equal(prop, "style")) {
                v = xml_style_json(t->vals.val_chars, t->data_len);
            } else {
                size_t len = t->data_len;
                charbuf b;
                char* ptr;
                if(len > 0 && t->vals.val_chars[len - 1] == '\0')
                    --len;
                if(str_equal(prop, "class") || str_equal(prop, "className"))
                    prop = class_property;
                ptr = t->vals.val_chars;
                charbuf_froms(&b, &ptr);
                if(!numbers || !json_parse_num(&v, &b))
                    v = json_stringn(t->vals.val_chars, len);
            }
            json_set_property(obj, json_string(prop), v);
        }
        return 1;
    }
    return 0;
}

/**
 * @brief      { function_description }
 * @param      node  The node
 * @return     { description_of_the_return_value }
 */
static jsonval
xml_to_json_obj(xmlnode* node) {
    /*  static const char* const node_types[] = {"(null)", "XML_DOCUMENT", "XML_ELEMENT", "XML_ATTRIBUTE", "XML_TEXT" };
        buffer_putm_internal(buffer_2, node_types[(int)node->type], " ", node->name, "\n", 0);*/
    if(node->type == XML_ELEMENT) {
        jsonval obj = json_object();
        json_set_property(&obj, json_string(tag_property), json_string(node->name));
        if(node->attributes && node->attributes->list_tuple)
            hmap_to_jsonobj(node->attributes, &obj);
        if(node->children)
            json_set_property(&obj, json_string(children_property), xmllist_to_jsonarray(node->children));
        return obj;
    }
    if(node->type == XML_TEXT) {
    }
    return json_undefined();
}

/**
 *
 */
static jsonval
xml_to_json(xmlnode* node) {
    if(node->type == XML_DOCUMENT || node->attributes == NULL)
        return xmllist_to_jsonarray(node->children);
    return xml_to_json_obj(node);
}

static int
xml_depth(xmlnode* node) {
    int i = 0;
    while(node) {
        node = node->parent;
        ++i;
    }
    return i;
}

void
usage(char* av0) {
    buffer_putm_internal(buffer_1,
                         "Usage: ",
                         str_basename(av0),
                         " [OPTIONS] [FILES...]\n"
                         "\n"
                         "Options:\n"
                         "\n"
                         "  -h, --help              Show this help\n"
                         "  -s, --single-quote      Use ' as quote\n"
                         "  -d, --double-quote      Use \" as quote\n"
                         "  -o, --one-line          One-line\n"
                         "  -c, --compact           Compact\n"
                         "  -l, --indent NUM        Indent level\n"
                         "  -n, --numbers           Show numbers unquoted\n"
                         "\n"
                         "  -T, --tag NAME          Name of property with tag name [",
                         tag_property,
                         "]\n"
                         "  -C, --children NAME     Name of property with children [",
                         children_property,
                         "]\n"
                         "  -N, --class NAME        Name of property with classes [",
                         class_property,
                         "]\n"
                         "\n",
                         0);
    buffer_flush(buffer_1);
}

int
testwalk(xmlnode* node, xmlnode* root) {
    if(node->name && node->name[0] != '/' && node->type == XML_ELEMENT) {
        int depth = xml_depth(node);
        buffer_putm_internal(buffer_2, "walk: ", node->name, " depth: ", 0);
        buffer_putlong(buffer_2, depth);
        buffer_putnlflush(buffer_2);
    }
    return 0;
}

int
main(int argc, char* argv[]) {
    buffer* input = buffer_0;
    xmlnode* doc;
    static xmlnodeset ns;
    xmlnodeset_iter_t it, e;
    size_t i = 0;
    int c;
    int index = 0;

    struct longopt opts[] = {{"help", 0, NULL, 'h'},
        {"single-quote", 0, NULL, 's'},
        {"double-quote", 0, NULL, 'd'},
        {"one-line", 0, NULL, 'o'},
        {"compact", 0, NULL, 'c'},
        {"indent", 0, NULL, 'l'},
        {"numbers", 0, NULL, 'n'},
        {"no-quote", 0, NULL, 'Q'},
        {"tag", 0, NULL, 'T'},
        {"children", 0, NULL, 'C'},
        {"class", 0, NULL, 'N'},
        {0, 0, 0, 0}
    };

    errmsg_iam(argv[0]);

    for(;;) {
        c = getopt_long(argc, argv, "hsdol:cT:C:N:nQ", opts, &index);
        if(c == -1)
            break;
        if(c == 0)
            continue;

        switch(c) {
        case 'h':
            usage(argv[0]);
            return 0;
        case 's':
            quote_char[0] = '\'';
            break;
        case 'd':
            quote_char[0] = '"';
            break;
        case 'o':
            one_line = 1;
            break;
        case 'c':
            compact = 1;
            break;
        case 'n':
            numbers = 1;
            break;
        case 'Q':
            no_quote = 1;
            break;
        case 'l':
            scan_int(optarg, &indent);
            break;
        case 'T':
            tag_property = optarg;
            break;
        case 'C':
            children_property = optarg;
            break;
        case 'N':
            class_property = optarg;
            break;
        default:
            usage(argv[0]);
            return 1;
        }
    }

    while(optind < argc) {

        if(argv[optind])
            input->fd = open_read(argv[optind]);

        buffer_skip_until(input, "\r\n", 2);
        doc = xml_read_tree(input);
        // xml_walk(doc, testwalk);

        buffer_close(input);
        {
            jsonval root = xml_to_json(doc);

            json_print(root, buffer_1, pretty_printer);
        }
        /*
         * Cleanup function for the XML library.
         */
        xml_free(doc);

        optind++;
    }

    return (0);
}
