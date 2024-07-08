#include <proc.h>
#include <am.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static int pcbCount = 0;
static int pcbIndex = 0;
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
  
  char *p = (char *)(new_page(8) + 8 * PGSIZE);
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
    memcpy(p, argv[i], length);
    argvPointer[i] = p;
  }
  p-=4;
  *(uint32_t *)p = 0;
  for (int i = envpc-1; i >= 0; i--) {
    p -= 4;
    *(char **)p = envpPointer[i];
  }
  p -= 4;
  *(uint32_t *)p = 0;
  for (int i = argc-1; i >= 0; i--) {
    p -= 4;
    *(char **)p = argvPointer[i];
  }
  p -= 4;
  *(uint32_t *)p = argc;
  pcb->cp->gpr[10] = (intptr_t)p;
}

void execve(const char *filename, char *const argv[], char *const envp[]) {
  context_uload(current, filename, argv, envp);
  switch_boot_pcb();
  yield();
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
  char *const argv[] = {"/bin/exec-test", NULL};
  char *const envp[] = {NULL};
  context_uload(&pcb[0], "/bin/exec-test", argv, envp);
  pcbCount = 1;
  //context_kload(&pcb[0], hello_fun, (void *)0);

  yield();
}

Context* schedule(Context *prev) {
  if (pcbCount == 0) return prev;
  current->cp = prev;
  if (current == &pcb_boot) {
    pcbIndex = 0;
    current = pcb;
  } else {
    pcbIndex = (pcbIndex + 1) % pcbCount;
    current = pcb + pcbIndex;
  }
  Log("change to %d", pcbIndex);
  return current->cp;
}
