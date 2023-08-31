CXX = g++
CXXFLAGS = -std=c++17 -g

SRCS = ECView.cpp
OBJS = $(SRCS:.cpp=.o)

ec: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o view
	rm -f $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) view
	