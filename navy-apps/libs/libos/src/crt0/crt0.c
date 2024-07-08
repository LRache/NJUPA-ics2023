#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *sp) {
  char *empty[] =  {NULL };
  environ = empty;
  int argc = *(int *)sp;
  char **args = (char **)(sp+1);
  char **envp = (char **)(sp + argc + 1);
  int r = main(argc, args, envp);
  exit(r);
  assert(0);
}
