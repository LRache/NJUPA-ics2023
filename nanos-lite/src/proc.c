#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void context_kload(PCB *p, void (*entry)(void *), void *arg) {
  Area kstack = {.start = p, .end = p+1};
  Context *context = kcontext(kstack, entry, arg);
  p->cp = context;
}

void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]) {
  uintptr_t entry = loader(pcb, filename);
  pcb->cp = ucontext(NULL, (Area){.start=pcb, .end=pcb+1}, (void *)entry);
  
  char *p = (char *)0x87ffffff;
  int argc = 0, envpc = 0;
  while (argv[argc] != NULL) argc++;
  while (envp[envpc] != NULL) envpc++;
  char *argvPointer[argc], *envpPointer[envpc];
  for (int i = envpc - 1; i >= 0; i--) {
    int length = strlen(envp[i]) + 1;
    p -= length;
    memcpy(p, envp[i], length);
    envpPointer[i] = p;
  }
  p--;
  *p = 0;
  for (int i = argc - 1; i >= 0; i--) {
    int length = strlen(argv[i]) + 1;
    p -= length;
    memcpy(p, envp[i], length);
    argvPointer[i] = p;
  }
  p--;
  *p = 0;
  for (int i = envpc-1; i >= 0; i--) {
    p -= 4;
    *(char **)p = envpPointer[i];
  }
  p -= 4;
  *(uint32_t *)p = 0;
  for (int i = argc-1; i >= 0; i--) {
    p -= 4;
    *(char **)p = argvPointer[i];
    Log("%p", p);
  }
  p -= 4;
  *(uint32_t *)p = argc;
  pcb->cp->gpr[10] = (intptr_t)p;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    // Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
  //naive_uload(NULL, "/bin/cpp-test");
  char * const argv[] = {"1a", "2b", "3c", NULL};
  char * const envp[] = {"a=1", "b=2", "c=3", NULL};
  context_uload(&pcb[1], "/bin/mainargs-test", argv, envp);
  context_kload(&pcb[0], hello_fun, (void *)0);

  yield();
}

Context* schedule(Context *prev) {
  current->cp = prev;
  if (current != pcb) current = pcb;
  else current = pcb+1;
  return current->cp;
}
