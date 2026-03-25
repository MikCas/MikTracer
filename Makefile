CXX = clang++
CXXFLAGS = -std=c++17 -O3 -I include 
SRC = $(wildcard src/*.cpp)
TARGET = miktracer

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean