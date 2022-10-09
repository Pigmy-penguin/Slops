/*
   Copyright 2022 Pigmy-penguin

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

           http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <drivers/char/serial.h>
#include <drivers/firmware/smbios.h>
#include <kernel/console.h>

#define MODULE_NAME "smbios"

static struct smbios_header *first_hdr;

static int smbios_table_len(struct smbios_header *hd) {
  int i;
  const char *strtab = (char *)hd + hd->len;
  // Scan until we find a double zero byte
  for (i = 1; strtab[i - 1] != '\0' || strtab[i] != '\0'; i++)
    ;
  return hd->len + i + 1;
}

static char *check_type(u8 type) {
  switch (type) {
  case 0:
    return "BIOS Information\x00";
  case 1:
    return "System Information\x00";
  case 2:
    return "Mainboard Information\x00";
  case 3:
    return "Enclosure/Chasis Information\x00";
  case 4:
    return "Processor Information\x00";
  case 7:
    return "Cache Information\x00";
  case 9:
    return "System Slots Information\x00";
  case 16:
    return "Physical Memory Array\x00";
  case 17:
    return "Memory Device Information\x00";
  case 19:
    return "Memory Array Mapped Address\x00";
  case 20:
    return "Memory Device Mapped Address\x00";
  case 32:
    return "System Boot Information\x00";
  default:
    return "SMBIOS structure not supported";
  }
}

struct smbios_proc_info *get_proc_info() {
  struct smbios_header *hdr = first_hdr;
  for (int i = 0; i <= 11; i++) {
    pr_info("[header %d : %s]", i, check_type(hdr->type));
    if (hdr->type == 4) {
      pr_info("Found processor information structure");
      return (struct smbios_proc_info *)((void *)hdr +
                                         sizeof(struct smbios_header));
    }
    hdr = (struct smbios_header *)((void *)hdr + smbios_table_len(hdr));
  }
  return NULL;
}

void parse_smbios_ep32_struct(void *entry_32) {
  pr_info("Parsing 32 bits SMBIOS Entry point structure");
  pr_info("Entry point: %x", entry_32);
  struct smbios_entry_point_32 *smbios_ep =
      (struct smbios_entry_point_32 *)entry_32;

  pr_info("SMBIOS %d.%d parsed", smbios_ep->smbios_maj_version,
          smbios_ep->smbios_min_version);
  pr_info("Structure table physical address at: %x",
          smbios_ep->struct_table_addr);

  void *struct_table_virt_addr =
      (void *)(0xffff800000000000 + smbios_ep->struct_table_addr);
  first_hdr = (struct smbios_header *)struct_table_virt_addr;
}

void parse_smbios_ep64_struct(void *entry_64) {
  pr_info("Parsing 64 bits SMBIOS Entry point structure");
  pr_info("Entry point: %x", entry_64);
  struct smbios_entry_point_64 *smbios_ep =
      (struct smbios_entry_point_64 *)entry_64;

  pr_info("SMBIOS %d.%d parsed", smbios_ep->smbios_maj_version,
          smbios_ep->smbios_min_version);
  pr_info("Structure table physical address at: %x",
          smbios_ep->struct_table_addr);

  void *struct_table_virt_addr =
      (void *)(0xffff800000000000 + smbios_ep->struct_table_addr);
  first_hdr = (struct smbios_header *)struct_table_virt_addr;
}
