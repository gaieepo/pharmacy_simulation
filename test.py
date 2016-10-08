# from pmain import test 
# from scipy import optimize

# def f(X):
# 	reg, pac, flx, che, pay = map(lambda x: int(round(x)), X)
# 	return test(11, reg, pac, flx, che, pay, 30, 540);

# result = optimize.minimize(f, (1, 1, 1, 1, 1)).x
# print result

# print test(11, 2, 3, 2, 2, 2, 30, 540)

import time
import random
import multiprocessing
from multiprocessing.dummy import Pool

size = 400
idx = range(size)

A = [[random.randint(10, 99) for j in range(size)] for i in range(size)]
B = [[random.randint(10, 99) for j in range(size)] for i in range(size)]
R1 = [[0 for j in range(size)] for i in range(size)]
R2 = [[0 for j in range(size)] for i in range(size)]

def mm():
	for i in range(size):
		for j in range(size):
			for k in range(size):
				R1[i][j] += A[i][k] * B[k][j]

def mm_mt(i):
	for j in range(size):
		for k in range(size):
			R2[i][j] += A[i][k] * B[k][j]

start = time.time()	
mm()
end = time.time()
print (end - start)


p = Pool(multiprocessing.cpu_count())
# print multiprocessing.cpu_count()

start = time.time()
p.map(mm_mt, idx)
end = time.time()
print (end - start)

print R1 == R2