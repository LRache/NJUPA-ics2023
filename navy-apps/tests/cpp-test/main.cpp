#include <stdio.h>

class Test {
public:
  Test()  { printf("%s,%d: Hello, Project-N!\n", __func__, __LINE__); }
  ~Test() { printf("%s,%d: Goodbye, Project-N!\n", __func__, __LINE__); }
};

Test test;

extern "C" {
  extern void __libc_init_array(void);
}

void (*f)(void) = __libc_init_array;

int main() {
  printf("%s,%d: Hello world!\n", __func__, __LINE__);
  return 0;
}
