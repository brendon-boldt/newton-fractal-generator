make:
	g++ src/*.cpp -lpthread -lX11 -std=c++14 -fopenmp -o bin/fractal.out
	
g:
	g++ newton.cpp f.cpp -lpthread -lX11 -std=c++14 -fopenmp -g

o:
	g++ newton.cpp f.cpp -lpthread -lX11 -std=c++14 -fopenmp -O1

oo:
	g++ newton.cpp f.cpp -lpthread -lX11 -std=c++14 -fopenmp -O2

ooo:
	g++ newton.cpp f.cpp -lpthread -lX11 -std=c++14 -fopenmp -O3
