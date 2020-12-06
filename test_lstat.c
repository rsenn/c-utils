extern int lstat();
int
main() {
  void* ptr = &lstat;
  return (int)ptr;
}
