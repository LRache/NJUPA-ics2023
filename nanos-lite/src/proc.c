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

void context_kload(PCB *pcb, void (*entry)(void *), void *arg) {
  Area kstack = {.start = pcb->stack, .end = pcb->stack+STACK_SIZE};
  Context *context = kcontext(kstack, entry, arg);
  pcb->cp = context;
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
  yield();
}

Context* schedule(Context *prev) {
  if (pcbCount == 0) return NULL;
  Log("%p %d", prev, currentPcbIndex);
  current->cp = prev;
  currentPcbIndex = (currentPcbIndex + 1) % pcbCount; 
  Log("%d", currentPcbIndex);
  current = pcb + currentPcbIndex;
  Log("%p", current->cp);
  return current->cp;
}
