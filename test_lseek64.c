extern int lseek64();
int
main() {
  void* ptr = &lseek64;
  return (int)ptr;
}
