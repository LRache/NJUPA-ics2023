#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    const char *filepath = "/home/rache/Documents/ics2023/hello.txt";
    int fd = open(filepath, 0);
    char buffer[64];
    size_t nbytes = read(fd, buffer, 63);
    buffer[nbytes] = 0;
    printf("%s\n", buffer);
    return 0;
}
