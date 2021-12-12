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