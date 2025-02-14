#include "../tokenizer_internal.h"

void
tokenizer_register_marker(tokenizer* t,
                          enum markertype mt,
                          const char* marker) {
  t->marker[mt] = marker;
}
