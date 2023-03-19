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

f = open("testcase.txt", "w")

for i in range(200):
    f.write(str(rand.randint(1, n_20)) + " ")

for i in range(100):
    f.write(str(rand.randint(n_20+1, n_40)) + " ")

for i in range(200):
    f.write(str(rand.randint(n_40+1, n_50)) + " ")

for i in range(100):
    f.write(str(rand.randint(n_50+1, n_60)) + " ")

for i in range(200):
    f.write(str(rand.randint(1, n_40)) + " ")

for i in range(200):
    f.write(str(rand.randint(1, n)) + " ")

f.close()
