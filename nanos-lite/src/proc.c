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

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  pcb->cp = ucontext(NULL, (Area){.start=pcb, .end=pcb+1}, (void *)entry);
  pcb->cp->gpr[10] = 0x87000000;
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
  context_uload(&pcb[0], "/bin/menu");
  // context_kload(&pcb[1], hello_fun, (void *)2);

  yield();
}

Context* schedule(Context *prev) {
  current->cp = prev;
  if (current != pcb) current = pcb;
  else current = pcb+1;
  return current->cp;
}
