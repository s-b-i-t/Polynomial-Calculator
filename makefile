CC = g++
CFLAGS = -std=c++17 -g -Iinclude

TARGET = ec

SRC_DIR = src
SOURCES = \
	$(SRC_DIR)/Polynomial.cpp \
	$(SRC_DIR)/RationalComplex.cpp \
	$(SRC_DIR)/Complex.cpp \
	$(SRC_DIR)/test.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
	