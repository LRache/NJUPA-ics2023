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

#include "sdb.h"

#define NR_WP 32


static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp() {
  assert(free_ != NULL);
  
  WP *wp = free_;
  free_ = free_->next;
  if (head == NULL) {
    head = wp;
  } else {
    WP *node = head;
    while (node->next)
    {
      node = node->next;
    }
    node->next = wp;
  }
  return wp;
}

void free_wp(WP *wp) {
  if (free_ == wp) {
    free_ = wp;
  } else {
    WP *node = free_;
    while (node->next)
    {
      node = node->next;
    }
    node->next = wp;
  }
  if (wp == head) {
    head = head->next;
  } else {
    WP *node = head;
    WP *next = head->next;
    while (next) {
      node = next;
      next = node->next;
      if (next == wp) {
        node->next = wp->next;
      }
    }
  }
}

void watchopint_display() {
  WP *node = head;
  while (node)
  {
    printf("%d %s\n", node->NO, node->expr);
    node = node->next;
  }
}
