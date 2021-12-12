#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <asm/unistd.h>
#include <unistd.h>

inline ssize_t my_write(int fd, const void *buf, size_t size)
{
    ssize_t ret;
    asm volatile(
        "syscall"
        : "=a"(ret)
        //                 EDI      RSI       RDX
        : "0"(__NR_write), "D"(fd), "S"(buf), "d"(size)
        : "rcx", "r11", "memory");
    return ret;
}



#endif