CXX      = g++
CXXFLAGS = -std=c++17 -O2 -fopenmp -Wall
TARGET   = primes_omp
SRC      = primes_omp.cpp

.PHONY: all clean run run-1 run-2 run-4 run-8

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	OMP_NUM_THREADS=8 ./$(TARGET) 2 1000000000

# Запуски для порівняння прискорення (для таблиці у звіті)
run-1: $(TARGET)
	OMP_NUM_THREADS=1 ./$(TARGET) 2 1000000000

run-2: $(TARGET)
	OMP_NUM_THREADS=2 ./$(TARGET) 2 1000000000

run-4: $(TARGET)
	OMP_NUM_THREADS=4 ./$(TARGET) 2 1000000000

run-8: $(TARGET)
	OMP_NUM_THREADS=8 ./$(TARGET) 2 1000000000

clean:
	rm -f $(TARGET)
