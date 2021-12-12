# include <stdio.h> 
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <seccomp.h>
int now;


typedef struct TreeNode {
    struct TreeNode *f, *ch[2]; 
    int illegal[2]; 
    char *confession;
    int val;
} node;

node *root[2]; 

void Exit() {
    free(root[0]);
    free(root[1]);
    root[0] = root[1] = NULL;
    puts("落日和晚风都不及你...");
    exit(0); 
}

int check(node *o) {
    unsigned short var1, var2; 
    for (int i = 0; i <= 1; ++i)
        if (o -> ch[i]) {
            var1 = o -> ch[i] -> f;
            var2 = o;
            if (var1 != var2)
                return 0;
        }
    return 1; 
}

int isrson(node *o, node *f) {
    return f -> ch[0] == o ? 0 : 1;
}

void Insert(node **o, char *s, int val, node *f) {
    if (!*o) {
        *o = (node*)malloc(sizeof(node));
        (*o) -> f = f; 
        (*o) -> val = val;
        (*o) -> confession = strdup(s); 
        return ;
    }
    for (int i = 0; i <= 1; ++i)
        if ((*o) -> illegal[i])
            (*o) -> ch[i] = NULL, (*o) -> illegal[i] = 0;
    if (val == (*o) -> val)
        return ;
    if (val < (*o) -> val)
        Insert(&((*o) -> ch[0]), s, val, *o);
    else
        Insert(&((*o) -> ch[1]), s, val, *o);
}

node* find(node *o, int val) {
    if (!o) return NULL;
    if (!check(o))
        return NULL;
    if (val == o -> val) return o; 
    if (val < o -> val)
        return find(o -> ch[0], val);
    else return find(o -> ch[1], val); 
}

void Link(node *o, int val, int opt, node *new_son) { // 需选手自己注意插入的new_son子树的权值要满足和o -> val的大小关系.  
    if (!o) return ;
    if (!check(o))
        return;
    if (o -> val == val) {
        if (!o -> ch[opt]) {
            o -> ch[opt] = new_son;
            new_son -> f = o;
        }
        return ;
    }
    if (val < o -> val)
        Link(o -> ch[0], val, opt, new_son);
    else 
        Link(o -> ch[1], val, opt, new_son); 
}

void Dele(node **o) {
    if (!(*o)) return ;
    if (!check(*o))
        return; 
    Dele(&(*o) -> ch[0]);
    Dele(&(*o) -> ch[1]);
    free((*o) -> confession);
    // (*o) -> confession = NULL; 
    free((*o));
    *o = NULL;
}

void DeleTree(node **o, int val) {
    if (!(*o)) return ;
    if (!check(*o))
        return ;
    if (val == (*o) -> val) {
        Dele(&(*o));
        return ; 
    }
    if (val < (*o) -> val)
        DeleTree(&(*o) -> ch[0], val);
    else 
        DeleTree(&(*o) -> ch[1], val); 
}

void Init() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_ALLOW);
	seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
	seccomp_load(ctx);
}

int readi() {
    char buf[0x20];
    read(0, buf, 0x20);
    return atoi(buf); 
}

// # define debug
# ifdef debug

void Debug(node *o) {
    if (!o) return ;
    Debug(o -> ch[0]);
    printf("S:%s\nval:%d\n", o -> confession, o -> val);
    Debug(o -> ch[1]);
}
# endif

void Menu()  {
    puts("1. 写下对你的思念.");
    puts("2. 交换彼此的思念.");
    puts("3. 读一封枫叶的书信.");
    puts("4. 扔下这片枫叶.");
    puts("5. 让我来切身体会吧.");
    puts("6. 重新书写这份思念.");
    puts("Your Choice:");
# ifdef debug
    Debug(root[now]);
    puts("--------------");
    Debug(root[now ^ 1]);
# endif
}

void Add() {
    static char s[0x1000];
    puts("写下你的思念吧.");
    read(0, s, 0xfff);
    s[0xfff] = '\x00'; 
    puts("留下这份思念的价值吧.");
    int val = readi();
    Insert(&root[now], s, val, NULL);
}

void Exchange() {
    puts("你要用哪份思念来交换.");
    int val1 = readi();
    puts("将这份思念放到哪片枫叶下面.");
    int val2 = readi();
    int opt = readi();
    node *u = find(root[now], val1);
    u -> f -> illegal[isrson(u, u -> f)] = 1;
    Link(root[now ^ 1], val2, opt == 0 ? 0 : 1, u);
}

void Read() {
    puts("你想拾起哪片枫叶.");
    int val = readi();
    node *u = find(root[now], val);
    puts(u -> confession);
}

void Drop() {
    puts("你想丢掉哪片枫叶.");
    int val = readi();
    DeleTree(&root[now], val);
}

void Edit() {
    puts("你想重写哪份思念?");
    int val = readi();
    node *u = find(root[now], val);
    if (u) {
        int len = strlen(u -> confession);
        puts("写下你的思念吧.");
        read(0, u -> confession, len);
    }
}

int main() {
    Init();
    root[0] = (node*)malloc(sizeof(node));
    root[1] = (node*)malloc(sizeof(node));
    root[0] -> val = root[1] -> val = 0x7fffffff;
    puts("栖霞山的枫叶红了, 拾起一片枫叶, 写满对你的思念.  ");
    while (1) {
        Menu();
        int choice = readi();
        switch (choice)
        {
        case 1:
            Add();
            break;

        case 2:
            Exchange();
            break; 

        case 3:
            Read(); 
            break; 

        case 4:
            Drop();
            break; 
        
        case 5:
            now ^= 1;
            break;

        case 6:
            Edit();
            break; 
        
        default:
            Exit();
        }
    }
    return 0; 
}