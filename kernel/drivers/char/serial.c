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

#include <arch/amd64/include/pio.h>
#include <drivers/char/serial.h>
#include <kernel/console.h>
#include <kernel/string.h>
#include <kernel/types.h>

int init_serial(void) {
  outb(COM1 + 1, 0x00); // Disable interrupts
  outb(COM1 + 3, 0x80); // Enable DLAB
  outb(COM1 + 0, 0x03); // 38400 baud
  outb(COM1 + 3, 0x03); // 8 bits, because why not
  outb(COM1 + 2, 0xc7); // I don't understand what this does
  outb(COM1 + 4, 0x0B);
  outb(COM1 + 4, 0x1E); // Set loopback mode
  outb(COM1 + 0, 0xAB); // Send byte 0xAB...

  // ... and check if we get the same byte
  if (inb(COM1 + 0) != 0xAB)
    return 1;
  // The serial is working, let's set it to operation mode
  outb(COM1 + 4, 0x0f);
  return 0;
}

void serial_putchar(char c) {
  switch (c) {
  case '^':
    serial_puts("\e[91m");
    return;
  case '~':
    serial_puts("\e[0m");
    return;
  case '!':
    serial_puts("\e[92m");
    return;
  case '*':
    serial_puts("\e[93m");
    return;
  case '$':
    return;
  }
  outb(COM1 + 0, c);
}

void serial_puts(const char *str) {
  while (*str)
    serial_putchar(*str++);
}

void serial_print(const char *fmt, ...) {
  char buffer[1024];

  __builtin_va_list vl;
  __builtin_va_start(vl, fmt);

  vsnprintk(buffer, sizeof(buffer), fmt, vl);

  __builtin_va_end(vl);

  serial_puts(buffer);
}
