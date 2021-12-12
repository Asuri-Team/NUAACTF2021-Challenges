#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>

void a()
{
	asm(
	"mov $0x25,%rax;	mov $60,%rdi;	syscall;");
}
void r(char* addr, int len)
{
	asm(
	"mov $0,%rax;	mov %rsi,%rdx;	mov %rdi,%rsi;	mov $0,%rdi;	syscall;");
}
void w(char* addr, int len)
{
	asm(
	"mov $1,%rax;	mov %rsi,%rdx;	mov %rdi,%rsi;	mov $1,%rdi;	syscall;");	
}
int v()
{
	char a[0x20];
	r(a,0x70);
	w("Bye\n",5);
	return 114514;
}
int main()
{
	a();
	w("welcome to NUAActf2021\n",24);
	w("it's too tiny to be pwned!\n",28);
	v();
}
