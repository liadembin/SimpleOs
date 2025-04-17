BUILD_DIR = dist
GCC = gcc
NASM = nasm
LD = ld
OBJCOPY = objcopy

# Common compiler flags for all C files
GCCFLAGS = -m32 -Os -ffreestanding -fno-pic -fomit-frame-pointer \
           -nostdlib -ffunction-sections -fdata-sections \
           -fno-ident -fno-asynchronous-unwind-tables

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

all: run

C_SOURCES = kernel.c ports.c stdlib.c vga.c pci.c interrupts.c paging.c network.c rtl.c
C_OBJECTS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

ASM_OBJECTS = $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/isr.bin $(BUILD_DIR)/enable_paging.bin

$(BUILD_DIR)/kernel.bin: $(ASM_OBJECTS) $(C_OBJECTS)
	$(LD) -m elf_i386 -T linker_save.ld -Ttext 0x1000 --gc-sections -o $@ $^ --oformat binary

$(BUILD_DIR)/%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel_entry.o: asm/kernel_entry.asm
	$(NASM) $< -f elf -o $@

$(BUILD_DIR)/isr.bin: asm/isr.asm
	$(NASM) $< -f elf -o $@

$(BUILD_DIR)/enable_paging.bin: asm/enable_paging.asm
	$(NASM) $< -f elf -o $@

$(BUILD_DIR)/main.bin: asm/main.asm
	$(NASM) $< -f bin -D SECTOR_COUNT_CONST=94 -o $@

$(BUILD_DIR)/os-image.bin: $(BUILD_DIR)/main.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

run: $(BUILD_DIR)/os-image.bin
	sudo qemu-system-i386 \
		-no-reboot -no-shutdown \
		-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
		-drive file=$<,format=raw,index=0,if=floppy \
		-netdev tap,id=n1,ifname=tap0,script=no,downscript=no \
		-device rtl8139,mac=52:54:00:12:34:33,netdev=n1 \
		-object filter-dump,id=f1,netdev=n1,file=netlog.pcap \
		-boot a \
		-vga std \
		-m 256M \
		-serial stdio

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

