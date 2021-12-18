import numpy as np

with open('data.txt') as f:
    a = f.read().splitlines()
data = [eval(x) for x in a]

# data = [...]
for points in data:
    x = [_[0] for _ in points]
    y = [_[1] for _ in points]
    x = np.array(x)
    y = np.array(y)
    k, b = np.polyfit(x, y, 1)
    print(chr(int(k)), end='')
    print(chr(int(b)), end='')
# flag{9D4f8e0f-1a5B9E35-aef0fBd2bF5A}
