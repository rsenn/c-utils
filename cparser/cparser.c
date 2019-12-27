#include "ansi-c.tab.h"
#include "buffer.h"
#include "errmsg.h"
#include <unistd.h>
const char* yytokname(int tok);
extern const char* yytext;

static char inbuf[1024];
buffer input_buffer = BUFFER_INIT(&read, 0, inbuf, sizeof(inbuf));
const char* input_file = "<stdin>";

int
main(int argc, char* argv[]) {
  if(argc >= 2) {
    if(buffer_mmapread(&input_buffer, argv[1])) {
      errmsg_warnsys("ERROR: open input file '", argv[1], "'", 0);
      exit(1);
    } else {
      input_file = argv[1];
    }
  }

  int token, result = 1;

  /*do {
    token = yylex();
    const char* tokname = yytokname(token);

    buffer_puts(buffer_2, "Token: ");
    buffer_putspad(buffer_2, tokname, 20);
    buffer_putspad(buffer_2, yytext, 20);
    buffer_puts(buffer_2, " (");
    buffer_putlong(buffer_2, token);
    buffer_puts(buffer_2, ")");
    buffer_putnlflush(buffer_2);

  } while(token);*/

  result = yyparse();

  buffer_puts(buffer_2, "Result: ");
  buffer_putlong(buffer_2, result);
  buffer_putnlflush(buffer_2);
  return !(result == 0);
}
