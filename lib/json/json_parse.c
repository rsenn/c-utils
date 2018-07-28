#include "../json.h"
#include "../charbuf.h"
#include <ctype.h>


int
json_parse_array(jsonval* j, charbuf* b) {
	j->type = JSON_ARRAY;



}

int
json_parse_object(jsonval* j, charbuf* b) {
	j->type = JSON_OBJECT;



}
int
json_parse(jsonval* j, charbuf* b) {

	charbuf_skip_pred(b, &isspace);

	if(charbuf_peek(b) == '[')
		return json_parse_array(j, b);
	if(charbuf_peek(b) == '{')
		return json_parse_object(j, b);

}
