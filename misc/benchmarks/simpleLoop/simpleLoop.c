int main() {
  int x = 0;
  for (int i = 0; i < 1000000; i = i + 1) {
    x = x + 1;
  }

  return x;
}
