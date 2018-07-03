/**
 * section: xmlReader
 * synopsis: Parse multiple XML files reusing an xmlReader
 * purpose: Demonstrate the use of xmlReaderForFile() and
 * xmlReaderNewFile to parse XML files while reusing the reader object
 * and parser context.  (Note that the XMLReader functions require
 * libxml2 version later than 2.6.)
 * usage: reader4 <filename> [ filename ... ]
 * test: reader4 test1.xml test2.xml test3.xml > reader4.tmp && diff reader4.tmp $(srcdir)/reader4.res
 * author: Graham Bennett
 * copy: see Copyright for the status of this software.
 */

#include "buffer.h"
#include <libxml/xmlreader.h>

#ifdef LIBXML_READER_ENABLED

static void processDoc(xmlTextReaderPtr readerPtr) {
    int ret;
    xmlDocPtr docPtr;
    const xmlChar *URL;

    ret = xmlTextReaderRead(readerPtr);
    while(ret == 1) {
      ret = xmlTextReaderRead(readerPtr);
    }

    /*
     * One can obtain the document pointer to get insteresting
     * information about the document like the URL, but one must also
     * be sure to clean it up at the end (see below).
     */
    docPtr = xmlTextReaderCurrentDoc(readerPtr);
    if(NULL == docPtr) {
      buffer_putsflush(buffer_2, "failed to obtain document\n");
      return;
    }
      
    URL = docPtr->URL;
    if(NULL == URL) {
      buffer_putsflush(buffer_2, "Failed to obtain URL\n");      
    }

    if(ret != 0) {
      buffer_putmflush(buffer_2, URL, ": Failed to parse\n");
      return;
    }

    printf("%s: Processed ok\n", (const char *)URL);
}

int main(int argc, char **argv) {
    xmlTextReaderPtr readerPtr;
    int i;
    xmlDocPtr docPtr;

    if(argc < 2)
        return(1);

    /*
     * this initialises the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /*
     * Create a new reader for the first file and process the
     * document.
     */
    readerPtr = xmlReaderForFile(argv[1], NULL, 0);
    if(NULL == readerPtr) {
      buffer_putmflush(buffer_2, argv[1], ": failed to create reader\n");      
      return(1);
    }
    processDoc(readerPtr);

    /*
     * The reader can be reused for subsequent files.
     */
    for(i=2; i < argc; ++i) {
      	xmlReaderNewFile(readerPtr, argv[i], NULL, 0);
	if(NULL == readerPtr) {
	  buffer_putmflush(buffer_2, argv[i], ": failed to create reader\n");      
	  return(1);
	}
        processDoc(readerPtr);
    }

    /*
     * Since we've called xmlTextReaderCurrentDoc, we now have to
     * clean up after ourselves.  We only have to do this the last
     * time, because xmlReaderNewFile calls xmlCtxtReset which takes
     * care of it.
     */
    docPtr = xmlTextReaderCurrentDoc(readerPtr);
    if(docPtr != NULL)
      xmlFreeDoc(docPtr);

    /*
     * Clean up the reader.
     */
    xmlFreeTextReader(readerPtr);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}

#else
int main(void) {
    buffer_putsflush(buffer_2, "xmlReader support not compiled in\n");
    exit(1);
}
#endif
