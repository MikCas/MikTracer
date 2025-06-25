# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
DEBUG_FLAGS = -std=c++17 -Wall -Wextra -g -DDEBUG

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
RENDER_DIR = renders
TARGET = miktracer # Executable name

# Source files and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
RENDERS = $(wildcard $(RENDER_DIR)/*.ppm)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	
# Compile source files to object files (cpp -> .o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Create the executable - link all object files
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

##### UTILITY TARGETS
# Default target
all: $(TARGET)

# Remove build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Build and Run the program
run: $(TARGET)
	./$(TARGET)

# Build with debug flags
debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: $(TARGET)

render: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o program 
	./program 256 256 "test"

cleanrender:
	rm -f program
	rm -f $(RENDER_DIR)/*.ppm
	
# Install dependencies (if needed)
install-deps:
	@echo "No external dependencies required for this project"

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  debug      - Build with debug flags"
	@echo "  clean      - Remove build files and executable"
	@echo "  run        - Build and run the program"
	@echo "  help       - Show this help message"

# Declare phony targets
.PHONY: all debug clean run quick install-deps help
