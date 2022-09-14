#ifndef _LIMINE_H
#define _LIMINE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <kernel/types.h>

/* Misc */

typedef void (*limine_entry_point)(void);
#ifdef LIMINE_NO_POINTERS
#define LIMINE_PTR(TYPE) u64
#else
#define LIMINE_PTR(TYPE) TYPE
#endif

#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

struct limine_uuid {
  u32 a;
  u16 b;
  u16 c;
  u8 d[8];
};

#define LIMINE_MEDIA_TYPE_GENERIC 0
#define LIMINE_MEDIA_TYPE_OPTICAL 1
#define LIMINE_MEDIA_TYPE_TFTP 2

struct limine_file {
  u64 revision;
  LIMINE_PTR(void *) address;
  u64 size;
  LIMINE_PTR(char *) path;
  LIMINE_PTR(char *) cmdline;
  u32 media_type;
  u32 unused;
  u32 tftp_ip;
  u32 tftp_port;
  u32 partition_index;
  u32 mbr_disk_id;
  struct limine_uuid gpt_disk_uuid;
  struct limine_uuid gpt_part_uuid;
  struct limine_uuid part_uuid;
};

/* Boot info */

#define LIMINE_BOOTLOADER_INFO_REQUEST                                         \
  { LIMINE_COMMON_MAGIC, 0xf55038d8e2a1202f, 0x279426fcf5f59740 }

struct limine_bootloader_info_response {
  u64 revision;
  LIMINE_PTR(char *) name;
  LIMINE_PTR(char *) version;
};

struct limine_bootloader_info_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_bootloader_info_response *) response;
};

/* Stack size */

#define LIMINE_STACK_SIZE_REQUEST                                              \
  { LIMINE_COMMON_MAGIC, 0x224ef0460a8e8926, 0xe1cb0fc25f46ea3d }

struct limine_stack_size_response {
  u64 revision;
};

struct limine_stack_size_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_stack_size_response *) response;
  u64 stack_size;
};

/* HHDM */

#define LIMINE_HHDM_REQUEST                                                    \
  { LIMINE_COMMON_MAGIC, 0x48dcf1cb8ad2b852, 0x63984e959a98244b }

struct limine_hhdm_response {
  u64 revision;
  u64 offset;
};

struct limine_hhdm_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_hhdm_response *) response;
};

/* Framebuffer */

#define LIMINE_FRAMEBUFFER_REQUEST                                             \
  { LIMINE_COMMON_MAGIC, 0x9d5827dcd881dd75, 0xa3148604f6fab11b }

#define LIMINE_FRAMEBUFFER_RGB 1

struct limine_framebuffer {
  LIMINE_PTR(void *) address;
  u64 width;
  u64 height;
  u64 pitch;
  u16 bpp;
  u8 memory_model;
  u8 red_mask_size;
  u8 red_mask_shift;
  u8 green_mask_size;
  u8 green_mask_shift;
  u8 blue_mask_size;
  u8 blue_mask_shift;
  u8 unused[7];
  u64 edid_size;
  LIMINE_PTR(void *) edid;
};

struct limine_framebuffer_response {
  u64 revision;
  u64 framebuffer_count;
  LIMINE_PTR(struct limine_framebuffer **) framebuffers;
};

struct limine_framebuffer_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_framebuffer_response *) response;
};

/* Terminal */

#define LIMINE_TERMINAL_REQUEST                                                \
  { LIMINE_COMMON_MAGIC, 0xc8ac59310c2b0844, 0xa68d0c7265d38878 }

#define LIMINE_TERMINAL_CB_DEC 10
#define LIMINE_TERMINAL_CB_BELL 20
#define LIMINE_TERMINAL_CB_PRIVATE_ID 30
#define LIMINE_TERMINAL_CB_STATUS_REPORT 40
#define LIMINE_TERMINAL_CB_POS_REPORT 50
#define LIMINE_TERMINAL_CB_KBD_LEDS 60
#define LIMINE_TERMINAL_CB_MODE 70
#define LIMINE_TERMINAL_CB_LINUX 80

#define LIMINE_TERMINAL_CTX_SIZE ((u64)(-1))
#define LIMINE_TERMINAL_CTX_SAVE ((u64)(-2))
#define LIMINE_TERMINAL_CTX_RESTORE ((u64)(-3))
#define LIMINE_TERMINAL_FULL_REFRESH ((u64)(-4))

struct limine_terminal;

typedef void (*limine_terminal_write)(struct limine_terminal *, const char *,
                                      u64);
typedef void (*limine_terminal_callback)(struct limine_terminal *, u64, u64,
                                         u64, u64);

struct limine_terminal {
  u64 columns;
  u64 rows;
  LIMINE_PTR(struct limine_framebuffer *) framebuffer;
};

struct limine_terminal_response {
  u64 revision;
  u64 terminal_count;
  LIMINE_PTR(struct limine_terminal **) terminals;
  LIMINE_PTR(limine_terminal_write) write;
};

struct limine_terminal_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_terminal_response *) response;
  LIMINE_PTR(limine_terminal_callback) callback;
};

/* 5-level paging */

#define LIMINE_5_LEVEL_PAGING_REQUEST                                          \
  { LIMINE_COMMON_MAGIC, 0x94469551da9b3192, 0xebe5e86db7382888 }

struct limine_5_level_paging_response {
  u64 revision;
};

struct limine_5_level_paging_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_5_level_paging_response *) response;
};

/* SMP */

#define LIMINE_SMP_REQUEST                                                     \
  { LIMINE_COMMON_MAGIC, 0x95a67b819a1b857e, 0xa0b61b723b6a73e0 }

#define LIMINE_SMP_X2APIC (1 << 0)

struct limine_smp_info;

typedef void (*limine_goto_address)(struct limine_smp_info *);

struct limine_smp_info {
  u32 processor_id;
  u32 lapic_id;
  u64 reserved;
  LIMINE_PTR(limine_goto_address) goto_address;
  u64 extra_argument;
};

struct limine_smp_response {
  u64 revision;
  u32 flags;
  u32 bsp_lapic_id;
  u64 cpu_count;
  LIMINE_PTR(struct limine_smp_info **) cpus;
};

struct limine_smp_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_smp_response *) response;
  u64 flags;
};

/* Memory map */

#define LIMINE_MEMMAP_REQUEST                                                  \
  { LIMINE_COMMON_MAGIC, 0x67cf3d9d378a806f, 0xe304acdfc50c3c62 }

#define LIMINE_MEMMAP_USABLE 0
#define LIMINE_MEMMAP_RESERVED 1
#define LIMINE_MEMMAP_ACPI_RECLAIMABLE 2
#define LIMINE_MEMMAP_ACPI_NVS 3
#define LIMINE_MEMMAP_BAD_MEMORY 4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define LIMINE_MEMMAP_KERNEL_AND_MODULES 6
#define LIMINE_MEMMAP_FRAMEBUFFER 7

struct limine_memmap_entry {
  u64 base;
  u64 length;
  u64 type;
};

struct limine_memmap_response {
  u64 revision;
  u64 entry_count;
  LIMINE_PTR(struct limine_memmap_entry **) entries;
};

struct limine_memmap_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_memmap_response *) response;
};

/* Entry point */

#define LIMINE_ENTRY_POINT_REQUEST                                             \
  { LIMINE_COMMON_MAGIC, 0x13d86c035a1cd3e1, 0x2b0caa89d8f3026a }

typedef void (*limine_entry_point)(void);

struct limine_entry_point_response {
  u64 revision;
};

struct limine_entry_point_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_entry_point_response *) response;
  LIMINE_PTR(limine_entry_point) entry;
};

/* Kernel File */

#define LIMINE_KERNEL_FILE_REQUEST                                             \
  { LIMINE_COMMON_MAGIC, 0xad97e90e83f1ed67, 0x31eb5d1c5ff23b69 }

struct limine_kernel_file_response {
  u64 revision;
  LIMINE_PTR(struct limine_file *) kernel_file;
};

struct limine_kernel_file_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_kernel_file_response *) response;
};

/* Module */

#define LIMINE_MODULE_REQUEST                                                  \
  { LIMINE_COMMON_MAGIC, 0x3e7e279702be32af, 0xca1c4f3bd1280cee }

struct limine_module_response {
  u64 revision;
  u64 module_count;
  LIMINE_PTR(struct limine_file **) modules;
};

struct limine_module_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_module_response *) response;
};

/* RSDP */

#define LIMINE_RSDP_REQUEST                                                    \
  { LIMINE_COMMON_MAGIC, 0xc5e77b6b397e7b43, 0x27637845accdcf3c }

struct limine_rsdp_response {
  u64 revision;
  LIMINE_PTR(void *) address;
};

struct limine_rsdp_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_rsdp_response *) response;
};

/* SMBIOS */

#define LIMINE_SMBIOS_REQUEST                                                  \
  { LIMINE_COMMON_MAGIC, 0x9e9046f11e095391, 0xaa4a520fefbde5ee }

struct limine_smbios_response {
  u64 revision;
  LIMINE_PTR(void *) entry_32;
  LIMINE_PTR(void *) entry_64;
};

struct limine_smbios_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_smbios_response *) response;
};

/* EFI system table */

#define LIMINE_EFI_SYSTEM_TABLE_REQUEST                                        \
  { LIMINE_COMMON_MAGIC, 0x5ceba5163eaaf6d6, 0x0a6981610cf65fcc }

struct limine_efi_system_table_response {
  u64 revision;
  LIMINE_PTR(void *) address;
};

struct limine_efi_system_table_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_efi_system_table_response *) response;
};

/* Boot time */

#define LIMINE_BOOT_TIME_REQUEST                                               \
  { LIMINE_COMMON_MAGIC, 0x502746e184c088aa, 0xfbc5ec83e6327893 }

struct limine_boot_time_response {
  u64 revision;
  s64 boot_time;
};

struct limine_boot_time_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_boot_time_response *) response;
};

/* Kernel address */

#define LIMINE_KERNEL_ADDRESS_REQUEST                                          \
  { LIMINE_COMMON_MAGIC, 0x71ba76863cc55f63, 0xb2644a48c516a487 }

struct limine_kernel_address_response {
  u64 revision;
  u64 physical_base;
  u64 virtual_base;
};

struct limine_kernel_address_request {
  u64 id[4];
  u64 revision;
  LIMINE_PTR(struct limine_kernel_address_response *) response;
};

#ifdef __cplusplus
}
#endif

#endif
