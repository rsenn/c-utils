#ifndef XML_INTERNAL_H
#define XML_INTERNAL_H

#include "xml.h"
#include "strlist.h"

/* The arguments of the xml_find_*_pred_N, xml_find_all_N and xml_pfind_*
 * functions are C strings (or NULL), each a list whose items are separated by
 * '|'. xml_pargs_make turns the first three of ptr[] into strlists held in
 * lists[3], ptr[i] points to them afterwards and the predicate gets them;
 * xml_pargs_free releases them. */
void xml_pargs_make(const void* ptr[], strlist lists[3]);
void xml_pargs_free(strlist lists[3]);

#endif
