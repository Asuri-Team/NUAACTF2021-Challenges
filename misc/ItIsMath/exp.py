import matplotlib.pyplot as plt
import numpy

# plt.figure()
# with open('data.txt', 'r') as f:
#     # for i in range(18):
#     while True:
#         s = f.readline()
#         if not s:
#             break
#         points = numpy.array(eval(s)).T
#         plt.plot(points[0], points[1], 'x-')
#         # plt.show()

# plt.show()

# 线性回归找斜率和截距

from sklearn.linear_model import LinearRegression
import numpy as np

with open('data.txt') as f:
    a = f.read().splitlines()
a = [eval(x) for x in a]

ans = []
for i in range(len(a)):
    xx, yy = list(zip(*a[i]))

    xx = np.array(xx).reshape(-1, 1)
    yy = np.array(yy)

    linear_regression = LinearRegression(normalize=True)
    linear_regression.fit(xx, yy)
    message = linear_regression.coef_[0]
    print(message)
    ans.append(round(message))
    y = linear_regression.intercept_
    print(y)
    # print(chr(round(y)))
    ans.append(round(y))

print(bytes(ans))
# flag{9D4f8e0f-1a5B9E35-aef0fBd2bF5A}