BUILD_DIR = dist

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

all: run

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/ports.o $(BUILD_DIR)/stdlib.o $(BUILD_DIR)/vga.o $(BUILD_DIR)/interrupts.o $(BUILD_DIR)/pci.o $(BUILD_DIR)/isr.bin $(BUILD_DIR)/enable_paging.bin $(BUILD_DIR)/paging.o $(BUILD_DIR)/network.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

$(BUILD_DIR)/kernel_entry.o: asm/kernel_entry.asm
	nasm $< -f elf -o $@

$(BUILD_DIR)/kernel.o: kernel.c
	gcc -m32 -g -ffreestanding -fno-pic -c $< -o $@

$(BUILD_DIR)/ports.o: ports.c
	gcc -m32 -g -ffreestanding -fno-pic -c $< -o $@

$(BUILD_DIR)/stdlib.o: stdlib.c
	gcc -m32 -g -ffreestanding -fno-pic -c $< -o $@

$(BUILD_DIR)/vga.o: vga.c
	gcc -m32 -g -ffreestanding -fno-pic -c $< -o $@

$(BUILD_DIR)/pci.o: pci.c
	gcc -m32 -g -ffreestanding -fno-pic -c $< -o $@

$(BUILD_DIR)/interrupts.o: interrupts.c
	gcc -m32 -g -ffreestanding -fno-pic -c $< -o $@

$(BUILD_DIR)/paging.o: paging.c 
	gcc -m32 -g -ffreestanding -fno-pic -c $< -o $@

$(BUILD_DIR)/network.o: network.c
	gcc -m32 -g -ffreestanding -fno-pic -c $< -o $@

$(BUILD_DIR)/main.bin: asm/main.asm
	nasm $< -f bin -D SECTOR_COUNT_CONST=94 -o $@

$(BUILD_DIR)/isr.bin: asm/isr.asm
	nasm $< -f elf -o $@
$(BUILD_DIR)/enable_paging.bin: asm/enable_paging.asm
	nasm $< -f elf -o $@
$(BUILD_DIR)/os-image.bin: $(BUILD_DIR)/main.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

run: $(BUILD_DIR)/os-image.bin
	qemu-system-i386 -no-reboot -no-shutdown \
		-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
		-drive file=$<,format=raw,index=0,if=floppy \
		-boot a \
		-vga std \
		-m 256M

drive: $(BUILD_DIR)/os-image.bin
	qemu-system-i386 -no-reboot -no-shutdown \
		-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
		-drive file=$<,format=raw,index=0,if=ide \
		-boot order=c \
		-vga std \
		-m 256M \
		-monitor stdio \
		-d int,cpu_reset

clean:
	$(RM) -r $(BUILD_DIR)
