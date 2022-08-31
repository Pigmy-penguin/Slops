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

#include <kernel/panic.h>
#include <kernel/console.h>
#include <kernel/string.h>
#include <drivers/char/serial.h>
#include <arch/amd64/timers/tsc.h>

extern u64 __time_at_boot;

void hlt(void)
{
   do {
      asm volatile ("hlt");
   } while (1);
   __builtin_unreachable();
}

void panic(const char *msg, const char *file, u32 line, ...)
{
   __builtin_va_list args;
   __builtin_va_start(args, line);
   cls();
   putc('\n');

   char message[1024];
   vsnprintk(message, sizeof(message), msg, args);
   serial_print("PANIC: \"%s\" at %s:%d\n", message, file, line);
   printk("$- PANIC -~ \"%s\" at %s:%d\n\n", message, file, line);
   __builtin_va_end(args);

   void *current_RIP;
   asm( "lea 0(%%rip), %0" : "=r"(current_RIP) );  
   printk("$RIP~: %x\n\n", current_RIP);
   printk("$TIME~: (SINCE BOOT) %dMs\n", tsc_get_ms());
   hlt();
}
