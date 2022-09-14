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

#ifndef CONSOLE_H
#define CONSOLE_H

#include <kernel/types.h>

// printk helpers
#define WARN "^WARN: "
#define ERR "^ERR: "
#define EMERG "^EMERG: "

#define pr_info(fmt, ...) printk("*" MODULE_NAME "~: " fmt "\n", ##__VA_ARGS__)
// #define pr_info(fmt, ...) printk("@%s *" MODULE_NAME ": " fmt "\n",
// get_jiffies(), ##__VA_ARGS__)
#define pr_warn(fmt, ...) printk(WARN fmt "\n", ##__VA_ARGS__)
#define pr_err(fmt, ...) printk(ERR fmt "\n", ##__VA_ARGS__)
#define pr_emerg(fmt, ...) printk(EMERG fmt "\n", ##__VA_ARGS__)

// Default colors:
#define DEFAULT_FG_COLOR 0xffffff
#define DEFAULT_BG_COLOR 0x000000

void init_console(void);
void putc(char c);
void puts(const char *str);
void cls(void);
void printk(const char *fmt, ...);
int vsnprintk(char *buffer, u32 buffer_size, const char *fmt,
              __builtin_va_list vl);

#endif // CONSOLE_H
