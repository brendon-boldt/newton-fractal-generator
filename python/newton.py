import numpy as np
from PIL import Image

ITERS = 50
LIMIT = 1e10
SCALE = 10.0
SIZE = (200, 200)
STEP = 1.0
DELTA = 1e-1
COEFFS = [1, 0, 0, -1]

def f(z):
	return z**3.0 - 1.0

def fprime(z):
	return 3 * (z**2.0)

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

			if np.abs(x - roots[0]) < DELTA:
				imArr[j,i,0] = 0
			elif np.abs(x - roots[1]) < DELTA:
				imArr[j,i,0] = 80
			elif np.abs(x - roots[2]) < DELTA:
				imArr[j,i,0] = 160
			else:
				imArr[j,i,2] = 0
	im = Image.fromarray(imArr, mode='HSV')
	im.show()


main()
