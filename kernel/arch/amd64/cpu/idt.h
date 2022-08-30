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

#ifndef IDT_H
#define IDT_H
#include <kernel/types.h>

struct idtr_struct {
   u16 size;
   u64 addr;
}__attribute__((packed));

// 16 bytes long
struct interrupt_desc {
   u16 offset_0;        // offset bits 0..15
   u16 selector;        // a code segment selector in GDT or LDT
   u8  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   u8  type_attributes; // gate type, dpl, and p fields
   u16 offset_1;        // offset bits 16..31
   u32 offset_2;        // offset bits 32..63
   u32 zero;            // reserved
}__attribute((packed));

struct idt_struct {
   struct interrupt_desc entries[256];
}__attribute((packed));

struct registers {
   u64 r15;
   u64 r14;
   u64 r13;
   u64 r12;
   u64 r11;
   u64 r10;
   u64 r9;
   u64 r8;
   u64 rbp;
   u64 rdi;
   u64 rsi;
   u64 rdx;
   u64 rcx;
   u64 rbx;
   u64 rax;
   u64 int_no;
   u64 err;
   // the interrupt stackframe
   u64 rip;
   u64 cs;
   u64 rflags;
   u64 rsp;
   u64 ss;
} __attribute__((packed));

typedef void (*isr_t)(struct registers *);

void idt_flush(u64);
void idt_load(void);
// TODO: void read_idtr(struct *idtr)

#endif
