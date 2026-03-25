CXX = clang++
CXXFLAGS = -std=c++17 -O3 -I include 
SRC = $(wildcard src/*.cpp)
TARGET = miktracer

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

render: $(TARGET)
	./$(TARGET)
	mkdir -p renders
	sips -s format png image.ppm --out renders/image.png

clean:
	rm -f $(TARGET) image.ppm

.PHONY: clean render