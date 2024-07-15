CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
TARGET = homework3
SRCS = homework3.cpp Queries_HT.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
