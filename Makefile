# Project settings
PROJECT_NAME = main.exe
BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = $(BUILD_DIR)/obj

# Compiler settings
CXX = clang++
CC = clang
CXXFLAGS = -std=c++23 -Wall -Wextra -g
CFLAGS = -Wall -Wextra -g
LDFLAGS = -Wl,/NODEFAULTLIB:libcmt

# Include directories
INCLUDES = -I./include \
           -I./vendor/glfw/include \
           -I./vendor/glm/include \
           -I./vendor/stb/include \
           -I./vendor/glad/include

# Library directories and libraries
LIBDIRS = -L./vendor/glfw/lib \
          -L./vendor/glm/lib \
          -L./vendor/glad/lib

LIBS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 -lmsvcrt -lucrt

# Find all .cpp files using wildcard (supports up to 3 levels deep)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard $(SRC_DIR)/*/*.cpp) \
          $(wildcard $(SRC_DIR)/*/*/*.cpp) \
          $(wildcard include/*.cpp) \
          $(wildcard include/*/*.cpp) \
          $(wildcard include/*/*/*.cpp)

# Convert sources to objects, preserving relative directory structure
define src_to_obj
$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(subst /,_,$(subst \,_,$(1))))
endef

OBJECTS = $(foreach src,$(SOURCES),$(call src_to_obj,$(notdir $(src))))

# GLAD source
GLAD_SRC = vendor/glad/src/glad.c
GLAD_OBJ = $(OBJ_DIR)/glad.o

# Target executable
TARGET = $(BUILD_DIR)/$(PROJECT_NAME)

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS) $(GLAD_OBJ) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(GLAD_OBJ) -o $@ $(LIBDIRS) $(LIBS)

# Create specific rules for each source file found
define create_obj_rule
$(OBJ_DIR)/$(subst /,_,$(subst \,_,$(notdir $(1:.cpp=.o)))): $(1) | $(OBJ_DIR)
	@echo Compiling $$<...
	@if not exist "$$(@D)" mkdir "$$(@D)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $$< -o $$@
endef

$(foreach src,$(SOURCES),$(eval $(call create_obj_rule,$(src))))

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
	@if exist "$(BUILD_DIR)" powershell -Command "Get-ChildItem -Path '$(BUILD_DIR)' -Exclude 'gamedata' | Remove-Item -Recurse -Force"

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
