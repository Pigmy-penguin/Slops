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

#include <arch/amd64/timers/tsc.h>
#include <drivers/video/fb.h>
#include <kernel/console.h>
#include <kernel/string.h>
#include <kernel/types.h>

#include <drivers/char/serial.h>

#define MODULE_NAME "console"

extern u32 *fb;
extern u16 width;
extern u16 height;
extern u16 pitch;
static int early;

void static early_puts(const char *str) { serial_puts(str); }

void static late_puts(const char *str) {
#ifdef DEBUG
  serial_puts(str);
#endif
  while (*str) 
    fb_putchar(*str++);
}

void puts(const char *str) {
  if (early == 1)
    early_puts(str);
  else
    late_puts(str);
}

void init_console(int early_flag) {
  if (early_flag == 1) {
    init_serial();
    early = 1;
  } else {
    early = 0;
    puts("\nSlops version 0.0.0\n\n");
  }
}

int vsnprintk(char *buffer, u32 buffer_size, const char *fmt,
              __builtin_va_list vl) {
  char c;
  char buf[20];
  char *p = NULL;

  u32 buffer_index = 0;

  while ((c = *fmt++) != 0) {
    if (c == '%') {
      c = *fmt++;
      switch (c) {
      case 'p':
      case 'x':
        buf[0] = '0';
        buf[1] = 'x';
        itoa(buf + 2, c, __builtin_va_arg(vl, int));
        p = buf;
        goto string;
        break;
        // case 'l':
        //      *fmt++;
        //      c = 'd';
        //      // fallthrough
      case 'd':
      case 'u':
        itoa(buf, c, __builtin_va_arg(vl, int));
        p = buf;
        goto string;
        break;
      case 's':
        p = __builtin_va_arg(vl, char *);
        if (!p)
          p = "(null)";
      string:
        while (*p) {
          buffer[buffer_index++] = (*p++);
        }
        break;
      default:
        buffer[buffer_index++] = __builtin_va_arg(vl, int);
        break;
      }
    } else {
      buffer[buffer_index++] = c;
    }
    if (buffer_index >= buffer_size - 1) {
      break;
    }
  }

  buffer[buffer_index] = '\0';

  return buffer_index;
}

void printk(const char *fmt, ...) {
  char buffer[1024];

  __builtin_va_list vl;
  __builtin_va_start(vl, fmt);

  vsnprintk(buffer, sizeof(buffer), fmt, vl);

  __builtin_va_end(vl);
  char ts[17] = "![    0.000] ";
  u64 ms = tsc_get_ms();
  // Display timestamp
  // TODO: optimize timestamp display

  if (fmt[0] == '$')
    ;

  else if (ms == 0)
    puts(ts);

  else {
    // Count number of digits
    u64 seconds = ms / 1000;
    u64 milliseconds = ms - (seconds * 1000);
    int num = seconds;
    u8 count = 0;
    do {
      // Increment digit count
      count++;
      // Remove last digit of num
      num /= 10;
    } while (num != 0);

    int x = 2;
    for (int i = (5 - count); i != 0; i--) {
      ts[x] = ' ';
      x++;
    }

    itoa(&ts[x], 10, seconds);
    for (int i = count; i != 0; i--) {
      x++;
    }
    ts[x] = '.';
    x++;
    num = milliseconds;
    u8 count2 = 0;
    do {
      // Increment digit count
      count2++;
      // Remove last digit of num
      num /= 10;
    } while (num != 0);

    if (count2 == 2) {
      ts[x] = '0';
      x++;
    } else if (count2 == 1) {
      ts[x] = '0';
      x++;
      ts[x] = '0';
      x++;
    }
    itoa(&ts[x], 10, milliseconds);
    ts[11] = ']';
    ts[12] = ' ';
    puts(ts);
  }
  puts(buffer);
}

char *get_timestamp(void) {
  // int count = 0;
  char *ts = "[    0.000000]\x00";
  u64 ms = tsc_get_ms();
  if (ms == 0)
    return ts;
  return ts;
}
