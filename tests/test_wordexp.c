extern int wordexp();
int main() {
  void *ptr = &wordexp;
  return (int)ptr;
}
