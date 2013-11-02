all: kernel.asm
	nasm -fbin kernel.asm -o kernel.bin
	mcopy -o -i diskette.img  kernel.bin ::/


