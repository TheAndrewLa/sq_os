vbr_name = vbr.asm
vbr_binary_name = vbr.bin

kernel_binary_name = kernel.bin

image_name = sqos.img

.PHONY: clean

all: clean build_floppy

clean:
	@rm -rf build

build_dir:
	@mkdir -p build

build_floppy: $(vbr_name) | build_dir
	@nasm -fbin $(vbr_name) -o build/$(vbr_binary_name)

	@clang++ -std=c++20 -O3 -m32 -ffreestanding -fno-pic -fno-rtti kernel/display.cpp -c -o build/display.o
	@clang++ -std=c++20 -O3 -m32 -ffreestanding -fno-pic -fno-rtti kernel/kernel.cpp -c -o build/kernel.o
	@ld -m elf_i386 -T scripts/linker.ld build/display.o build/kernel.o -o build/$(kernel_binary_name)

	@dd if=/dev/null of=build/$(image_name) bs=1024 count=1440
	@dd if=build/$(vbr_binary_name) of=build/$(image_name) seek=0 conv=notrunc
	@dd if=build/$(kernel_binary_name) of=build/$(image_name) seek=1 conv=notrunc

debug: build_floppy
	@qemu-system-i386 -display sdl,gl=on -nic none -fda build/$(image_name)
