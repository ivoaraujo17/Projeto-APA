CXX = g++
CXXFLAGS = -Wall -Wextra -g3
TARGET = output/main.exe
SRCS = main.cpp read_arquivo.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = read_arquivo.h

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
