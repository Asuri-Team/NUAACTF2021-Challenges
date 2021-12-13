from random import randint
from secret import flag
import time

def gen_parameter(para):
    a = randint(1,para)
    b = randint(1,para)
    if a < b:
        a, b = b, a
    return a, b

print("Let's play a fun game!")
print("Do you know 'ax-by' means what?")
print("Just try to find the answer!")
print("Are you ready?")

res = [1, 1, 1, randint(2,100), randint(2,100)]
for i in range(5):
    print(f"Level {i+1}")
    a, b = gen_parameter(1<<10)
    print(f"{a}*x-{b}*y={res[i]}")
    print("Please give me x and y in one second!")
    start = time.time()
    x = int(input())
    y = int(input())
    end = time.time()
    if a*x - b*y == res[i]:
        if end - start >= 1:
            print("Sorry, You are too slow!")
            exit(0)
        else:
            print("That's right!")
    else:
        print("Worng!")
        exit(0)

print("Congratulations to you! Here is your flag:")
print(flag)