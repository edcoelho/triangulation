SRC_DIR := src/
BUILD_DIR := build/

CXXFLAGS := -pedantic-errors -Wall -I$(SRC_DIR)
LDLIBS := -lm

# Default main file.
MAIN := $(addprefix $(SRC_DIR), main.cpp)
# .o file corresponding to the main file.
MAIN_OBJ := $(patsubst %.cpp, %, $(notdir $(MAIN)))
MAIN_OBJ := $(addprefix $(BUILD_DIR), $(MAIN_OBJ)).o

# List of .cpp files
CPP_FILES := $(shell find $(SRC_DIR) -name "*.cpp")

# List of .hpp files
HPP_FILES := $(shell find $(SRC_DIR) -name "*.hpp")

# List of .o files
OBJ_FILES := $(patsubst $(SRC_DIR)%.cpp, $(BUILD_DIR)%.o, $(CPP_FILES))
# Removes .o files that don't have a corresponding .hpp file.
OBJ_FILES := $(filter $(patsubst $(SRC_DIR)%.hpp, $(BUILD_DIR)%.o, $(HPP_FILES)), $(OBJ_FILES))

# List of headers without a corresponding implementation file.
HEADERSONLY := $(filter-out $(patsubst $(SRC_DIR)%.cpp, $(SRC_DIR)%.hpp, $(CPP_FILES)), $(HPP_FILES))

# Indicating to make which targets are not associated with actual files.
.PHONY: main clean debug

# Default target.
ALL: $(BUILD_DIR) main

# Target to generate debug executable.
debug: CXXFLAGS += -g
debug: ALL

# Linking target.
main: $(MAIN_OBJ) $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(MAIN_OBJ) -o main $(LDLIBS)

# Target to compile the main file.
$(MAIN_OBJ): $(MAIN) $(HEADERSONLY)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to compile other .cpp files
$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp $(SRC_DIR)%.hpp $(HEADERSONLY)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to create the build directory.
$(BUILD_DIR):
ifeq ($(wildcard $(BUILD_DIR)),)
    $(shell mkdir $(BUILD_DIR))
    $(shell find $(SRC_DIR) -type d -exec mkdir -p "$(BUILD_DIR){}" \;)
    $(shell mv $(BUILD_DIR)$(SRC_DIR)* $(BUILD_DIR) 2>/dev/null ; true) # "2>/dev/null" suppresses possible mv error
    $(shell rm -r $(BUILD_DIR)$(SRC_DIR))
endif

# Target to run the executable.
run:
	./main

# Target to delete object files and executable.
clean:
	rm -rf $(BUILD_DIR) *.o main
	clear