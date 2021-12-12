# include <stdio.h> 
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
//# include <linux/seccomp.h>
# include <seccomp.h>
# include <malloc.h>
# include <time.h>


void* pindex[32];
int psize[32];


void check()
{
	if(__malloc_hook!=NULL || __free_hook!=NULL)
	{
		puts("don't touch my hook, it's dangerous");
		__malloc_hook = NULL;
		__free_hook = NULL;
	}
}

void input(void* ptr, unsigned int size)
{
	unsigned int i;
	char buf;
	*(char*)(ptr + size - 1) = 0;
	
	for(i=0;i<(unsigned int)(size-1);++i)
	{
		read(0,&buf,1 );
		if(buf=='\n')
			return;
		*(char*)(ptr+i) = buf;
	}
	check();

}
int input_num()
{
	char buf[8];
	read(0,buf,8);
	return atoi(buf);
}

int menu()
{
	puts("1.Add");
	puts("2.Edit");
	puts("3.Show");
	puts("4.Delete");
	printf(">>");
	return input_num();
}

void initial()
{
	scmp_filter_ctx ctx;	
	
	setvbuf(stdin,0,2,0);
	setvbuf(stdout,0,2,0);
	setvbuf(stderr,0,2,0);
	
	srand(time(0));
	malloc( (rand()%16)*10 + 0x10 );
	
  	
	ctx = seccomp_init(SCMP_ACT_ALLOW);
	seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
	seccomp_load(ctx);	
}

void add()
{
	unsigned int size;
	unsigned int idx;
	void* ptr;
	size = input_num();
	idx = input_num();
	if(size>0x60)
	{
		puts("too big");
		return;
	}
	if(idx>=32)
	{
		puts("out of range");
		return;
	}
	check();
	ptr = malloc(size);
	input(ptr,size);
	pindex[idx] = ptr;
	psize[idx] = size;
}

void edit()
{
	unsigned int idx;
	idx = input_num();
	if(idx>=32)
		return;
	input(pindex[idx],psize[idx]);
}



void show()
{
	unsigned int idx;
	idx = input_num();
	if(idx>=32)
		return;
	puts((char*)(pindex[idx]));
}

void dele()
{
	unsigned int idx;
	idx = input_num();
	if(idx>=32)
		return;
	check();
	free(pindex[idx]);
	pindex[idx]=0;
	psize[idx]=0;
}

int main()
{
	int gift = (int)(&printf);
	int choice;
	
	initial();
	
	gift = gift & 0xffff;
	printf("a gift from Asuri: %x\n", gift);
	
	puts("in return for the gift, you can't get any output on your screen");
	puts("how can you leak my info this time");
	close(1);
	

	while(1)
	{
		choice = menu();
		switch(choice)
		{
			case 1: add();break;
			case 2: edit();break;
			case 3: show();break;
			case 4: dele();break;
			case 5: system("/bin/sh");break;
			default: puts("invalid");break;
		}
	}
}
