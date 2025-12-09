CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++23 -Ofast
LIBS = -lm -lgtest
INCLUDES = -I include

SRCF := ./src
OUT := $(SRCF)/build
TARGET = raytrace


SRC := $(shell find $(SRCF) -name '*.cpp')
OBJ := $(patsubst $(SRCF)/%.cpp,$(OUT)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LIBS) $(INCLUDES)


$(OUT)/%.o: $(SRCF)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT) $(TARGET)
