#include <stdio.h>

#ifndef __MINGW32__
#ifndef __MINGW64__
#error "Not MINGW"
#endif
#endif

int main() {
	printf("OK\n");
	return 0;
}
