CC = gcc

CFLAGS = -Wall -Wextra -O1 -fno-stack-protector

elf: md5.c main.c
	@$(CC) $(CFLAGS) -s -o bin/elf md5.c main.c

clean:
	@$(RM) bin/elf