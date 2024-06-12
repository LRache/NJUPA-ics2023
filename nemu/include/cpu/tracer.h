#ifndef __CPU_TRACER_H__
#define __CPU_TRACER_H__

#include "common.h"
#include "decode.h"

enum CALL_TRACE_TYPE {
    FUN_CAL, FUN_RET
};

typedef struct CallLinkNode
{
    word_t pc;
    vaddr_t dst;
    struct CallLinkNode *next;
    int type; 
} FunTracer;

#define INST_TRACER_SIZE 32

typedef struct InstBuffer {
  char inst[INST_TRACER_SIZE][128];
  int start;
  int end;
} InstTracer;

void trace_function(Decode *_this);
void free_function_tracer();
void function_trace_display();
void trace_ins(Decode *_this);
void ins_trace_display();

#endif