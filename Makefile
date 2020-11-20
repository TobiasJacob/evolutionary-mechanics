CXX = g++
CXXFLAGS = -Wall -I.. -I. -O3 -std=c++17 -DDEBUG

# SRC are the object files, that are included by tests and by the program. You do not need a ending
SRC = src/Field.cpp src/Matrix.cpp src/Equation.cpp src/PerformanceEvaluator.cpp
# Those are the test files. Note that they provide a custom main function
TESTS = src/test/testing-test.cpp
# patsubst maps the source files to their object files
OBJSRC = $(patsubst %.cpp,build/%.o, $(SRC))
OBJHEADERS = $(patsubst %.cpp,%.hpp, $(SRC))
OBJTESTS = $(patsubst %.cpp,build/%.o, $(TESTS))

all: build/test build/program

build/program: $(OBJSRC) build/src/Program.o # Program provides the main function, OBJSRC all other object files
	$(CXX) $(CXXFLAGS) -o build/program $(OBJSRC) build/src/Program.o

build/test: build/src/test/test.o $(OBJTESTS) $(OBJSRC) # Link all test and source objects
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
