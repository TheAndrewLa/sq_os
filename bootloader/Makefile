# Folders

SUBDIRECTORIES = graphics disk fs

BUILD_DIR = build
BUILD_SUBDIR = $(SUBDIRECTORIES:%=$(BUILD_DIR)/%)

RELEASE_DIR = release
RELEASE_SUBDIR = $(SUBDIRECTORIES:%=$(RELEASE_DIR)/%)

LOGS_DIR = logs
DUMPS_DIR = dumps

# Names of entries

VBR_SOURCE = vbr.asm
VBR_BINARY = vbr.bin

SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

DEBUG_BINARY = bootloader.elf

BINARY = bootloader.bin

IMAGE_NAME = sqos_bootloader.img

COMMON_COMPILE_FLAGS = -mno-sse -mno-sse2 -mno-sse3 -mno-sse4 -mno-sse4.1 -mno-sse4.2 -m32 -std=c++20 -ffreestanding -fno-pic -fno-exceptions -fno-rtti -c
RELEASE_COMPILE_FLAGS = -Os
DEBUG_COMPILE_FLAGS = -g -O0 -Wall

.PHONY: clean

all: clean bootloader_debug

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BUILD_SUBDIR)

	@rm -rf $(RELEASE_DIR)
	@rm -rf $(RELEASE_SUBDIR)

	@rm -rf $(LOGS_DIR)
	@rm -rf $(DUMPS_DIR)

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_SUBDIR)

$(VBR_BINARY): dirs
	@nasm -fbin $(VBR_SOURCE) -o $(BUILD_DIR)/$@

$(OBJECTS): $(BUILD_DIR)/%.o: %.cpp | dirs
	@clang++ $(COMMON_COMPILE_FLAGS) $(RELEASE_COMPILE_FLAGS) $*.cpp -o $(BUILD_DIR)/$*.o

$(DEBUG_BINARY): $(OBJECTS)
	@ld -m elf_i386 $(OBJECTS) -T scripts/debug_binary.ld -o $(BUILD_DIR)/$@

$(BINARY): $(OBJECTS)
	@ld -m elf_i386 $(OBJECTS) -T scripts/flat_binary.ld -o $(BUILD_DIR)/$@

$(IMAGE_NAME): $(BINARY) $(VBR_BINARY)
	@dd if=/dev/null of=$(BUILD_DIR)/$@ bs=1024 count=1440 status=none
	@dd if=$(BUILD_DIR)/$(VBR_BINARY) of=$(BUILD_DIR)/$@ conv=notrunc seek=0 status=none
	@dd if=$(BUILD_DIR)/$(BINARY) of=$(BUILD_DIR)/$@ seek=1

bootloader_release: $(IMAGE_NAME)
bootloader_debug: $(DEBUG_BINARY) $(IMAGE_NAME)

debug: clean bootloader_debug
	@qemu-system-i386 -display sdl,gl=on -nic none -S -s -fda $(BUILD_DIR)/$(IMAGE_NAME)

release: clean bootloader_release
	@qemu-system-i386 -display sdl,gl=on -nic none -fda $(BUILD_DIR)/$(IMAGE_NAME)
