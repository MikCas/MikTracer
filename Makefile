# Compilers 
CC=gcc
CUDACC=nvcc

# Flags for optimization and libs
CCFLAGS=-std=c++17 -O3 -Wall 
CUFLAGS=-O3 
DEBUGFLAGS=-std=c++17 -Wall -Wextra -g -DDEBUG
LIBS=-lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
RENDER_DIR = renders

# Executables
TARGET = miktracer

# Cluster configuration
GPU_RUN = prun -np 1 -native '-C gpunode,TitanX'

# Source files and object files
CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
CU_SOURCES = $(wildcard $(SRC_DIR)/*.cu)
CPP_OBJECTS = $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
CU_OBJECTS = $(CU_SOURCES:$(SRC_DIR)/%.cu=$(BUILD_DIR)/%.o)
OBJECTS =  $(CPP_OBJECTS) $(CU_OBJECTS)
RENDERS = $(wildcard $(RENDER_DIR)/*.ppm)

# === BUILD RULES ===
# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create renders directory
$(RENDER_DIR):
	mkdir -p $(RENDER_DIR)

# Compile C++ source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Compile CUDA source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cu | $(BUILD_DIR)
	$(CUDACC) $(CUFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# === MAIN TARGETS ===
.: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CUDACC) $(CUFLAGS) -o $@ $(OBJECTS) 

# Compile in debug mode
debug:
	make DEBUG="-DDEBUG -g" FLAGS= all

# === UTILITY TARGETS ===
rungpu: $(TARGET)
	$(GPU_RUN) ./$(TARGET) $(W) $(H) $(F) $(B)


output: $(TARGET) | $(RENDER_DIR)
	$(GPU_RUN) ./$(TARGET) 256 256 "output" 

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -f $(RENDER_DIR)/*.ppm

