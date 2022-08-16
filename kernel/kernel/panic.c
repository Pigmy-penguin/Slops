#include <kernel/panic.h>
#include <kernel/console.h>
#include <kernel/string.h>
#include <drivers/char/serial.h>

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
   serial_print("PANIC: \"%s\" at %s:%d", msg, file, line);
   printk("$ PANIC ->: %s at %s:%d", msg, file, line);
   hlt();
}
