CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRCS = main.cpp displacement.cpp binary_conversion.cpp 
OBJS = $(SRCS:.cpp=.o)
TARGET = exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

run : $(TARGET)
	./exe
	make clean 

clean:
	$(RM) $(TARGET) $(OBJS)

