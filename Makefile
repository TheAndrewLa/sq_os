
src_name = hello_world.asm
binary_name = hello_world.bin

image_name = sqos.img

.PHONY: clean

all: clean debug

clean:
	@rm -rf build

build_dir:
	@mkdir -p build

build_floppy: $(src_names) | build_dir
	@nasm -fbin $(src_name) -o build/$(binary_name)
	@dd if=/dev/zero of=build/$(image_name) bs=1024 count=1440
	@dd if=build/$(binary_name) of=build/$(image_name) conv=notrunc

debug: build_floppy
	@qemu-system-i386 -fda build/$(image_name)

