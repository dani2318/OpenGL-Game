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

# Detect OS
ifeq ($(OS),Windows_NT)
	DETECTED_OS := Windows
else
	DETECTED_OS := $(shell uname -s)
endif

# Common includes
INCLUDES = -I./include \
           -I./vendor/glm/include \
           -I./vendor/stb/include \
           -I./vendor/glad/include

# Find all .cpp files using wildcard (supports up to 3 levels deep)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard $(SRC_DIR)/*/*.cpp) \
          $(wildcard $(SRC_DIR)/*/*/*.cpp) \
          $(wildcard include/*.cpp) \
          $(wildcard include/*/*.cpp) \
          $(wildcard include/*/*/*.cpp)

# GLAD source
GLAD_SRC = vendor/glad/src/glad.c
GLAD_OBJ = $(OBJ_DIR)/glad.o

# OS-specific settings
ifeq ($(DETECTED_OS),Windows)
	# Windows-specific settings
	PROJECT_NAME = OpenGL-Game.exe
	INCLUDES += -I./vendor/glfw/include
	LIBDIRS = -L./vendor/glfw/lib -L./vendor/glm/lib -L./vendor/glad/lib
	LIBS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 -lmsvcrt -lucrt
	LDFLAGS = -Wl,/NODEFAULTLIB:libcmt
	MKDIR = @if not exist "$(1)" mkdir "$(1)"
	RM = @if exist "$(1)" powershell -Command "Get-ChildItem -Path '$(1)' -Exclude 'gamedata' | Remove-Item -Recurse -Force"
else ifeq ($(DETECTED_OS),Linux)
	# Linux-specific settings
	PROJECT_NAME = OpenGL-Game
	# Use pkg-config to get GLFW flags
	GLFW_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null || echo "-I/usr/include -I/usr/local/include")
	GLFW_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null || echo "-lglfw")
	INCLUDES += $(GLFW_CFLAGS)
	LIBDIRS = -L/usr/lib/x86_64-linux-gnu -L/usr/lib -L/usr/local/lib
	LIBS = $(GLFW_LIBS) -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	LDFLAGS =
	MKDIR = @mkdir -p $(1)
	RM = @rm -rf $(1)
else ifeq ($(DETECTED_OS),Darwin)
	# macOS-specific settings
	PROJECT_NAME = OpenGL-Game
	INCLUDES += -I/usr/local/include -I/opt/homebrew/include
	LIBDIRS = -L/usr/local/lib -L/opt/homebrew/lib
	LIBS = -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LDFLAGS =
	MKDIR = @mkdir -p $(1)
	RM = @rm -rf $(1)
else
	$(error Unsupported operating system: $(DETECTED_OS))
endif

# Convert sources to objects
define src_to_obj
$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(subst /,_,$(subst \,_,$(1))))
endef

OBJECTS = $(foreach src,$(SOURCES),$(call src_to_obj,$(notdir $(src))))

# Target executable
TARGET = $(BUILD_DIR)/$(PROJECT_NAME)

# Default target
all:
	@echo Building for $(DETECTED_OS)...
	@$(MAKE) $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS) $(GLAD_OBJ) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(GLAD_OBJ) -o $@ $(LIBDIRS) $(LIBS)
	@echo Build complete: $(TARGET)

# Create specific rules for each source file found
define create_obj_rule
$(OBJ_DIR)/$(subst /,_,$(subst \,_,$(notdir $(1:.cpp=.o)))): $(1) | $(OBJ_DIR)
	@echo Compiling $$<...
	$(call MKDIR,$$(dir $$@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $$< -o $$@
endef

$(foreach src,$(SOURCES),$(eval $(call create_obj_rule,$(src))))

# Compile GLAD with C compiler
$(GLAD_OBJ): $(GLAD_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create directories
$(BUILD_DIR):
	$(call MKDIR,$(BUILD_DIR))

$(OBJ_DIR):
	$(call MKDIR,$(OBJ_DIR))

# Clean build files
clean:
	$(call RM,$(BUILD_DIR))
	@echo Clean complete

# Run the executable
run: $(TARGET)
	$(TARGET)

# Debug build
debug: CXXFLAGS += -DDEBUG -O0
debug: all

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: all

# Print variables for debugging
print-%:
	@echo $* = $($*)

print-os:
	@echo Detected OS: $(DETECTED_OS)

.PHONY: all clean run debug release print-% print-os
