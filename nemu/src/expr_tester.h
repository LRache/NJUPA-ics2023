#ifndef __EXPR_TESTER_H__
#define __EXPR_TESTER_H__

#include <stdio.h>
#include <assert.h>
#include "./monitor/sdb/sdb.h"

void init_sdb();

void test_expr() {
    FILE *fp = fopen("./src/test_expr/examples.txt", "r");
    assert(fp != NULL);
    init_sdb();
    
    unsigned int answer;
    char expression[256];
    bool success;
    while (~fscanf(fp, "%255s %u", expression, &answer))
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
}

#endif

