_start()
{
	a();
	v("welcome to NUAActf2021\n",24,0); //will be edited in ida after compling
	v("it's too tiny to be pwned!\n",28,1);//will be edited in ida after compling
	v();
}
void a()
{
	asm(
	"mov $0x25,%rax;	mov $12,%rdi;	syscall;");
}
/*
inline void r(char* addr, int len, int fd)
{
	asm(
	"mov $0,%rax;	syscall;");
}*/
/*
inline void w(char* addr, int len, int fd)
{
	asm(
	"mov $1,%rax;	syscall;");	
}*/
int v()
{
	char a[0x20];
	v(a,0x70,0); //will be edited in ida after compiling
	v("Bye\n",5,1); //will edited in ida after compiling
	return 114514;
}
void __libc_csu_init()
{
	asm(
	"mov %rdx,%r14;	push %r13;	mov %rsi,%r13;	push %r12;	mov %edi,%r12d;	push %rbp;	push %rbx;	sub %r15,%rbp;	sub $8,%rsp;	sar $3,%rbp;	jz loc_4010f6;	xor %ebx,%ebx;	nop;	loc_4010e0:	mov %r14,%rdx;	mov %r13,%rsi;	mov %r12d,%edi;	add $1,%rbx;	add $1,%rbx;	cmp %rbx,%rbp;	jnz loc_4010e0;	loc_4010f6:	add $8,%rsp;	pop %rbx;	pop %rbp;	pop %r12;	pop %r13;	pop %r14;");
}
