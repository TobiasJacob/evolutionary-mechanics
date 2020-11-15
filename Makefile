CC=g++
CFLAG= -Wall -I.. -I. -O3 -std=c++17 -DDEBUG

SRC = src/Program src/Field src/Matrix src/Equation src/PerformanceEvaluator
OBJBUILD = $(patsubst src/%,build/%.o, $(SRC))

build/program: $(OBJBUILD)
	$(CC) -o build/program $(OBJBUILD)

build/%.o: src/%.cpp build
	$(CC) $(CFLAG) -c -o $@ $<

build:
	mkdir build

.PHONY: clean

clean:
	rm -rf build