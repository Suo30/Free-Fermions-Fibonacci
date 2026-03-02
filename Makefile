# --- PROJECT CONFIGURATION ---
CXX      := g++
# Flags: Optimized (-O3), Warnings (-Wall), Debug info (-g), Include paths (-I)
CXXFLAGS := -pedantic -Wall -g -O3 -DDEBUG -I. -I include

# Libraries: Math (BLAS/LAPACK) + Graphics (X11/Imlib2)
LIBS     := -llapack -lblas -lm -lX11 -lImlib2

# Paths
SRC_DIR  := src
INC_DIR  := include
BIN_DIR  := bin
TARGET   := $(BIN_DIR)/simulation

# --- PROFESSOR'S LIBRARY ---
# The list of specific library modules to build
HVBS     := $(INC_DIR)/Matrix.o $(INC_DIR)/Text.o $(INC_DIR)/Common.o \
            $(INC_DIR)/MatrixC.o $(INC_DIR)/EX.o $(INC_DIR)/Image.o \
            $(INC_DIR)/Postscript.o $(INC_DIR)/Optimize.o $(INC_DIR)/Graph.o \
            $(INC_DIR)/Manybody.o $(INC_DIR)/Calculus.o

# --- BUILD RULES ---

# 1. Default Rule: Build the simulation
all: $(TARGET)

# 2. Link Step: Combines your main.o and the library objects into the executable
$(TARGET): $(SRC_DIR)/main.o $(HVBS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	@echo "Build Complete! Run with: ./$(TARGET)"

# 3. Compile Step for YOUR code (.cpp)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 4. Compile Step for LIBRARY code (.cc)
# This handles the professor's files inside include/
$(INC_DIR)/%.o: $(INC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 5. Cleanup: Deletes the executable and all temporary .o files
clean:
	rm -f $(BIN_DIR)/* $(SRC_DIR)/*.o $(INC_DIR)/*.o