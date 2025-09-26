# Project settings
PROJECT_NAME = main.exe
BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = $(BUILD_DIR)/obj

# Compiler settings
CXX = clang++
CC = clang
CXXFLAGS = -std=c++17 -Wall -Wextra -g
CFLAGS = -Wall -Wextra -g
LDFLAGS = -Wl,/NODEFAULTLIB:libcmt

# Include directories
INCLUDES = -I./include \
           -I./vendor/glfw/include \
           -I./vendor/glm/include \
           -I./vendor/glad/include

# Library directories and libraries
LIBDIRS = -L./vendor/glfw/lib \
          -L./vendor/glm/lib \
          -L./vendor/glad/lib
LIBS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 -lmsvcrt -lucrt

# Find all .cpp files in src directory
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# GLAD source (assuming it's in vendor/glad/src/ or lib/)
GLAD_SRC = vendor/glad/src/glad.c
GLAD_OBJ = $(OBJ_DIR)/glad.o

# Target executable
TARGET = $(BUILD_DIR)/$(PROJECT_NAME)

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS) $(GLAD_OBJ) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(GLAD_OBJ) -o $@ $(LIBDIRS) $(LIBS)

# Compile .cpp files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile GLAD with C compiler
$(GLAD_OBJ): $(GLAD_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create directories
$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

$(OBJ_DIR):
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"

# Clean build files
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"

# Run the executable
run: $(TARGET)
	$(TARGET)

# Debug build
debug: CXXFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: $(TARGET)

# Print variables for debugging
print-%:
	@echo $* = $($*)

.PHONY: all clean run debug release print-%