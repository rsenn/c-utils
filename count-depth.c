#include <unistd.h>
#include <limits.h>
#include "buffer.h"

static char buffer_0_in[BUFFER_INSIZE];
static buffer buffer_0 = BUFFER_INIT((void*)read, 0, buffer_0_in, BUFFER_INSIZE);

static char buffer_1_out[BUFFER_OUTSIZE];
static buffer buffer_1 = BUFFER_INIT((void*)write, 1, buffer_1_out, BUFFER_OUTSIZE);

int count_depth()
{
				char buffer[PATH_MAX];

				while(buffer_getline(&buffer_0, buffer, sizeof(buffer)))
				{


				}
}

int main(int argc, char *argv[])
{

count_depth();
}

