extern int posix_memalign();
int
main() {
  void* ptr = &posix_memalign;
  return (int)ptr;
}
