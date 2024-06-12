/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#ifndef __CPU_CPU_H__
#define __CPU_CPU_H__

#include <common.h>

#define INST_BUFFER_SIZE 32
typedef struct InstBuffer {
  char inst[INST_BUFFER_SIZE][128];
  int start;
  int end;
} InstBuffer;

enum CALL_TRACE_TYPE {
    FUN_CAL, FUN_RET
};

typedef struct CallLinkNode
{
    word_t pc;
    vaddr_t dst;
    struct CallLinkNode *next;
    int type; 
} CallTracer;

void cpu_exec(uint64_t n);
void ins_trace_display();
void call_trace_display();

void set_nemu_state(int state, vaddr_t pc, int halt_ret);
void invalid_inst(vaddr_t thispc);

#define NEMUTRAP(thispc, code) set_nemu_state(NEMU_END, thispc, code)
#define INV(thispc) invalid_inst(thispc)

#endif
