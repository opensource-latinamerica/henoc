# Makefile for HenocUniverse

# Compiler and flags
CXX = g++
CXXFLAGS = -c -pipe -O2 -fPIC -Wall -Wextra -D_REENTRANT -DQT_NO_DEBUG

# Include paths
# Note: The Qt path is hardcoded and may need to be adjusted for your system.
INCLUDES = -I. -Iode_install/include -I/nix/store/d42sv8cdf6hn0z5vadamr5kbi3q8q5vl-qt-full-5.15.14/mkspecs/linux-g++

# Source files
SOURCES = source/henocUniverse.cpp \
          source/vector.cpp \
          source/shapes.cpp \
          source/intersection.cpp \
          source/circle-circle.cpp \
          source/quad-quad.cpp \
          source/quad-circle.cpp \
          source/terrain-all.cpp \
          source/composite-all.cpp

# Object files
OBJECTS = $(patsubst source/%.cpp, .obj/%.o, $(SOURCES))

# Target library
TARGET = libHenocUniverse.a

# Default target
all: $(TARGET)

# Rule to build the target library
$(TARGET): $(OBJECTS)
	@echo "Creating library $@"
	@rm -f $(TARGET)
	@ar cqs $(TARGET) $(OBJECTS)

# Rule to compile source files into object files
.obj/%.o: source/%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

# Clean rule
clean:
	@echo "Cleaning up..."
	@rm -f .obj/*.o
	@rm -f $(TARGET)

# Phony targets
.PHONY: all clean
