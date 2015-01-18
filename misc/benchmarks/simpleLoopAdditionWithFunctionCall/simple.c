int add(int a, int b)
{
  return a + b;
}
int main() {
  int x = 0;

  for (int i = 0; i < 1000000; i = i + 1) {
    x = add(x, 1);;
  }

  return x;
}
