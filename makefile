CXX = g++
CXXFLAGS = -std=c++17 -g

SRCS = ECPolynomial.cpp test.cpp
OBJS = $(SRCS:.cpp=.o)

ec: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

