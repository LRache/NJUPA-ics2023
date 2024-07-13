#include <proc.h>
#include <memory.h>
#include <am.h>

#define MAX_NR_PROC 4

PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
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
  AddrSpace as;
  protect(&as);
  pcb->as = as;

  uintptr_t entry = loader(pcb, filename, &as);
  pcb->cp = ucontext(&as, (Area){.start=pcb, .end=pcb+1}, (void *)entry);

  for (int i = 1; i < 8; i++) {
    void *paddr = pg_alloc(1);
    map(&as, (void *)(as.area.start - i * PGSIZE), paddr, 1);
  }
  
  char *p = (char *)pg_alloc(1);
  map(&as, (void *)(as.area.end - PGSIZE), (void *)p, 1);
  p += PGSIZE;

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
  p -= 4;
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
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
  //naive_uload(NULL, "/bin/cpp-test");
  char *const argv[] = {"a1", "a2", "a3", NULL};
  char *const envp[] = {"e1=1", "e2=2", "e3=3", NULL};
  //context_kload(&pcb[0], hello_fun, (void *)1);
  context_uload(&pcb[0], "/bin/bmp-test", argv, envp);
  pcbCount = 1;

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
  return current->cp;
}
