#include <stdio.h>

int main(int argc, char **argv, char **envp) {
    printf("argc=%d\n", argc);
    printf("args:\n");
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    printf("envp:\n");
    char **e = envp;
    while (e) {
        printf("%s\n", *e);
        e++;
    }
    return 0;
}