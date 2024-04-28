CXX = g++
CXXFLAGS = -Wall -Wextra -g3
TARGET = main.exe
SRCS = main.cpp read_arquivo.cpp a_guloso.cpp solucao.cpp VND.cpp
OBJS = $(patsubst %.cpp, compilado/%.o, $(SRCS))
DEPS = read_arquivo.h a_guloso.h solucao.h VND.h

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

compilado/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
