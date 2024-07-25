CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
TARGET = homework4
SRCS = homework4.cpp Queries_NW.cpp Queries_BL.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
