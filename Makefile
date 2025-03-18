# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude -pthread

# Directories
SRC_DIR = src
INCLUDE_DIR = include

# Executable name
TARGET = dining_philo  # Renamed from 'program' to 'dining_philo'

# List of source files and object files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/peterson.cpp
OBJS = $(SRCS:.cpp=.o)  # Convert .cpp to .o

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET)

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Recompile everything from scratch
rebuild: clean $(TARGET)
