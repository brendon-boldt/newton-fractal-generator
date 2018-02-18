import numpy as np
from PIL import Image

ITERS = 50
LIMIT = 1e10
SCALE = 2.0
SIZE = (500, 500)



def f(z):
	return z * z + (-0.8 + 0.156j)

def iterate(z, f):
	for i in range(ITERS):
		z = f(z)
		if np.abs(z) > LIMIT:
			n = i;
			v = 1+n - np.log(np.log(np.abs(z))/np.log(LIMIT))/np.log(2);
			return v / ITERS;
	return -1.0

def main():
	arr = np.full((500,500,3), 255, dtype=np.uint8)
	for j in range(SIZE[1]):
		for i in range(SIZE[0]):
			z = SCALE * ((i/SIZE[0] - 0.5) + (0.5 - j/SIZE[1])*1.0j)
			'''
			if (i + j) % 100 == 0:
				print(z)
			'''
			x = iterate(z, f)
			if x < 0.0:
				arr[j,i,2] = 0
			else:
				arr[j,i,0] = x * 255
	im = Image.fromarray(arr, mode='HSV')
	im.show()


main()
