// #include "../sdb/sdb.h"
// #include <stdio.h>

// int main(int argc, char**argv) {
//     printf("%d", argc);
//     FILE *f = fopen(argv[0], "r");
//     if (f == NULL) {
//         printf("Cannot open file: %s", argv[1]);
//         return -1;
//     }
//     unsigned int answer;
//     char expression[256];
//     bool success;
//     while (~fscanf(f, "%s %u", expression, &answer))
//     {
//         printf("Testing %s", expression);
//         unsigned int result = expr(expression, &success);
//         if (!success) {
//             printf("Error: %s\n", expression);
//             return -1;
//         }
//         if (answer != result) {
//             printf("Incorrect answer: expr=%s result=%u answer=%u\n", expression, result, answer);
//             return -1;
//         }
//     }
    
//     return 0;
// }