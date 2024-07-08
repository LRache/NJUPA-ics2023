#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  char *empty[] =  {NULL };
  environ = empty;
  int argc = *(int *)args;
  char **args = (char **)(args + 1);
  char **envp = (char **)(args + argc + 2);
  int r = main(argc, args, envp);
  exit(r);
  assert(0);
}
