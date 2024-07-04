#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    const char *filepath = "/home/rache/Documents/ics2023/hello.txt";
    FILE *f = fopen(filepath, "r");
    char buffer[64];
    size_t nbytes = fread(buffer, 1, 63, f);
    buffer[nbytes] = 0;
    printf("%s\n", buffer);
    return 0;
}
