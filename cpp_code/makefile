CC = g++
CFLAGS = -std=c++17 -g
TARGET = ec
SOURCES = Polynomial.cpp RationalComplex.cpp test.cpp Complex.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
