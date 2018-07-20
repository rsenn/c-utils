#include "lib/xml.h"

int
xml_read_node(xmlreader* r, xmlnodeid id, stralloc* name,  stralloc* value) {

}


int main() {

  buffer input;
  xmlreader reader;

  buffer_mmapprivate(&input, "test.xml");

  xml_read(&reader, &input, &xml_read_node);


}
