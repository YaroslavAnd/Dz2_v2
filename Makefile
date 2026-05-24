CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic -pthread
TARGET = build/hw2_parallel_pi
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET) --quick

full: all
	./$(TARGET) --full

clean:
	rm -rf build
