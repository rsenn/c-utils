extern int kevent();
int main() {
  void *ptr = &kevent;
  return (int)ptr;
}
