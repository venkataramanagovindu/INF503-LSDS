CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
TARGET = homework5
SRCS = homework5.cpp PrefixTrie.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
