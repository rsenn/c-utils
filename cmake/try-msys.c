#include <stdio.h>

#ifndef __MSYS__
#error "Not MSYS"
#endif

int
main() {
  printf("OK\n");
  return 0;
}
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) 
__attribute__((stdcall)) 
{}
