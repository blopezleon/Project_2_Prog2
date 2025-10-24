
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra


SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))


TARGET = project2.out

all: directories $(TARGET)

directories:
	mkdir -p obj
	mkdir -p output

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj
	rm -f $(TARGET)
	rm -f output/*.tga

.PHONY: all clean directories