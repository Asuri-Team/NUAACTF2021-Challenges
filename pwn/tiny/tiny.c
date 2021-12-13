
int main()
{
	a();
	w("welcome to NUAActf2021\n",24);
	w("it's too tiny to be pwned!\n",28);
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
	v(a,0x70,0); #will be edited in ida after compiling
	v("Bye\n",5,1); #will edited in ida after compiling
	return 114514;
}
void __libc_csu_init()
{
	asm(
	"push %r15;
	push %r14;
	mov %rdx,%r14;
	push %r13;
	mov %rsi,%r13;
	push %r12;
	mov %edi,%r12d;
	push %rbp;
	push %rbx;
	sub %r15,%rbp;
	sub $8,%rsp;
	sar $3,%rbp;
	jz label1;
	xor %ebx,%ebx;
	nop;
	label2:
	mov %r14,%rdx;
	mov %r13,%rsi;
	mov %r12d,%edi;
	call ds:[%r15+%rbx*8];
	add $1,%rbx;
	cmp %rbx,%rbp;
	jnz label2;
	label1:
	add $8,%rsp;
	pop %rbx;
	pop %rbp;
	pop %r12;
	pop %r13;
	pop %r14;
	pop %r15;
	retn;"
}