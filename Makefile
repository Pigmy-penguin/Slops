ISO_IMAGE 	:= slops.iso
KERNEL 		:= kernel.elf

ASMSOURCES  := $(shell find . -not \( -path './limine' -prune \) -type f -name '*.S')
CSOURCES    := $(shell find . -not \( -path './limine' -prune \) -type f -name '*.c')
HEADER_DEPS := $(CSOURCES:.c=.d)
OBJ         := $(ASMSOURCES:.S=.o) $(CSOURCES:.c=.o)

ifneq ($(DEBUG),)
  COMMON_CFLAGS     += -g -DDEBUG -Wno-error -Wno-unused-parameter
else
  COMMON_CFLAGS     += -DNDEBUG -Os
endif


CC := clang
AS := $(CC)
LD := ld.lld

SHOW_COMMAND	:= @printf "%-15s%s\n"
SHOW_CC		:= $(SHOW_COMMAND) "[ $(CC) ]"
SHOW_LD		:= $(SHOW_COMMAND) "[ $(LD) ]"
SHOW_INSTALL	:= $(SHOW_COMMAND) "[ INSTALL ]"
SHOW_CLEAN	:= $(SHOW_COMMAND) "[ CLEAN ]"

ASFLAGS := 	-I./kernel/		\
			-m64		\
			-c		\
			-g

LDFLAGS := 	-nostdlib			\
			-zmax-page-size=0x1000 	\
			-static                	\
			--no-dynamic-linker    	\
			-ztext			\
			--oformat elf64-x86-64	\
			-m elf_x86_64		\
			-Tkernel/linker.ld	\
			kernel/drivers/video/font.o \
			-Map kernel.map

CFLAGS := 	-ffreestanding			\
			$(COMMON_CFLAGS)	\
			-fno-stack-protector 	\
			-fpie                	\
			-c 			\
			-m64 			\
			-I./kernel/ 		\
			-Wall 			\
			-Wextra 		\
			-Wstrict-prototypes 	\
			-MMD 			\
			-Werror 		\
			-fno-builtin		\
			-nostdlib		\
			-nostdinc		\
			-mno-red-zone		\
			-mno-mmx		\
			-mno-sse		\
			-mno-sse2		\
			-target x86_64-elf

ifneq ($(V),)
	SILENCE	=
else
	SILENCE = @
endif

LIMINE_DIR := limine
LIMINE := $(LIMINE_DIR)/limine-install

QEMU := qemu-system-x86_64
QEMU_FLAGS := -M q35 -m 2G -enable-kvm -cpu host -serial file:debug.txt# -s -S

TMP_ISO_ROOT := tmp_iso_root

.PHONY: all clean distclean

all: $(ISO_IMAGE)

run: $(ISO_IMAGE)
	clear
	@echo "Running Slops..."
	$(SILENCE)$(QEMU) $(QEMU_FLAGS) -cdrom $(ISO_IMAGE)

$(LIMINE_DIR):
	@echo "Downloading Limine..."
	$(SILENCE)git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1 > /dev/null 2>&1


$(LIMINE): $(LIMINE_DIR)
	@echo "Building Limine..."
	$(SILENCE)$(MAKE) -C $(LIMINE_DIR) > /dev/null 2>&1

$(KERNEL): $(OBJ)
	$(SHOW_LD) $@
	$(SILENCE)$(LD) $(LDFLAGS) $^ -o $@

-include $(HEADER_DEPS)
%.o: %.c
	$(SHOW_CC) $@
	$(SILENCE)$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(SHOW_CC) $@
	$(SILENCE)$(AS) $(ASFLAGS) $< -o $@

$(ISO_IMAGE): $(KERNEL) $(LIMINE)
	$(SHOW_INSTALL) $@
	$(SILENCE)rm -rf $(TMP_ISO_ROOT)
	$(SILENCE)mkdir -p $(TMP_ISO_ROOT)
	$(SILENCE)cp $(KERNEL) \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin $(TMP_ISO_ROOT)/
	$(SILENCE)xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(TMP_ISO_ROOT) -o $(ISO_IMAGE) > /dev/null 2>&1
	$(SILENCE)limine/limine-deploy $(ISO_IMAGE) > /dev/null 2>&1
	$(SILENCE)rm -rf $(TMP_ISO_ROOT)

clean:
	$(SHOW_CLEAN) $(KERNEL)
	$(SILENCE)-rm -f $(ISO_IMAGE) $(OBJ) $(KERNEL) $(HEADER_DEPS)

distclean: clean
	$(SHOW_CLEAN) $(LIMINE_DIR)
	$(SILENCE)-rm -rf $(LIMINE_DIR)
