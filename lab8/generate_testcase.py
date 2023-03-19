import random as rand
import sys

n = int(sys.argv[1])
n_20 = int(n * 0.2)
n_30 = int(n * 0.3)
n_40 = int(n * 0.4)
n_50 = int(n * 0.5)
n_60 = int(n * 0.6)
n_70 = int(n * 0.7)
n_80 = int(n * 0.8)
n_90 = int(n * 0.9)

data = []

f = open("testcase.txt", "w")

for i in range(8000):
    data.append(str(rand.randint(1, n_20)))

rand.shuffle(data)

for i in range(2000):
    data.append(str(rand.randint(n_20+1, n)))

rand.shuffle(data)

for i in range(len(data)):
    f.write(data[i] + " ")

f.close()
