#include <stdio.h>

class Test {
public:
  Test()  { printf("%s,%d: Hello, Project-N!\n", __func__, __LINE__); }
  ~Test() { printf("%s,%d: Goodbye, Project-N!\n", __func__, __LINE__); }
};

Test test;

int main() {
  #ifdef HAVE_INITFINI_ARRAY
  printf("A\n");
  #endif
  printf("%s,%d: Hello world!\n", __func__, __LINE__);
  return 0;
}
