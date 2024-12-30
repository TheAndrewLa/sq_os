# Folders

SUBDIRECTORIES = interrupts vga memory memory/page_tables memory/allocators scheduler

BUILD_DIR = _build
BUILD_SUBDIR = $(SUBDIRECTORIES:%=$(BUILD_DIR)/%)

RELEASE_DIR = _release
RELEASE_SUBDIR = $(SUBDIRECTORIES:%=$(RELEASE_DIR)/%)

# Names of entries

VBR_SOURCE = asm_sources/vbr.asm
VBR_BINARY = vbr.bin

SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

ASM_SOURCES = $(wildcard asm_sources/*.asm)
ASSEMBLED_OBJECTS = $(ASM_SOURCES:asm_sources/%.asm=$(BUILD_DIR)/%.o)

DEBUG_BINARY = image.elf
BINARY = image.bin

IMAGE_NAME = sqos.img

COMMON_COMPILE_FLAGS = -mno-sse -mno-sse2 -mno-sse3 -mno-sse4 -mno-sse4.1 -mno-sse4.2 -m32 -std=c++20 -ffreestanding -fno-pic -fno-exceptions -fno-rtti -c
RELEASE_COMPILE_FLAGS = -Os -Wall
DEBUG_COMPILE_FLAGS = -g -Os -Wall -Werror -Wno-return-type

.PHONY: clean

all: clean bootloader_debug

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BUILD_SUBDIR)

	@rm -rf $(RELEASE_DIR)
	@rm -rf $(RELEASE_SUBDIR)

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_SUBDIR)

$(VBR_BINARY): dirs
	@nasm -fbin $(VBR_SOURCE) -o $(BUILD_DIR)/$@

$(OBJECTS): $(BUILD_DIR)/%.o: %.cpp | dirs
	@clang++ $(COMMON_COMPILE_FLAGS) $(RELEASE_COMPILE_FLAGS) $*.cpp -o $(BUILD_DIR)/$*.o

$(ASSEMBLED_OBJECTS): $(BUILD_DIR)/%.o: asm_sources/%.asm
	@nasm -felf32 asm_sources/$*.asm -o $(BUILD_DIR)/$*.o

$(DEBUG_BINARY): $(OBJECTS) $(ASSEMBLED_OBJECTS)
	@ld -m elf_i386 $(OBJECTS) $(ASSEMBLED_OBJECTS) -T _scripts/debug_binary.ld -o $(BUILD_DIR)/$@

$(BINARY): $(OBJECTS) $(ASSEMBLED_OBJECTS)
	@ld -m elf_i386 $(OBJECTS) $(ASSEMBLED_OBJECTS) -T _scripts/flat_binary.ld -o $(BUILD_DIR)/$@

$(IMAGE_NAME): $(BINARY) $(VBR_BINARY)
	@nasm -fbin applications/first.asm -o $(BUILD_DIR)/first_app.bin
	@nasm -fbin applications/second.asm -o $(BUILD_DIR)/second_app.bin
	@nasm -fbin applications/third.asm -o $(BUILD_DIR)/third_app.bin

	@dd if=/dev/zero of=$(BUILD_DIR)/$@ bs=1024 count=1440 status=none
	@dd if=$(BUILD_DIR)/$(VBR_BINARY) of=$(BUILD_DIR)/$@ conv=notrunc status=none
	@dd if=$(BUILD_DIR)/$(BINARY) of=$(BUILD_DIR)/$@ conv=notrunc seek=1

	@dd if=$(BUILD_DIR)/first_app.bin of=$(BUILD_DIR)/$@ conv=notrunc seek=120
	@dd if=$(BUILD_DIR)/second_app.bin of=$(BUILD_DIR)/$@ conv=notrunc seek=140
	@dd if=$(BUILD_DIR)/third_app.bin of=$(BUILD_DIR)/$@ conv=notrunc seek=160

#	@nasm -fbin applications/empty_90kb.asm -o $(BUILD_DIR)/sample.bin

#	@dd if=/dev/zero of=$(BUILD_DIR)/$@ bs=1024 count=1440 status=none
#	@dd if=$(BUILD_DIR)/$(VBR_BINARY) of=$(BUILD_DIR)/$@ conv=notrunc status=none
#	@dd if=$(BUILD_DIR)/sample.bin of=$(BUILD_DIR)/$@ conv=notrunc seek=1

bootloader_release: $(IMAGE_NAME)
bootloader_debug: $(DEBUG_BINARY) $(IMAGE_NAME)

debug: clean bootloader_debug
	@qemu-system-i386 -display sdl,gl=on -nic none -S -s -drive file=$(BUILD_DIR)/$(IMAGE_NAME),if=floppy,format=raw

release: clean bootloader_release
	@qemu-system-i386 -display sdl,gl=on -nic none -drive file=$(BUILD_DIR)/$(IMAGE_NAME),if=floppy,format=raw
