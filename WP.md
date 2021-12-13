[TOC]  

# Web  

# Misc

## 我们生活在南京（一）

用 Audacity 反转声音即可。

## 我们生活在南京（二）

根据提示，发现程序中被插入了高频正弦波，可以算出来频率是 1000Hz，添加一个相位相反的新正弦波抵消即可。

## 我们生活在南京（三）

根据提示，发现 `NU` 后面的字符都比特翻转了，还原即可，之后得到一个音频文件，还是根据提示可以知道国际空间站常用 PD-120 模式发送 SSTV，用相应的软件（比如 Robot36）解码即可。

# Rev  

## Legende Lof Links
其实题目里面藏了一个`LLL`在里面  

### 逆向分析
程序运行起来的时候，能够看到一些奇怪的截图  
![](./img/nazo05.png)   

分析找到这个逻辑:  
![](./img/nazo02.png)   
这些字符看起来有点奇怪，进行凯撒加密之后能够解开:
```
legends keywords
```
可以看到这边给了一串数字`6421025468326853`，这个数字可以在代码中搜到:  
![](./img/nazo04.png)   
虽然从
可以看到是一个匹配的逻辑
```cpp
char __thiscall check_value(__m128i **this, int a2, int a3)
{
  __m128i *v3; // edi
  __m128i *v4; // ecx
  unsigned int v5; // edx
  unsigned int i; // esi
  unsigned __int64 v7; // kr00_8
  int v9; // eax

  v3 = this[1];
  v4 = *this;
  v5 = 0;
  for ( i = 0; v4 != v3; v4 = (__m128i *)((char *)v4 + 24) )
  {
    v7 = __PAIR64__(_mm_cvtsi128_si32(_mm_srli_si128(*v4, 12)), _mm_cvtsi128_si32(_mm_srli_si128(*v4, 8)))
       + __PAIR64__(v5, i);
    v5 = HIDWORD(v7);
    i = v7;
  }
  if ( i == a2 && v5 == a3 )
    return 1;
  v9 = sub_402220(std::cout, "Have fun!");
  std::ostream::operator<<(v9, sub_4025C0);
  return 0;
```
从逻辑上看可以看出来，这边再进行一个类似累加的过程。（从汇编可以看出来，本质上压入的是一个长数字，但是这边将数字分成了两个不同的数字压入栈中）  
结合当前函数的外部，可以推测此题 **企图让某些地方的值加起来等于这个总数**。

简单玩一下游戏，或者直接去翻找资源文件能发现，游戏刚开始就往下走能够走到一个奇怪的地图
![](./img/nazo06.png)  
这个地图的构造是一个 **8*5大小的格子方块组成，其中每一行8个格子**。找到对应的地图文件，叫做nazo.tmx，使用工具`TiledMap`打开后看到如下图:  
![](./img/nazo00.png)  
会发现这里面的每一个对象身上都带有一个叫做`value`的值。于是可以猜测，这边的怪物身上的值会以某种形式传入到那个`check_value`函数中，于是在函数中下断点调试，跟到这一步
![](./img/nazo10.png)  
此处能够发现，这个地方
这个值分析程序主要逻辑之后，能够发现程序尝试读取这个地方的数值，并且用eax和esi分别存储高位和低位，结合逆向分析可知此时为一个大数加法运算。  

### 算法解密
结合题目提到，不小心把公钥生成脚本上传上去，可以知道这个`nazo.tmx`中包含的正是彩蛋解密用的密钥。分析脚本可知是一个密码学中的常见背包问题，可通过LLL算法计算出最短向量来进行解密。（其中题目中也有暗示这个算法）脚本本身也很容易在网上搜到对应的解密脚本，于是将对应资源文件中的数据读取出来之后，可以结合逆向分析的结果，直接写出对应的解密脚本:
```python
pubKey = [288987930484713, 340150388569723, 775832214910355, 216867890037121, 501352569419409, 318822438792492, 158678987418047, 103047289635008, 232924311836164, 825277525877592, 266171732738326, 232058805490403, 503492576572133, 361556090493722, 185759789132988, 141839477664327, 548824923286652, 13387599878646, 459796431386589, 353077318969337, 511286054037178, 57089108809875, 584998735988633, 284676495083131, 800281594291554, 371509900212370, 392227033009677, 840774125862985, 195657954859337, 509931569484802, 159469040461778, 809855051248225, 459522106994522, 605887145649518, 508449489953120, 836314895545713, 244798820584214, 467004182165356, 381450144739742, 581067701595314]

nbit = len(pubKey)
S = 6421025468326853  

print("start")
# create a large matrix of 0's (dimensions are public key length +1)
A = Matrix(ZZ,nbit+1,nbit+1)
# fill in the identity matrix
for i in range(nbit):
    A[i,i] = 1
# replace the bottom row with your public key
for i in range(nbit):
    A[i,nbit] = pubKey[i]
# last element is the encoded message
A[nbit,nbit] = -S

res = A.LLL()
resfil = open("res.txt", 'w')
# print(res)
resfil.write(str(res))

res = A.LLL()
for i in range(0, nbit + 1):
    # print solution
    M = res.row(i).list()
    flag = True
    for m in M:
        if m != 0 and m != 1:
            flag = False
            break
    if flag:
        print(i, M)
        M = ''.join(str(j) for j in M)
        # remove the last bit
        M = M[:-1]
        M = hex(int(M, 2))[2:-1]
        print(M)
```
密钥解密逻辑  
![](./img/nazo07.png)  
拿到对应的序列之后，可以直接按照对应的序列打死怪物，然后打死boss获取彩蛋。或者通过逆向，找到打死boss之后的逻辑:  
![](./img/nazo08.png)  
可以看到当前在一个程序的全局变量上读取了大段的数据，并且调用解密函数。分析特征可知是一个AES-256-cbc加密，于是最终可写出解密脚本，最终解密得到的`flag.tmx`为  
![](./img/nazo11.png)

## Warm up

一个很简单的逆向，看[源码](./rev/Warm%20up/xor.c)可知只是在 construct 过程中加了另一个异或和反调试。

## Destroyed ELF

首先修复 ELF Program Header，自己随便编译一个 elf 文件进行对照（比如 hello world，一般来说Ubuntu 20.04 和 18.04 的 gcc 都行）：

``` c
#include <stdio.h>

int main()
{
    return 0;
}
```

首先修复 magic word，修复之后打开会发现报错：

```
ELF file with PHT cannot be ET_REL
```

接下来就修复该字段为 0x3。

接着发现报错

The PHT overwrites previously loaded segments with different file contents.
Although the OS will likely succeed in loading this file,
we recommend you to analyze this file's PHT.

继续与自己编译出来的 binary 对比，发现 elf header size 也不正确，修改之。

接下来 IDA 就能正常解析了。

然后发现 start 函数只会输出错误的 flag，发现入口点被修改了，对比自己编译的binary，发现正常的位置在 0x1040 左右，而这个文件中的 start 函数位于 151F。向前追溯到 text 段开头，将 0x10e0 位置的代码 convert 并 create function 得到正确的 start 函数。接着将 header 中的 entry point 指向 0x10e0。

然而此时的 main 函数仍不正确，不过我们可以用多种方法定位 main 函数，这里找到 main 函数的地址是 0x15DF，create function 之后就可以正常逆向了。我们也可以将 start 中的main函数指向正确的位置。

接着会发现代码是先对 ELF Header 算了个哈希然后和 flag 异或，再和一个全局变量比较。动态调试获取哈希值然后和全局变量异或即可得到 flag。

注：这道题手动 patch 了几个位置，src 中提供的源码编译出来和题目不一致是正常现象。

# PWN
## format
格式化字符串
flag读入到了栈上
输入%7$s 即可读出flag
## thread
在sale_handler中存在sleep, 所以一个货品卖掉会先加钱过1s再清空数量, 买1份 卖两次就行了

## leaf
在二叉查找树的合并过程中存在UAF漏洞, 具体就是exchang中父亲的ch指针没有清空, check是short类型的, 构造堆然后orw即可

## tiny
多次调用alarm(sys_alarm)会返回剩余时间，在剩余2秒的时候调用sys_alarm，控制rax，调用sys_open

## can’t leak
利用unsortedbin残留指针申请到stdout->fileno,修改为2通过stderr流输出，泄露[‘environ’]得到栈地址，写orw
## nohook
每次edit后会检查free_hook和malloc_hook，不为空则清空，利用tcache->system，free(malloc_hook)来向malloc_hook写入system地址。或者打IO_FILE

## tanchishe
就一个输入点，不用逆向直接打，input函数会使得rop被复制到堆上，注意不破坏topsize就行

# Crypto

## checkin

1. 题目给出了加密函数，根据给出的a获取其乘法逆元。

```
(11n) mod 26 = 1
n = 19
```

2. 编写脚本解密，最后加上`{}_`即可。

```python
m = r"oclzloovydvblbvnucdhqpumj"
flag = ""

for i in m:
    flag += chr(((19 * (ord(i) - 0x61 - 11)) % 26) + 0x61)

print(flag)
```

## guessgame

```python
from pwn import * 
POST = 'ctf.asuri.club' 
HOST = '10000' 
r = remote(POST, HOST) 
for i in range(50-7): 
  r.sendline("1") 
for i in range(7):
  r.sendline("0") 
r.interactive()
```

## numbers



## easyRSA

根据题目给出的程序容易看出是RSA的共模攻击，最终exp。

```python
# coding=utf-8
import string
import gmpy
from rsa import transform,core
from Crypto.Util.number import long_to_bytes

def egcd(a, b):
    if a == 0:
        return b, 0, 1
    else:
        g, y, x = egcd(b % a, a)
        return g, x - b // a * y, y
def main():
    file1 = open("flag.enc1" ,'rb').read()
    c1 = transform.bytes2int(file1)

    file2 = open("flag.enc2" ,'rb').read()
    c2 = transform.bytes2int(file2)

    n = 0x00b0bee5e3e9e5a7e8d00b493355c618fc8c7d7d03b82e409951c182f398dee3104580e7ba70d383ae5311475656e8a964d380cb157f48c951adfa65db0b122ca40e42fa709189b719a4f0d746e2f6069baf11cebd650f14b93c977352fd13b1eea6d6e1da775502abff89d3a8b3615fd0db49b88a976bc20568489284e181f6f11e270891c8ef80017bad238e363039a458470f1749101bc29949d3a4f4038d463938851579c7525a69984f15b5667f34209b70eb261136947fa123e549dfff00601883afd936fe411e006e4e93d1a00b0fea541bbfc8c5186cb6220503a94b2413110d640c77ea54ba3220fc8f4cc6ce77151e29b3e06578c478bd1bebe04589ef9a197f6f806db8b3ecd826cad24f5324ccdec6e8fead2c2150068602c8dcdc59402ccac9424b790048ccdd9327068095efa010b7f196c74ba8c37b128f9e1411751633f78b7b9e56f71f77a1b4daad3fc54b5e7ef935d9a72fb176759765522b4bbc02e314d5c06b64d5054b7b096c601236e6ccf45b5e611c805d335dbab0c35d226cc208d8ce4736ba39a0354426fae006c7fe52d5267dcfb9c3884f51fddfdf4a9794bcfe0e1557113749e6c8ef421dba263aff68739ce00ed80fd0022ef92d3488f76deb62bdef7bea6026f22a1d25aa2a92d124414a8021fe0c174b9803e6bb5fad75e186a946a17280770f1243f4387446ccceb2222a965cc30b3929
    e1 = 17
    e2 = 65537
    s = egcd(e1, e2)
    s1 = s[1]
    s2 = s[2]

    if s1 < 0:
        s1 = -s1
        c1 = gmpy.invert(c1, n)
    elif s2 < 0:
        s2 = -s2
        c2 = gmpy.invert(c2, n)

    m = pow(c1, s1, n) * pow(c2, s2, n) % n
    print(str(long_to_bytes(m)))

if __name__ == '__main__':
    main()

#flag{number_is_very great}

```

## MLWE

Kyber是一个格密码算法，基于Module-LWE问题，是目前NIST标准热门候选算法。

因为尚未制定为标准，因此没有公开的test vectors，按照算法团队提供的参考实现就行。

所以直接给了算法的参考实现，密文ct和私钥sk，都是base64编码了一下。

所以decode到memory之后直接调用`indcpa_dec`即可。

源码链接：https://github.com/pq-crystals/kyber

题目hint：Kyber768 CPA PKE

flag{b138c574d2039fc2}

exp在附件中
