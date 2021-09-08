AS = nasm
CC = gcc
LD = ld

CCFLAGS     = -ffreestanding -m64 -O2 -c -fno-stack-protector
LDFLAGS     = -Ttext 0x8000 --oformat=binary -shared -fno-PIE
C_SOURCES   = $(wildcard kernel/*.c kernel/sys/*.c kernel/sys/GDT/*.c kernel/sys/IDT/*.c kernel/memory/management/physical/*.c kernel/sys/PIC/*.c kernel/drivers/keyboard/PS2/*.c)
ASM_SOURCES = $(wildcard kernel/*.asm kernel/sys/*.asm kernel/sys/GDT/*.asm kernel/sys/IDT/*.asm)

SMP    = 8
MEMORY = 1024M


BINARY      = boot/bootloader.bin boot/bootloaderpart2.bin kernel/kernel.bin
#BINARY      = boot/bootloader.bin kernel/kernel.bin

OBJ         = ${ASM_SOURCES:.asm=.o}
OBJ        += ${C_SOURCES:.c=.o}

BOOTLOADER  = boot/bootloader.asm

OUTFILE     = dist/myOS.img

main: $(BINARY)
	@mkdir -p dist
	@\cat $^ > $(OUTFILE) 
	@rm $^

boot/bootloader.bin:
	@$(AS) -f bin boot/bootloader.asm -o $@

boot/bootloaderpart2.bin:
	@$(AS) -f bin boot/bootloaderpart2.asm -o $@

kernel/kernel.bin: $(OBJ)
	@$(LD) $(LDFLAGS) $^ -o $@
	@rm $^

%.o: %.c
	@$(CC) $(CCFLAGS) $< -o $@

%.o: %.asm
#	@$(AS) -f elf64 $< -o $@
	@$(AS) -f elf64 $< -o $@


run:
	@qemu-system-x86_64 -soundhw pcspk -drive file=$(OUTFILE),format=raw,media=disk -smp $(SMP) -m $(MEMORY)
 
runusb:
	@qemu-system-x86_64 -soundhw pcspk -drive file=$(OUTFILE),format=raw,media=disk -device nec-usb-xhci -device usb-kbd -smp $(SMP) -m $(MEMORY)

clean:
	@rm -r dist
	
tbtosp:
	@find ./kernel -type f -not -path '*/\.*' -exec sed -i 's/\t/  /g' {} +
	@find ./boot   -type f -not -path '*/\.*' -exec sed -i 's/\t/  /g' {} +



