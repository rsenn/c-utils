extern int sigtimedwait();
int
main() {
  void* ptr = &sigtimedwait;
  return (int)ptr;
}
