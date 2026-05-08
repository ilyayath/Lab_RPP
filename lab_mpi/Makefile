CXX      = mpicxx
CXXFLAGS = -std=c++17 -O2 -Wall
TARGET   = primes_mpi
SRC      = primes_mpi.cpp

.PHONY: all clean run run-1 run-2 run-4 run-8

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	mpirun -np 4 ./$(TARGET) 2 1000000000

# Запуски для порівняння прискорення (для таблиці у звіті)
run-1: $(TARGET)
	mpirun -np 1 ./$(TARGET) 2 1000000000

run-2: $(TARGET)
	mpirun -np 2 ./$(TARGET) 2 1000000000

run-4: $(TARGET)
	mpirun -np 4 ./$(TARGET) 2 1000000000

run-8: $(TARGET)
	mpirun -np 8 ./$(TARGET) 2 1000000000

clean:
	rm -f $(TARGET)
