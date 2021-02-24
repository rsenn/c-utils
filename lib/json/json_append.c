#include "../json_internal.h"
#include "../alloc.h"

jsonval*
json_append(jsonval* arr, const jsonval value) {
    jsonval* v;


    if((v = json_push(arr))) 
      memcpy(v, &value, sizeof(jsonval));

    return v;
}
