#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"
#include "global.h"

// type
typedef int (*main_t)();
typedef struct
{
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_Ehdr;

// func
void fake_main();
int comp(uint8_t *ehdr, uint8_t *flag);

// var
uint8_t to_cmp_arr[] = {0x3c, 0x24, 0x4b, 0xe5, 0x68, 0xcb, 0x54, 0x99, 0x63, 0x54, 0xe1, 0x8d, 0xee, 0xfa, 0x4a, 0x96};

int main(int argc, char *argv[])
{
label:;
    void *addr = &&label;
    void *base_addr = (void *)(((uint64_t)addr & ~(0xfff)) - 0x1000);
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)base_addr;
    uint8_t *ehdr_md5;
    ehdr_md5 = md5String((char *)ehdr, sizeof(Elf64_Ehdr));

    if (argc != 2)
    {
        puts("Usage: ./elf <flag>\n");
        free(ehdr_md5);
        return 0;
    }
    if (!comp(ehdr_md5, (uint8_t *)argv[1]))
    {
        puts("Wrong!");
    }
    else
    {
        puts("Congratulations!");
    }
    free(ehdr_md5);
    return 0;
}

void fake_main()
{
    char fake_flag[] = "Haha, flag{1234567890} is not real flag!\n";
    my_write(STDOUT_FILENO, (const void *)fake_flag, sizeof(fake_flag));
    ssize_t ret;
    asm volatile(
        "syscall"
        : "=a"(ret)
        //                 EDI
        : "0"(__NR_exit), "D"(0)
        : "memory");
    return;
}

int comp(uint8_t *ehdr, uint8_t *flag)
{
    uint8_t comp_arr[16];
    for (int i = 0; i < 16; ++i)
    {
        comp_arr[i] = ehdr[i] ^ flag[i];
    }
    for (int i = 0; i < 16; ++i)
    {
        if (comp_arr[i] != to_cmp_arr[i])
        {
            return 0;
        }
    }
    return 1;
}

// flag{kn0w_e1f_and_m4n_i5_us3ful}