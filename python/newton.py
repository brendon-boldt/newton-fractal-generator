import numpy as np
from PIL import Image

ITERS = 40
LIMIT = 1e10
SCALE = 10.0
SIZE = (200, 200)
STEP = 1.0 + 0j
DELTA = 1e0
COEFFS = [1, 0, 0, -1]

f_poly = np.poly1d(COEFFS)
f_polyder = np.polyder(COEFFS)

def polyval_dict(p, z):
	y = 0
	for k,v in p:
		pass


def polyval(p, z):
	y = 0
	for n, c in enumerate(p):
		if c == 0:
			continue
		y += c * z ** n
	return y

def f(z):
	#return z ** 3 - 1
	#return np.polyval(f_poly, z)
	#return polyval([-1, 0, 0, 1], z)
	return polyval([-1, 0, 0, 1], z)

def fprime(z):
	#return 3 * z**2
	#return np.polyval(f_polyder, z)
	#return polyval([0, 0, 3], z)
	return polyval([0, 0, 3], z)

def iterate(z):
	for i in range(ITERS):
		fpz = fprime(z)
		if fpz == 0.0:
			return z
		z = z - STEP * f(z)/fpz
		if np.abs(z) > LIMIT:
			return LIMIT
	return z

def main():
	roots = np.roots(COEFFS)
	vals = np.zeros((SIZE[0],SIZE[1]), dtype=np.float32)
	imArr = np.full((SIZE[0],SIZE[1],3), 255, dtype=np.uint8)
	for j in range(SIZE[1]):
		for i in range(SIZE[0]):
			z = SCALE * ((i/SIZE[0] - 0.5) + (0.5 - j/SIZE[1])*1.0j)
			x = iterate(z)

			diverges = True
			for idx, r in enumerate(roots):
				if np.abs(x - r) < DELTA:
					imArr[j, i, 0] = int(idx*255/len(roots))
					diverges = False

			if diverges:
				imArr[j,i,2] = 0


			'''
			if np.abs(x - roots[0]) < DELTA:
				imArr[j,i,0] = 0
			elif np.abs(x - roots[1]) < DELTA:
				imArr[j,i,0] = 80
			elif np.abs(x - roots[2]) < DELTA:
				imArr[j,i,0] = 160
			else:
				imArr[j,i,2] = 0
			'''
	im = Image.fromarray(imArr, mode='HSV')
	im.show()

import time

start = time.time()
x = 100
for i in range(200*200*1000):
	x = x * x
	x = x/x
#main()
end = time.time()
print("Run Time: %.2f" % ((end-start)))
