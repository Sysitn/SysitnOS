ARCH ?= x86_64

CC := clang
LD := ld.lld
OBJCOPY := llvm-objcopy

BUILD_DIR := build/$(ARCH)
OBJ_DIR := $(BUILD_DIR)/obj
LIMINE_DIR := build/limine
INCLUDE_DIR := include

TARGET := $(BUILD_DIR)/sysitnos-$(ARCH).elf
ISO := build/sysitnos-$(ARCH).iso

LIMINE_EXE := $(LIMINE_DIR)/limine
LIMINE_BRANCH := v11.x-binary
LIMINE_EFI_x86_64      := BOOTX64.EFI
LIMINE_EFI_loongarch64 := BOOTLOONGARCH64.EFI

ifeq ($(ARCH), x86_64)
	ARCH_CFLAGS := -target x86_64-unknown-none -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -masm=intel -mcmodel=kernel
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

ifeq ($(ARCH), loongarch64)
	ARCH_CFLAGS := -target loongarch64-unknown-none -march=loongarch64 -mabi=lp64d
	ARCH_LDFLAGS := -m elf64loongarch
endif

CFLAGS := -ffreestanding -Wall -Wextra -Wmissing-braces -fno-stack-protector -fno-stack-check -fno-lto -fno-pic -fno-pie -I$(INCLUDE_DIR) $(ARCH_CFLAGS)
LDFLAGS := $(ARCH_LDFLAGS) -T arch/$(ARCH)/linker.ld -nostdlib -static --no-dynamic-linker -z text -z max-page-size=0x1000

SOURCES_C := $(shell find kernel/ arch/$(ARCH) drivers/ -name '*.c')
SOURCES_S := $(shell find kernel/ arch/$(ARCH) drivers/ -name '*.S')
OBJECTS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES_C)) $(patsubst %.S, $(OBJ_DIR)/%.o, $(SOURCES_S))

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

$(LIMINE_EXE):
	@mkdir -p $(LIMINE_DIR)
	git clone https://github.com/Limine-Bootloader/Limine.git --branch=$(LIMINE_BRANCH) --depth=1 $(LIMINE_DIR)
	$(MAKE) -C $(LIMINE_DIR)

iso: iso-$(ARCH)

iso-%: build/$(ARCH)/sysitnos-$(ARCH).elf $(LIMINE_EXE)
	@mkdir -p build/$*/iso_root/boot/limine
	@mkdir -p build/$*/iso_root/EFI/BOOT

	cp build/$*/sysitnos-$(ARCH).elf build/$*/iso_root/boot/
	cp $(LIMINE_DIR)/BOOTIA32.EFI build/$*/iso_root/EFI/BOOT/
	cp $(LIMINE_DIR)/$(LIMINE_EFI_$*) build/$*/iso_root/EFI/BOOT/
	cp $(LIMINE_DIR)/limine-uefi-cd.bin build/$*/iso_root/boot/limine

	@echo "timeout: 3" > build/$*/iso_root/limine.conf
	@echo "remember_last_entry: yes" >> build/$*/iso_root/limine.conf
	@echo "graphics: yes" >> build/$*/iso_root/limine.conf
	@echo "" >> build/$*/iso_root/limine.conf
	@echo "/SysitnOS ($*)" >> build/$*/iso_root/limine.conf
	@echo "    protocol: limine" >> build/$*/iso_root/limine.conf
	@echo "    path: boot():/boot/sysitnos-$*.elf" >> build/$*/iso_root/limine.conf

	@if [ "$*" = "x86_64" ]; then \
		cp $(LIMINE_DIR)/limine-bios*.bin $(LIMINE_DIR)/limine-bios.sys build/$*/iso_root/boot/limine ; \
		xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
			-no-emul-boot -boot-load-size 4 -boot-info-table \
			--efi-boot boot/limine/limine-uefi-cd.bin \
			-efi-boot-part --efi-boot-image --protective-msdos-label \
			build/$*/iso_root -o build/sysitnos-$*.iso 2>/dev/null ; \
		$(LIMINE_EXE) bios-install build/sysitnos-$*.iso ; \
	else \
		xorriso -as mkisofs -e EFI/BOOT/$(LIMINE_EFI_$*) \
			-no-emul-boot \
			build/$*/iso_root -o build/sysitnos-$*.iso; \
	fi

bin: bin-$(ARCH)

bin-%: build/$(ARCH)/sysitnos-$(ARCH).elf
	$(OBJCOPY) -O binary build/$(ARCH)/sysitnos-$(ARCH).elf build/sysitnos-$(ARCH).bin

clean:
	@rm -rf build/aarch64 build/loongarch64 build/riscv64 build/x86_64 build/sysitnos-*.iso build/sysitnos-*.bin
	@echo "Clean Success"

distclean: 
	@rm -rf build

debug:
	@echo "SOURCES C: $(SOURCES_C)"
	@echo "SOURCES S: $(SOURCES_S)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "BUILD_DIR: $(BUILD_DIR)"

.PHONY: all clean debug run iso distclean