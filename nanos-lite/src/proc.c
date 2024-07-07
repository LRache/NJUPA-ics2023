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

void hello_fun(void *arg) {
  // int j = 1;
  // while (1) {
  //   Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
  //   j ++;
  //   yield();
  // }
  while (1) {
    putch("?AB"[(uintptr_t)arg > 2 ? 0 : (uintptr_t)arg]);
    for (int volatile i = 0; i < 100000; i++) ;
    yield();
  }
}

void init_proc() {
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
  // naive_uload(NULL, "/bin/menu");
  // context_kload(&pcb[0], hello_fun, (void *)1);
  // context_kload(&pcb[1], hello_fun, (void *)2);
  pcb[0].cp = kcontext((Area) { pcb[0].stack, &pcb[0] + 1 }, hello_fun, (void *)1L);
  pcb[1].cp = kcontext((Area) { pcb[1].stack, &pcb[1] + 1 }, hello_fun, (void *)2L);

  yield();
}

Context* schedule(Context *prev) {
  current->cp = prev;
  if (current != pcb) current = pcb;
  else current = pcb+1;
  Log("%p", current->cp);
  return current->cp;
}
