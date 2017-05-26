
#include <stdio.h>
#include <string.h>
#include <libxml/SAX.h>

int read_xmlfile(FILE *f);
xmlSAXHandler make_sax_handler();

static void OnStartElementNs(
    void *ctx,
    const xmlChar *localname,
    const xmlChar *prefix,
    const xmlChar *URI,
    int nb_namespaces,
    const xmlChar **namespaces,
    int nb_attributes,
    int nb_defaulted,
    const xmlChar **attributes
);

static void OnEndElementNs(
    void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI
);

static void OnCharacters(void* ctx, const xmlChar * ch, int len);

int main(int argc, char *argv[]) {
    FILE *f = fopen("sample.xml", "r");
    if (!f) {
        puts("file open error.");
        exit(1);
    }

    if(read_xmlfile(f)) {
        puts("xml read error.");
        exit(1);
    }

    fclose(f);
    return 0;
}

int read_xmlfile(FILE *f) {
    char chars[1024];
    int res = fread(chars, 1, 4, f);
    if (res <= 0) {
        return 1;
    }

    xmlSAXHandler SAXHander = make_sax_handler();
 
    xmlParserCtxtPtr ctxt = xmlCreatePushParserCtxt(
        &SAXHander, NULL, chars, res, NULL
    );

    while ((res = fread(chars, 1, sizeof(chars), f)) > 0) {
        if(xmlParseChunk(ctxt, chars, res, 0)) {
            xmlParserError(ctxt, "xmlParseChunk");
            return 1;
        }
    }
    xmlParseChunk(ctxt, chars, 0, 1);

    xmlFreeParserCtxt(ctxt);
    xmlCleanupParser();
    return 0;
}

xmlSAXHandler make_sax_handler (){
    xmlSAXHandler SAXHander;

    memset(&SAXHander, 0, sizeof(xmlSAXHandler));

    SAXHander.initialized = XML_SAX2_MAGIC;
    SAXHander.startElementNs = OnStartElementNs;
    SAXHander.endElementNs = OnEndElementNs;
    SAXHander.characters = OnCharacters;

    return SAXHander;
}

static void OnStartElementNs(
    void *ctx,
    const xmlChar *localname,
    const xmlChar *prefix,
    const xmlChar *URI,
    int nb_namespaces,
    const xmlChar **namespaces,
    int nb_attributes,
    int nb_defaulted,
    const xmlChar **attributes
) {
    printf("<%s>\n", localname);
}

static void OnEndElementNs(
    void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI
) {
    printf("</%s>\n", localname);
}

static void OnCharacters(void *ctx, const xmlChar *ch, int len) {
    char chars[len + 1];
    strncpy(chars, (const char *)ch, len);
    chars[len] = (char)NULL;
    printf("[%s]\n", chars);
}