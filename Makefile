CXX = g++
CXXFLAGS = -Wall -I.. -I. -O3 -std=c++17 -DDEBUG

# SRC are the object files, that are included by tests and by the program. You do not need a ending
SRC = src/Field.cpp src/Matrix.cpp src/Equation.cpp src/PerformanceEvaluator.cpp
# Those are the test files. Note that they provide a custom main function
TESTS = src/test/testing-test.cpp
# patsubst maps the source files to their object files
OBJSRC = $(patsubst %.cpp,build/%.o, $(SRC))
OBJTESTS = $(patsubst %.cpp,build/%.o, $(TESTS))

.PHONY: program
program: $(OBJSRC) build/src/Program.o # Program provides the main function, OBJSRC all other object files
	$(CXX) $(CXXFLAGS) -o build/program $(OBJSRC) build/src/Program.o

.PHONY: test
test: $(OBJTESTS) $(OBJSRC) # Link all test and source objects
	$(CXX) $(CXXFLAGS) -o build/test $(OBJTESTS) $(OBJSRC)

# General rule to build all object files
build/src/%.o: src/%.cpp | build/src/test # Use "order-only prerequisites" when depending on directories
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
