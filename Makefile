CXX = mpic++
CXXFLAGS = -Wall -I.. -I. -std=c++17 -O3 -fopenmp -DDEBUG

# SRC are the object files, that are included by tests and by the program. Do not include src/Program.cpp here!
SRC = src/Field.cpp src/Matrix.cpp src/Equation.cpp src/PerformanceEvaluator.cpp src/EvolutionaryOptimizator.cpp src/SparseMatrix.cpp src/Microtime.cpp src/plotting/Plotter.cpp src/VectorOperations.cpp src/Organism.cpp
# Those are the test files. Note that they provide a custom main function. Do not include src/test/test.cpp here!
TESTS = src/test/testing-test.cpp src/test/SparseMatrix-test.cpp src/test/Equation-test.cpp src/test/PerformanceEvaluator-test.cpp src/test/PerformanceEvaluatorSpeedup-test.cpp src/test/Organism-test.cpp
# Those are the MPI Test
MPITESTS = src/mpi-test/mpi-test.cpp src/mpi-test/mpi-sendRec.cpp src/mpi-test/mpi-broadCast.cpp

# patsubst maps the source files to their object files
OBJSRC = $(patsubst %.cpp,build/%.o, $(SRC))
OBJHEADERS = $(patsubst %.cpp,%.hpp, $(SRC))
OBJTESTS = $(patsubst %.cpp,build/%.o, $(TESTS))
OBJMPITESTS = $(patsubst %.cpp,build/%.o, $(MPITESTS))
EXECMPITESTS = $(patsubst src/mpi-test/%.cpp,build/mpi-test/%, $(MPITESTS))

# This default target keeps everything up to date.
.PHONY: all
all: build/test build/program mpi-tests

# Program provides the main function, OBJSRC all other object files
build/program: $(OBJSRC) build/src/Program.o Makefile
	$(CXX) $(CXXFLAGS) -o build/program $(OBJSRC) build/src/Program.o

# Link all test and source objects
build/test: build/src/test/test.o $(OBJTESTS) $(OBJSRC) Makefile
	$(CXX) $(CXXFLAGS) -o build/test $(OBJTESTS) $(OBJSRC) build/src/test/test.o

# Special rule, so that catch has not to be rebuild when a header changes
build/src/test/test.o: src/test/test.cpp Makefile | build
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# General rule to build all object files. Important: If a single header changes, everything has to be rebuild. If a single cpp file changes, only the cpp file has to be rebuild.
build/src/%.o: src/%.cpp $(OBJHEADERS) Makefile | build # Use "order-only prerequisites" when depending on directories
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Link all test and source objects
build/mpi-test/%: build/src/mpi-test/%.o $(OBJSRC) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $< $(OBJSRC)

.PHONY: mpi-tests
mpi-tests: $(EXECMPITESTS)

# Create build directory
build: Makefile
	mkdir -p build
	mkdir -p build/src
	mkdir -p build/src/plotting
	mkdir -p build/src/test
	mkdir -p build/src/mpi-test
	mkdir -p build/mpi-test

# Delete build dir
.PHONY: clean
clean:
	rm -rf build
