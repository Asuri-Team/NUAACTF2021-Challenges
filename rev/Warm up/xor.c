#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <string.h>

unsigned char xor_arr[] = "qasxcytgsasxcvrefghnrfghnjedfgbhn\0";
unsigned char ans[] = {0x56, 0x4e, 0x57, 0x58, 0x51, 0x51, 0x9, 0x46, 0x17, 0x46, 0x54, 0x5a, 0x59, 0x59, 0x1f, 0x48, 0x32, 0x5b, 0x6b, 0x7c, 0x75, 0x6e, 0x7e, 0x6e, 0x2f, 0x77, 0x4f, 0x7a, 0x71, 0x43, 0x2b, 0x26, 0x89, 0xfe, '\0'};

void __attribute__((constructor)) change_global_arr()
{
    volatile char *ptr = xor_arr;
    for (int i = 0; i < 34; ++i)
    {
        ptr[i] ^= i * 2 + 'A';
    }
}

void __attribute__((constructor)) check_gdb()
{
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0)
    {
        while(1);
    }
}

void check_at_exit()
{
    if (strcmp(xor_arr, ans) == 0)
    {
        printf("Congratulations!\n");
    }
    else
    {
        printf("Wrong!\n");
    }
}

int main()
{
    atexit(check_at_exit);
    char flag[35];
    fgets(flag, 35, stdin);
    for (int i = 0; i < 34; ++i)
    {
        xor_arr[i] ^= flag[i];
    }
    return 0;
}

// flag{c0n5truct0r5_functi0n_in_41f}

// gcc -z noexecstack -z now -fstack-protector-all -s -o xor xor.c