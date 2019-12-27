message("Checking C11 generics")

file(WRITE "${CMAKE_BINARY_DIR}/try_c11generics.c" "
void print_int(int n) { printf(\"%d\\n\", n); }
void print_float(float n) { printf(\"%f\\n\", n); }
void print_double(double n) { printf(\"%lf\\n\", n); }
#define print(num) _Generic((num), int: print_int, float: print_float, double: print_double)(num)

void test_c11generics() {
  int i = 123;
  float f = 4.342;
  double d = 3.141592;
  print(i);
  print(f);
  print(d);
}
int main() {
  test_c11generics();
}")
try_compile(HAVE_C11_GENERICS "${CMAKE_BINARY_DIR}" SOURCES "${CMAKE_BINARY_DIR}/try_c11generics.c")

if(HAVE_C11_GENERICS)
  message("Checking C11 generics - supported")
else(HAVE_C11_GENERICS)
  message("Checking C11 generics - not supported")
endif(HAVE_C11_GENERICS)

if(HAVE_C11_GENERICS)
  add_definitions(-DHAVE_C11_GENERICS=1)
endif(HAVE_C11_GENERICS)