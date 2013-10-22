CC = gcc
ASM = nasm
DBG = gdb
CFLAGS = -ggdb -Wall -std=c99 -pedantic -m64
ASMFLAGS = -felf64 -g -F dwarf
OPENCV_LIB = `pkg-config --cflags --libs opencv`

BIN = tp2
BIN_DIR = ../bin



OBJS = tp2.o utils.o \
	color_filter_c.o miniature_c.o decode_c.o \
	color_filter_asm.o miniature_asm.o decode_asm.o

.PHONY: all clean

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(OPENCV_LIB) -lm
	cp $(BIN) $(BIN_DIR)/$(BIN)

tp2.o: tp2.c
	$(CC) $(CFLAGS) -c -o $@ $< $(OPENCV_LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(OPTIMIZACION) -c -o $@ $< -lm

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

all: $(BIN)

clean:
	rm -f $(OBJS)
	rm -f ./$(BIN)
	rm -f $(BIN_DIR)/$(BIN)



