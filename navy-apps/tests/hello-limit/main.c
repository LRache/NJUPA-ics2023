#include <unistd.h>
#include <stdio.h>

int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  for (int i = 0; i < 100; i++) {
    printf("Hello World from Navy-apps for the %dth time!\n", i);
    for (volatile int j = 0; j < 10000; j++);
  }
  return 0;
}
