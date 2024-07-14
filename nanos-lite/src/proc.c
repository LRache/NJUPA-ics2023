#include <proc.h>
#include <memory.h>
#include <am.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] = {};
static PCB *running[MAX_NR_PROC];
static int runningCount = 0;
static int runningIndex = 0;
static PCB pcb_boot = {};
// static PCB program[MAX_NR_PROC] = {};
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
  // char *const argv[] = {"/bin/hplayer", "/home/rache/Music/xitiejie.pcm", NULL};
  char *const empty[] = {NULL};
  context_uload(&pcb[0], "/bin/hello", empty, empty);
  context_uload(&pcb[1], "/bin/hello", empty, empty);
  running[0] = &pcb[0];
  running[1] = &pcb[1];
  runningCount = 2;

  yield();
}

void proc_exit(int r) {
  Log("Exit with code %d", r);
  for (int i = runningIndex; i < runningCount-1; i++) {
    running[i] = running[i+1];
  }
  running[runningCount - 1] = NULL;
  runningCount--;
  if (runningCount == 0) halt(0);
  yield();
}

Context* schedule(Context *prev) {
  if (runningCount == 0) return prev;
  current->cp = prev;
  if (current == &pcb_boot) {
    runningIndex = 0;
    current = running[0];
  } else {
    runningIndex = (runningIndex + 1) % runningCount;
    current = running[runningIndex];
  }
  return current->cp;
}
