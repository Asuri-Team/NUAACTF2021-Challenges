# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
int balance;
int Size, number;

void *Buy_Handler(void *arg)
{
    int num = *(int *)arg;
    if (num > 0)
    {
        if (balance / 100 >= num)
        {
            Size += num;
            balance -= 100 * num;
        }
    }
}

void *Sale_Handler(void *arg)
{
    balance += Size * 90;
    sleep(1);
    Size = 0;
}

int readi()
{
    char buf[0x20];
    read(0, buf, 0x20);
    return atoi(buf);
}

void Buy()
{
    pthread_t buy_thread;
    puts("How many Jiamu's power do you want to buy?");
    number = readi();
    int result;
    int ret = pthread_create(&buy_thread, NULL, Buy_Handler, (void *)&number);
    if (ret != 0)
    {
        puts("Create thread failed");
        return;
    }
}

void Sale()
{
    pthread_t sale_thread;
    puts("Sale Jiamu's power is not a right choice");
    pthread_create(&sale_thread, NULL, Sale_Handler, NULL);
}

void Print()
{
    printf("Your balance:%d\n", balance);
    printf("Your have %d Jiamu's power\n", Size);
}

void Init()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

void Buy_Flag()
{
    if (balance >= 1001)
    {
        int fd = open("./flag", 0);
        char buf[0x30];
        read(fd, buf, 0x30);
        write(1, buf, 0x30);
        close(fd);
        balance -= 1001;
    }
    else
    {
        puts("Your don't have enough money!");
    }
}

int main()
{
    Init();
    balance = 1000;
    while (1)
    {
        puts("Welcome to Jiamu's shop");
        puts("1. Buy Jiamu's power");
        puts("2. Sale Jiamu's power");
        puts("3. Show your shopping list");
        puts("4. Buy flag");
        puts("5. exit");
        int choice = readi();
        switch (choice)
        {
        case 1:
            Buy();
            break;
        case 3:
            Print();
            break;
        case 4:
            Buy_Flag();
            break;
        case 5:
            exit(0);
        case 2:
            Sale();
            break;
        default:
            puts("Error!");
            break;
        }
    }
    return 0;
}