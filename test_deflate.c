extern int deflate();
int
main() {
  void* ptr = &deflate;
  return (int)ptr;
}
