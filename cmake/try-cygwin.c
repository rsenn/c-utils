#include <stdio.h>

#ifndef __CYGWIN__
#error "Not CYGWIN"
#endif

int main() {
	printf("OK\n");
	return 0;
}
