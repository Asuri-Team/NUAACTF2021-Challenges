# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>


int main() {
    char s[8];
    char *buf;
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    int fd = open("./flag", 0);
    buf = (char*)malloc(0x30);
    read(fd, buf, 0x30);
    close(fd);
    read(0, s, 0x8); 
    printf(s);
    free(buf);
    return 0; 
}