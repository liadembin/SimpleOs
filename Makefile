all: run

kernel.bin: kernel_entry.o kernel.o isr.bin
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel_entry.o: kernel_entry.asm
	nasm $< -f elf -o $@

kernel.o: kernel.c
	gcc -m32 -g -ffreestanding  -fno-pic -c  $< -o $@

main.bin: main.asm
	nasm $< -f bin -D SECTOR_COUNT_CONST=43 -o $@ 
isr.bin: isr.asm
	nasm $< -f elf -o $@

os-image.bin: main.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -no-reboot -no-shutdown \
		-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
		-drive file=$<,format=raw,index=0,if=floppy \
		-boot a \
		-vga std \
		-m 256M 
				# -device i8042 \
		# -k il
clean:
	$(RM) *.bin *.o *.dis
