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

void panic(const char *msg, const char *file, u32 line)
{
   cls();
   putc('\n');
   serial_print("PANIC: \"%s\" at %s:%d\n", msg, file, line);
   printk("$- PANIC -: \"%s\" at %s:%d\n\n", msg, file, line);

   void *current_RIP;
   asm( "lea 0(%%rip), %0" : "=r"(current_RIP) );  
   printk("$RIP: %x\n\n", current_RIP);
   printk("$TIME: (SINCE BOOT) %dMs\n", tsc_get_ms());
   hlt();
}
