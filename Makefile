CXX = g++
CXXFLAGS = -Wall -I.. -I. -O3 -std=c++17 -DDEBUG

# SRC are the object files, that are included by tests and by the program. Do not include src/Program.cpp here!
SRC = src/Field.cpp src/Matrix.cpp src/Equation.cpp src/PerformanceEvaluator.cpp src/SparseMatrix.cpp
# Those are the test files. Note that they provide a custom main function. Do not include src/test/test.cpp here!
TESTS = src/test/testing-test.cpp src/test/SparseMatrix-test.cpp src/test/Equation-test.cpp src/test/PerformanceEvaluator-test.cpp

# patsubst maps the source files to their object files
OBJSRC = $(patsubst %.cpp,build/%.o, $(SRC))
OBJHEADERS = $(patsubst %.cpp,%.hpp, $(SRC))
OBJTESTS = $(patsubst %.cpp,build/%.o, $(TESTS))

# This default target keeps everything up to date.
.PHONY: all
all: build/test build/program

# Program provides the main function, OBJSRC all other object files
build/program: $(OBJSRC) build/src/Program.o
	$(CXX) $(CXXFLAGS) -o build/program $(OBJSRC) build/src/Program.o

# Link all test and source objects
build/test: build/src/test/test.o $(OBJTESTS) $(OBJSRC)
	$(CXX) $(CXXFLAGS) -o build/test $(OBJTESTS) $(OBJSRC) build/src/test/test.o

# Special rule, so that catch has not to be rebuild when a header changes
build/src/test/test.o: src/test/test.cpp | build/src/test
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# General rule to build all object files. Important: If a single header changes, everything has to be rebuild. If a single cpp file changes, only the cpp file has to be rebuild.
build/src/%.o: src/%.cpp $(OBJHEADERS) | build/src/test # Use "order-only prerequisites" when depending on directories
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create build directory
build:
	mkdir -p build

# Create build/src directory
build/src: | build
	mkdir -p build/src

# Create build/src/test directory
build/src/test: | build/src
	mkdir -p build/src/test

# Delete build dir
.PHONY: clean
clean:
	rm -rf build
