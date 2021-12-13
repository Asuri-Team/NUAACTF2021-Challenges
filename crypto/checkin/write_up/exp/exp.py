m = r"oclzloovydvblbvnucdhqpumj"
flag = ""

for i in m:
    flag += chr(((19 * (ord(i) - 0x61 - 11)) % 26) + 0x61)

print(flag)