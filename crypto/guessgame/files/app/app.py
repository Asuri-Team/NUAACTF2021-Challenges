from secret import flag
import random

BITS = 100
num = random.randint(0,1 << BITS)
print("[~] Guess Dawn_whisper's number! [~]")

SCORE = 0
for _ in range(50):
    print("[+] Please input a number: ",end='')

    try:
        guess = int(input())
    except:
        print("[!] ERROR!")
        continue

    if guess == num:
        SCORE += 1
    else:
        print("[-] Sorry, please try again~")

    if SCORE >= 7:
        print("[~] Congratulations!")
        print("[-] Here's your flag!")
        print("[-]",flag)

    for i in range(BITS):
        if (1<<i) & num:
            mask = guess << i
            while mask:
                num = (num ^ (mask & ((1<<BITS)-1)))
                mask >>= BITS
            break
