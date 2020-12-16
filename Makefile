# -- ShadoKern Makefile -- #
kernel: kernel.o
	nasm -f elf32 kernel.asm -o kernel.o
	gcc -fno-stack-protector -m32 -c kernel.c -o kc.o
	ld -m elf_i386 -T link.ld -o kernel kernel.o kc.o

clean:
	rm -f *.o kernel

delobj:
	rm -f *.o
