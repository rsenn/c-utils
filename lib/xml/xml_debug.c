#include "../fmt.h"
#include "../xml.h"
#include "../str.h"
#include "../stralloc.h"

#include <assert.h>

static void
xml_debug_nodelist(xmlnode* node, buffer* b, int depth) {
  do {
    int closing = node_is_closing(node);
    if(!closing)
      buffer_putnspace(b, depth * 2);

    if(node->type == XML_TEXT) {
      stralloc space, text;
      void* av[4];
      av[0] = (void*)" ";
      av[1] = (void*)(intptr_t)(depth * 2 + 6);

      stralloc_init(&space);
      stralloc_init(&text);
      stralloc_cats(&space, "\\n\"\n");

      stralloc_fmt_call(&space, (stralloc_fmt_fn*)(void*)&fmt_repeat, av);
      stralloc_catc(&space, '"');
      stralloc_nul(&space);
      stralloc_subst(&text, node->name, str_len(node->name), "\n", space.s);
      /* xml_escape(node->name, str_len(node->name), &text); */
      buffer_puts(b, "text \"");
      buffer_putsa(b, &text);
      buffer_puts(b, "\"\n");
      stralloc_free(&text);
      continue;
    }
    buffer_putm_internal(b,
                         "element[",
                         node_is_closing(node) ? &node->name[1] : node->name,
                         NULL);
    buffer_putm_internal(b, "] ", NULL);
    buffer_putm_internal(b, node_is_closing(node) ? " closing" : NULL, NULL);
    if(node->attributes)
      xml_print_attributes(node->attributes, b, " ", "=", "'");

    if(node->children) {
      int only_text_children = (node->children->type == XML_TEXT);
      buffer_puts(b, " ");
      buffer_puts(b, "\n");

      if(only_text_children) {
        xml_debug_nodelist(node->children, b, depth + 1);
      } else {
        xml_debug_nodelist(node->children, b, depth + 1);
      }
      buffer_putnspace(b, depth * 2);
    } else if(node->name[0] == '/' ||
              (node->next && node_is_closing(node->next))) {
      buffer_puts(b, " ");
    } else {
      buffer_puts(b, node->name[0] == '?' ? "?" : " self-closing");
      closing = 1;
    }
    if(closing)
      buffer_puts(b, ",");

    if(closing)
      buffer_putnlflush(b);
    else
      buffer_flush(b);
  } while((node = node->next));
}

void
xml_debug(xmlnode* node, buffer* b) {
  xml_debug_nodelist(node, b, 0);
}
