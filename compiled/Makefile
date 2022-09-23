CXX := nasm
LNK := ld
CFLAGS := -f elf
LDLIBS := -m elf_i386 -s
TARGET := main

SRCS := $(wildcard *.asm)
OBJS := $(patsubst %.asm,%.o,$(SRCS))

all: $(TARGET)
%.o: %.asm
	$(CXX) $(CFLAGS) $< -o $@
$(TARGET): $(OBJS)
	$(LNK) $(LDLIBS) -o $@ $^
clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean
