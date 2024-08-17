CXX = g++
CXXFLAGS = -std=c++17 -g

SRCS = Polynomial.cpp test.cpp
OBJS = $(SRCS:.cpp=.o)

ec: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o test
	rm -f $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) test