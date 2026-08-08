# Aрхитектура
ARCH ?= x86_64

# Компилятор и линкер
CC := clang
LD := ld.lld

BUILD_DIR := build/$(ARCH)
OBJ_DIR := $(BUILD_DIR)/obj
INCLUDE_DIR := include

ifeq ($(ARCH), x86_64)
	ARCH_CFLAGS := -target x86_64-unknown-none -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel
	ARCH_LDFLAGS := -m elf_x86_64
endif

ifeq ($(ARCH), aarch64)
	ARCH_CFLAGS := -target aarch64-unknown-none -mcpu=cortex-a72 -mgeneral-regs-only
	ARCH_LDFLAGS := -m aarch64elf
endif

ifeq ($(ARCH), riscv64)
	ARCH_CFLAGS := -target riscv64-unknown-none -march=rv64imac -mabi=lp64 -mcmodel=medany
	ARCH_LDFLAGS := -m elf64lriscv
endif

#Флаги компиляции 
CFLAGS := -ffreestanding -Wall -Wextra -Wmissing-braces -fno-stack-protector -fno-stack-check -fno-lto -fno-pic -fno-pie -I$(INCLUDE_DIR) $(ARCH_CFLAGS)
LDFLAGS := $(ARCH_LDFLAGS) -T arch/$(ARCH)/linker.ld -nostdlib -static --no-dynamic-linker -z text -z max-page-size=0x1000

SOURCES_C := $(shell find kernel/ arch/$(ARCH) drivers/ -name '*.c')
SOURCES_S := $(shell find kernel/ arch/$(ARCH) drivers/ -name '*.S')
OBJECTS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES_C)) $(patsubst %.S, $(OBJ_DIR)/%.o, $(SOURCES_S))

TARGET := $(BUILD_DIR)/sysitnos-$(ARCH).elf
ISO := $(BUILD_DIR)/sysitnos.iso

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(TARGET)
	

run: iso
	qemu-system-x86_64 -serial stdio -cdrom $(BUILD_DIR)/sysitnos.iso

clean:
	@rm -rf build
	@echo "Clean Success"

debug:
	@echo "SOURCES C: $(SOURCES_C)"
	@echo "SOURCES S: $(SOURCES_S)"
	@echo "OBJECTS: $(OBJECTS)"

.PHONY: all clean debug run