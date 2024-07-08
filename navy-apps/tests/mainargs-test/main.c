#include <stdio.h>

int main(int argc, char **argv, char **envp) {
    printf("argc=%d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    printf("ENVP:\n");
    char **e = envp;
    while (e) {
        printf("%s\n", *e);
        e++;
    }
    return 0;
}