#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
static int pcbCount = 0;
static int currentPcbIndex = 0;
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void context_kload(PCB *p, void (*entry)(void *), void *arg) {
  Area kstack = {.start = p->stack, .end = p->stack+STACK_SIZE};
  Context *context = kcontext(kstack, entry, arg);
  p->cp = context;
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
  // naive_uload(NULL, "/bin/menu");
  context_kload(&pcb[0], hello_fun, (void *)1);
  context_kload(&pcb[1], hello_fun, (void *)2);
  pcbCount = 2;
  switch_boot_pcb();
  for (int i = 0; i < 2; i++) {
    Log("%d %p", i, pcb[i].cp);
  }
  yield();
}

Context* schedule(Context *prev) {
  if (pcbCount == 0) return NULL;
  Log("prev=%p", prev);
  //current->cp = prev;
  if (current == &pcb_boot) {
    current = pcb;
    return current->cp;
  }
  Log("set %d %p", currentPcbIndex, prev);
  currentPcbIndex = (currentPcbIndex + 1) % pcbCount; 
  current = pcb + currentPcbIndex;
  for(int i = 0; i < 2; i++) {
    Log("%d %p", i, pcb[i].cp);
  }
  return current->cp;
}
