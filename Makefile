.PHONY: o1 o2 o3 debug clean

CXX=g++
CXXFLAGS=-lpthread -lX11 -std=c++14 -fopenmp
DEPS = src/function.h src/config.h
OBJ = bin/function.o bin/generator.o bin/config.o
EXECUTABLE = generator.out
O_LEVEL = 

make:		## Build executable with no flags
make: $(EXECUTABLE)

help:		## Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

bin/%.o: src/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:		## Remove files from build process
clean:
	$(RM) $(EXECUTABLE) bin/*
	
o1:		## Build with O1 optimization. \
		## Run `make clean` when switching optimziation levels
o1: CXXFLAGS+=-O1
o1: $(EXECUTABLE)

o2:		## Build with O2 optimization.
o2: CXXFLAGS+=-O2
o2: $(EXECUTABLE)

o3:		## Build with O3 optimization.
o3: CXXFLAGS+=-O3
o3: $(EXECUTABLE)

