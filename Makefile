CXX := gcc -m32
CFLAGS := -g -ansi -pedantic-errors -ftime-report
TARGET := main

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ 
clean:
	rm -rf $(TARGET) *.o
cleanO:
	rm -rf main.o
	
.PHONY: all clean
