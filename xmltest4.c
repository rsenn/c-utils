#include "lib/xml.h"


int main() {

  buffer input;
  xmlreader reader;

  buffer_mmapprivate(&input, "test.xml");

  xml_read(&reader, &input);


}
