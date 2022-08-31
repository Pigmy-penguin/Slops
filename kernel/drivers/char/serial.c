#include <kernel/types.h>
#include <drivers/char/serial.h>
#include <arch/amd64/include/pio.h>
#include <kernel/string.h>
#include <kernel/console.h>

int init_serial(void)
{
   outb(COM1 + 1, 0x00); // Disable interrupts
   outb(COM1 + 3, 0x80); // Enable DLAB
   outb(COM1 + 0, 0x03); // 38400 baud
   outb(COM1 + 3, 0x03); // 8 bits, because why not
   outb(COM1 + 2, 0xc7); // I don't understand what this does
   outb(COM1 + 4, 0x0B);
   outb(COM1 + 4, 0x1E); // Set loopback mode
   outb(COM1 + 0, 0xAB); // Send byte 0xAB...

   // ... and check if we get the same byte
   if(inb(COM1 + 0) != 0xAB)
      return 1;
   // The serial is working, let's set it to operation mode
   outb(COM1 + 4, 0x0f);
   return 0;
}

static void serial_putc(char c)
{
   outb(COM1 + 0, c);
}

static void serial_puts(const char *str)
{
   while(*str)
      serial_putc(*str++); 
}

void serial_print(const char *fmt, ...)
{
	char buffer[1024];

	__builtin_va_list vl;
	__builtin_va_start(vl, fmt);

	vsnprintk(buffer, sizeof(buffer), fmt, vl);

	__builtin_va_end(vl);

	serial_puts(buffer);
}
