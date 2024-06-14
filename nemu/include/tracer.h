#ifndef __CPU_TRACER_H__
#define __CPU_TRACER_H__

#include "common.h"
#include "cpu/decode.h"

typedef struct SymTableEntry{
  vaddr_t start;
  vaddr_t end;
  char name[12];
  struct SymTableEntry *next;
} SymTableEntry;

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

void add_sym_table_entry(vaddr_t addr, int size, char *name);
void free_sym_table();

void trace_function(Decode *_this);
void free_function_tracer();
void function_trace_display();

void trace_ins(Decode *_this);
void ins_trace_display();

#endif