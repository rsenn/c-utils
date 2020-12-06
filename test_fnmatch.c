extern int fnmatch();
int
main() {
  void* ptr = &fnmatch;
  return (int)ptr;
}
