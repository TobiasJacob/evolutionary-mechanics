CXX=g++
CXXFLAGS= -Wall -I.. -I. -O3 -std=c++17 -DDEBUG

SRC = src/Program src/Field src/Matrix src/Equation src/PerformanceEvaluator
OBJBUILD = $(patsubst src/%,build/%.o, $(SRC))

test-all: test-example other-tests

test-example: src/test/test-filename.o tested-class.cpp
	$(CXX) $(CXXFLAGS) -o src/test/$@ $^
	test/$@ --success

testing-test: src/test/testing-test.o 
	$(CXX) $(CXXFLAGS) -o src/test/$@ $^
	test/$@ --success

build/program: $(OBJBUILD)
	$(CXX) -o build/program $(OBJBUILD)

build/%.o: src/%.cpp build
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build:
	mkdir build

.PHONY: clean

clean:
	rm -rf build
