#include "cpu/cpu.h"
#include "cpu/decode.h"

#define RISCV_JAL_CODE 0b1101111
#define RISCV_JALR_CODE 

static CallTracer *callTracer = NULL; 
static CallTracer *callTracerTail = NULL;

void trace_function(Decode *_this) {
    if ((_this->isa.inst.val & RISCV_JAL_CODE) == RISCV_JAL_CODE) {
    if (callTracer == NULL) {
      callTracer = malloc(sizeof(CallTracer));
      callTracerTail = callTracer;
    } else {
      callTracerTail->next = malloc(sizeof(CallTracer));
      callTracerTail = callTracerTail->next;
    }
    callTracerTail->dst = _this->dnpc;
    callTracerTail->pc = _this->pc;
    callTracerTail->type = FUN_CAL;
    callTracerTail->next = NULL;
  }
}