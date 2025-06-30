#TODO: Add support for debugging and profiling

# === CONFIGURATION ===
# Compilers and flags
CC=gcc 
CCFLAGS=-std=c++17 -O3 -Wall 
CUDACC=nvcc 
CUFLAGS=-O3 
LIBS=-lm 

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
UTILS_DIR = utils
RENDER_DIR = renders

# User settings
TARGET = miktracer 
GPU_RUN = prun -np 1 -native '-C gpunode,TitanX' 

# === FILES ===
# Source files and object files
CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp) 
CPP_UTILS = $(wildcard $(UTILS_DIR)/*.cpp)
CU_SOURCES = $(wildcard $(SRC_DIR)/*.cu)
CPP_OBJECTS = $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(CPP_UTILS:$(UTILS_DIR)/%.cpp=$(BUILD_DIR)/%.o)
CU_OBJECTS = $(CU_SOURCES:$(SRC_DIR)/%.cu=$(BUILD_DIR)/%.o)
OBJECTS = $(CPP_OBJECTS) $(CU_OBJECTS)
RENDERS = $(wildcard $(RENDER_DIR)/*.ppm)

# === DIRECTORY TARGETS ====
# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create renders directory
$(RENDER_DIR):
	mkdir -p $(RENDER_DIR)

# === COMPILATION TARGETS ===
# .cpp -> .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/%.o: $(UTILS_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# .cu -> .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cu | $(BUILD_DIR)
	$(CUDACC) $(CUFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

all: $(TARGET)

# Create executables
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CUDACC) $(CUFLAGS) -o $@ $(OBJECTS) 

# === UTILITY TARGETS ===
# Run the program on a GPU node with default parameters
run: $(TARGET) | $(RENDER_DIR)
	$(GPU_RUN) ./$(TARGET) 256 256 "output" 8 

# Run the program on a GPU node with user parameters
# W = width, H = height, F = filename, B = blocksize
rungpu: $(TARGET) | $(RENDER_DIR)
	$(GPU_RUN) ./$(TARGET) $(W) $(H) $(F) $(B)

# Clean objects and render files 
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -f $(RENDER_DIR)/*.ppm
 
debug:
	@echo "SRC_DIR: $(SRC_DIR)"
	@echo "CPP_SOURCES: $(CPP_SOURCES)"
	@echo "CU_SOURCES: $(CU_SOURCES)"
	@echo "CPP_OBJECTS: $(CPP_OBJECTS)"
	@echo "CU_OBJECTS: $(CU_OBJECTS)"
	@echo "OBJECTS: $(OBJECTS)"
	@ls -la $(SRC_DIR)/

help:
	@echo "Makefile commands:"
	@echo "  make all          - Build the project"
	@echo "  make run        - Run the program with default parameters"
	@echo "  make rungpu     - Run the program with user parameters"
	@echo "  make clean      - Clean build files and renders"
	@echo "  make help       - Show this help message"

.PHONY: all run rungpu clean debug help