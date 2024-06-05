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

#include <common.h>
#include "./monitor/sdb/sdb.h"

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

void init_sdb();

void test_expr() {
    FILE *fp = fopen("./src/test_expr/examples.txt", "r");
    assert(fp != NULL);
    init_sdb();
    
    unsigned int answer;
    char expression[256];
    bool success;
    while (fscanf(fp, "%255s %u", expression, &answer))
    {
        printf("%s %u\n", expression, answer);
        unsigned int result = expr(expression, &success);
        if (!success) {
            printf("success=false: expr=%s\n", expression);
        }
        if (result != answer) {
            printf("Result is incorrect: expr=%s, res=%u, ans=%u\n", expression, result, answer);
        }
    }
    fclose(fp);
    printf("PASSED\n");
}

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  engine_start();
  test_expr();

  return is_exit_status_bad();
}
