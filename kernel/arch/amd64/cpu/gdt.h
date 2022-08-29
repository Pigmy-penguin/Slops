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

#ifndef GDT_H
#define GDT_H
#include <kernel/types.h>

#define GDT_ENTRY_COUNT 5

#define GDT_SEGMENT (0b00010000)
#define GDT_PRESENT (0b10000000)
#define GDT_USER (0b01100000)
#define GDT_EXECUTABLE (0b00001000)
#define GDT_READWRITE (0b00000010)

#define GDT_ACCESS_BYTE 0b1100

struct gdtr {
   u16 size;
   u64 address;
}__attribute__((packed));

struct segment_descr {
   u16 limit0;
   u16 base0;
   u8 base1;
   u8 access_byte; // AKA flags
   u8 limit1 : 4;
   u8 flags : 4;
   u8 base2;

}__attribute__((packed));

struct tss_struct
{
    u32 reserved;
    u64 rsp[3];
    u64 reserved0;
    u64 ist[7];
    u32 reserved1;
    u32 reserved2;
    u16 reserved3;
    u16 iopb_offset;
} __attribute__((packed));

struct gdt_tss_struct
{
    u16 len;
    u16 base_low;
    u8 base_mid;
    u8 flags0;
    u8 flags1;
    u8 base_high;
    u32 base_upper;
    u32 reserved;
} __attribute__((packed));

struct gdt_struct
{
    struct segment_descr entries[GDT_ENTRY_COUNT];
    struct gdt_tss_struct tss;
}__attribute__((packed));

static inline void read_gdtr(struct gdtr *dtr)
{
	asm volatile("sgdt %0":"=m" (*dtr));
}

void gdt_load(void);

void gdt_flush(u64);
void tss_flush(u64);


#endif
