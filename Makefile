# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -g -Wno-unused-parameter -I./include -I/usr/include/freetype2 -pthread -D_POSIX_C_SOURCE=200809L -std=c++20
LDFLAGS = -lglfw -lGL -lm -lrt -pthread -lfreetype

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Source files
C_SRCS = $(wildcard $(SRC_DIR)/*.c)
CPP_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
SRCS = $(C_SRCS) $(CPP_SRCS)

# Object files
C_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SRCS))
CPP_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_SRCS))
OBJS = $(C_OBJS) $(CPP_OBJS)

# Target executable
TARGET = crime_sim

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(INCLUDE_DIR)/*.h)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INCLUDE_DIR)/*.h)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
