extern int lzma_code();
int
main() {
  void* ptr = &lzma_code;
  return (int)ptr;
}
