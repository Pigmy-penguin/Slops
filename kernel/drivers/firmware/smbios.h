#ifndef SMBIOS_H
#define SMBIOS_H

#include <kernel/types.h>

struct smbios_entry_point_64 {
   char anchor_string[5];
   u8 entry_point_checksum;
   u8 entry_point_len;
   u8 smbios_maj_version;
   u8 smbios_min_version;
   u8 smbios_docrev; // What is this?
   u8 entry_point_revision;
   u8 reserved;
   u32 struct_table_max_size;
   u64 struct_table_addr;
}__attribute__((packed));

struct smbios_entry_point_32 {
   char anchor_string[4];
   u8 entry_point_checksum;
   u8 entry_point_len;
   u8 smbios_maj_version;
   u8 smbios_min_version;
   u16 smbios_max_struct_size;
   u8 entry_point_revision;
   u8 formatted_area[5];
   char intermediate_anchor_string[5];
   u8 intermediate_checksum;
   u16 struct_table_len;
   u32 struct_table_addr;
   u16 nb_of_structs;
   u8 bcd_revision;
}__attribute__((packed));

struct smbios_header {
   u8 type;
   u8 len;
   u16 handle;
};

// Only SMBIOS 2.0 part of the struct is implemented
struct smbios_proc_info {
   u8 type;
   u8 len;
   u16 handle;
   u8 socket_reference;
   u8 proc_type;
   u8 proc_family;
   u8 manufacturer;
   u64 proc_id;
   u8 proc_version;
   u8 voltage;
   u16 external_clock;
   u16 max_speed;
   u16 current_speed;
   u8 status;
   u8 proc_upgrade;
}__attribute__((packed));

void parse_smbios_ep32_struct(void *entry_32);
void parse_smbios_ep64_struct(void *entry_64);
struct smbios_proc_info *get_proc_info(void);

#endif
