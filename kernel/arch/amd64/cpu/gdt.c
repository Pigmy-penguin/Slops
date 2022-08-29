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

#include <kernel/console.h>
#include <arch/amd64/cpu/gdt.h>
#include <drivers/char/serial.h>
#define MODULE_NAME "gdt"

static struct gdt_struct gdt;

static struct gdtr gdt_descriptor = {
   .size = sizeof(struct gdt_struct) - 1,
   .address = (u64)&gdt,
};

static struct tss_struct tss = {
	.reserved = 0,
	.rsp = { },
	.reserved0 = 0,
	.ist = { },
	.reserved1 = 0,
	.reserved2 = 0,
	.reserved3 = 0,
	.iopb_offset = 0,
};

static struct segment_descr make_gdt_ent(u32 base, u32 limit, u8 access_byte, u8 flags) {
   struct segment_descr gdt_ent = { };
   
   gdt_ent.limit0 = (u16) ((limit) & 0xffff);
   gdt_ent.base0 = (u16) ((base) & 0xffff);
   gdt_ent.base1 = (u8) (((base) >> 16) & 0xff);
   gdt_ent.access_byte = (access_byte);
   gdt_ent.limit1 = ((limit) >> 16) & 0x0f;
   gdt_ent.flags = (flags);
   gdt_ent.base2 = (u8) (((base) >> 24) & 0xff);

   return gdt_ent;
}

static struct gdt_tss_struct make_gdt_tss(u64 tss_addr) {
   struct gdt_tss_struct gdt_tss;

   gdt_tss.len = sizeof(struct tss_struct);
   gdt_tss.base_low = tss_addr & 0xffff;
   gdt_tss.base_mid = (tss_addr >> 16) & 0xff;
   gdt_tss.flags0 = 0b10001001;
   gdt_tss.flags1 = 0;
   gdt_tss.base_high = (tss_addr >> 24) & 0xff;
   gdt_tss.base_upper = tss_addr >> 32;
   gdt_tss.reserved = 0;

   return gdt_tss;
}

void gdt_load() {
   struct gdtr current_gdtr;
   read_gdtr(&current_gdtr);
   pr_info("Current (linear) GDT address %x", current_gdtr.address);
   pr_info("Current GDT size %x", current_gdtr.size);
   
   // Null descriptor
   gdt.entries[0] = make_gdt_ent(0, 0, 0, 0);
   gdt.entries[1] = make_gdt_ent(0, 0xfffff, 0x9a, GDT_PRESENT | GDT_SEGMENT 
         | GDT_READWRITE | GDT_EXECUTABLE); // Kernel Mode Code segment
   gdt.entries[2] = make_gdt_ent(0, 0xfffff, 0x92, GDT_PRESENT | GDT_SEGMENT 
         | GDT_READWRITE); // Kernel Mode Data segment
   gdt.entries[3] = make_gdt_ent(0, 0xfffff, 0xfa, GDT_PRESENT | GDT_SEGMENT 
         | GDT_READWRITE | GDT_EXECUTABLE | GDT_USER); // User Mode Code segment
   gdt.entries[4] = make_gdt_ent(0, 0xfffff, 0xf2, GDT_PRESENT | GDT_SEGMENT 
         | GDT_READWRITE |  GDT_USER); // User Mode Data segment

   gdt.tss = make_gdt_tss((u64) & tss); // TSS segment
   pr_info("Created %d GDT entries", GDT_ENTRY_COUNT);
   gdt_flush((u64)&gdt_descriptor);
   pr_info("GDT properly loaded");
   pr_info("New GDT address %x", gdt_descriptor.address);
   pr_info("New GDT size %x", gdt_descriptor.size);
}
