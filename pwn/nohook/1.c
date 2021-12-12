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
long long freehk = &printf + 1613080;
long long mallochk = &printf + 1600864;
int flag = 1;

void initial()
{
	setvbuf(stdin,0,2,0);
	setvbuf(stdout,0,2,0);
	setvbuf(stderr,0,2,0);
}
void menu()
{
	puts("1.add");
	puts("2.show");
	puts("3.delete");
	puts("4.edit");
	puts("5.exit");
}
long long get_num()
{
	char buf[0x20];
	read(0,buf,0x18);
	return atoll(buf);
}
void add()
{
	unsigned long long size;
	void* ptr;
	int i;
	for(i=0;i<32;++i)
	{
		if(!pindex[i])
			break;
	}
	puts("size:");
	size = get_num();
	pindex[i] = malloc(size+1);
	psize[i] = size;	
}
void show()
{
	unsigned int idx;
	if(!flag)
		return;
	flag = 0;
	puts("id:");
	idx = get_num();
	if(idx>31 || !pindex[idx])
		return;
	write(1,pindex[idx],psize[idx]);
}
void edit()
{
	unsigned int idx;
	puts("id:");
	idx = get_num();
	if(idx>31 || !pindex[idx])
		return;
	read(0,pindex[idx],psize[idx]);
	idx = 0;
	idx = 1;
	if(*(long long*)freehk!=0||*(long long*)mallochk!=0)
	{
		*(long long*)freehk=0;
		*(long long*)mallochk=0;
	}
}
void dele()
{
	int idx;
	puts("id:");
	idx = get_num();
	if(idx>31||!pindex[idx])
		return;
	free(pindex[idx]);
}
int main()
{
	int choice;
	initial();
	while(1)
	{
		menu();
		choice = get_num();
		if(choice==1)
			add();
		if(choice==2)
			show();
		if(choice==3)
			dele();
		if(choice==4)
			edit();
		if(choice>5||choice<=0)
			puts("no");
	}
	
}
