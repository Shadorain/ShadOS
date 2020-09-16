# Assemble boot.s file
as --32 boot.S -o boot.o

# Compile kernel.c file
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
gcc -m32 -c utils.c -o utils.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
gcc -m32 -c gui.c -o gui.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
gcc -m32 -c ttt.c -o ttt.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

# Linking kernel with kernel.o and boot.o files
ld -m elf_i386 -T linker.ld boot.o kernel.o utils.o chars.o gui.o ttt.o -o ShadOS.bin -nostdlib

# Check ShadOS.bin file is x86 multiboot file
grub-file --is-x86-multiboot ShadOS.bin

# Building the ISO file
# Required for grub to load menutry, when option selected then load ShadOS.bin file
mkdir -p isodir/boot/grub
cp ShadOS.bin isodir/boot/ShadOS.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o ShadOS.iso isodir

# Run in qemu
qemu-system-x86_64 -cdrom ShadOS.iso
