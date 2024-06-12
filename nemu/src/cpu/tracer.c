#include "cpu/cpu.h"
#include "cpu/decode.h"

#define RISCV_JAL_CODE 0b1101111
#define RISCV_JALR_CODE 

static FunTracer *funTracer = NULL; 
static FunTracer *funTracerTail = NULL;

void trace_function(Decode *_this) {
    if ((_this->isa.inst.val & RISCV_JAL_CODE) == RISCV_JAL_CODE) {
    if (funTracer == NULL) {
      funTracer = malloc(sizeof(FunTracer));
      funTracerTail = funTracer;
    } else {
      funTracerTail->next = malloc(sizeof(FunTracer));
      funTracerTail = funTracer->next;
    }
    funTracerTail->dst = _this->dnpc;
    funTracerTail->pc = _this->pc;
    funTracerTail->type = FUN_CAL;
    funTracerTail->next = NULL;
  }
}

void free_function_tracer() {
    FunTracer *t;
    while (funTracer) {
        t = funTracer->next;
        free(funTracer);
        funTracer = t;
    }
    funTracer = NULL;
}
