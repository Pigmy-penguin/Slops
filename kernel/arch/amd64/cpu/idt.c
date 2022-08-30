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
#include <arch/amd64/cpu/idt.h>
#define MODULE_NAME "idt"

__attribute__((aligned(0x10))) 
static struct interrupt_desc idt[256];
static struct idtr_struct idtr;
extern isr_t __interrupt_vector[];

void idt_set_descriptor(u8 vector, void* isr, u8 flags) {
   struct interrupt_desc* descriptor = &idt[vector];

   descriptor->offset_0 = (u64)isr & 0xFFFF;
   descriptor->selector = 0x08;
   descriptor->ist = 0x0;
   descriptor->type_attributes = flags;
   descriptor->offset_1 = ((u64)isr >> 16) & 0xFFFF;
   descriptor->offset_2 = ((u64)isr >> 32) & 0xFFFFFFFF;
   descriptor->zero = 0;
}

void idt_load() {
   idtr.addr = (u64)&idt[0];
   idtr.size = (u16)sizeof(idt) - 1;

   for (u8 vector = 0; vector < 128; vector++) {
      idt_set_descriptor(vector, (void*)__interrupt_vector[vector], 0x8e);
   }
   idt_flush((u64) & idtr);

   pr_info("Created 128 IDT entries");
   pr_info("IDT properly loaded");
   pr_info("Loaded IDT base: %x", idtr.addr);
   pr_info("Loaded IDT size: %x", idtr.size);
   return;
}

void interrupts_handler(struct registers *regs);
// This gets called from our ASM interrupt handler stub.
void interrupts_handler(struct registers *regs)
{
   pr_info("Interrupt no %d triggered", regs->int_no);
}
