#include "cpu/cpu.h"
#include "cpu/decode.h"
#include "cpu/tracer.h"

static FunTracer *funTracer = NULL; 
static FunTracer *funTracerTail = NULL;

void trace_function(Decode *_this) {
    if (BITS(_this->isa.inst.val, 6, 0) == 0b1101111) {
        if (funTracer == NULL) {
            funTracer = malloc(sizeof(FunTracer));
            funTracerTail = funTracer;
        } else {
            funTracerTail->next = malloc(sizeof(FunTracer));
            funTracerTail = funTracerTail->next;
        }
        funTracerTail->dst = _this->dnpc;
        funTracerTail->pc = _this->pc;
        funTracerTail->type = FUN_CAL;
        funTracerTail->next = NULL;
    } else if (_this->isa.inst.val == 0x00008067) {
        if (funTracer == NULL) {
            funTracer = malloc(sizeof(FunTracer));
            funTracerTail = funTracer;
        } else {
            funTracerTail->next = malloc(sizeof(FunTracer));
            funTracerTail = funTracerTail->next;
        }
        funTracerTail->dst = _this->dnpc;
        funTracerTail->pc = _this->pc;
        funTracerTail->type = FUN_RET;
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

void function_trace_display(){
    int level = 0;
    FunTracer *node = funTracer;
    while (node) {
        printf(FMT_WORD":", node->pc);
        if (node->type == FUN_CAL) {
            for (int i = 0; i < level; i++) {putchar(' '); putchar(' ');}
            level++;
            printf("call [@"FMT_WORD"]\n", node->dst);
        } else {
            if (level > 0) level--;
            for (int i = 0; i < level; i++) {putchar(' '); putchar(' ');}
            printf("ret  [@"FMT_WORD"]\n", node->dst);

        }
        node = node->next;
    }
}

static InstTracer instTracer = {};

void trace_ins(Decode *_this) {
    strcpy(instTracer.inst[instTracer.end], _this->logbuf);
    instTracer.end = (instTracer.end + 1) % INST_TRACER_SIZE;
    if (instTracer.end == instTracer.start) instTracer.start = (instTracer.start + 1) % INST_TRACER_SIZE;
}

void ins_trace_display() {
  int i = instTracer.start;
  while (i != instTracer.end)
  {
    puts(instTracer.inst[i]);
    i = (i + 1) % INST_TRACER_SIZE;
  }
}
