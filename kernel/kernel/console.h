#ifndef CONSOLE_H
#define CONSOLE_H

#include <kernel/types.h>

// printk helpers
#define WARN "^WARN: "
#define ERR "^ERR: "
#define EMERG "^EMERG: "

#define pr_info(fmt, ...) printk(fmt "\n", ##__VA_ARGS__)
#define pr_warn(fmt, ...) printk(WARN fmt "\n", ##__VA_ARGS__)
#define pr_err(fmt, ...) printk(ERR fmt "\n", ##__VA_ARGS__)
#define pr_emerg(fmt, ...) printk(EMERG fmt "\n", ##__VA_ARGS__)

// Default colors:
#define DEFAULT_FG_COLOR 0xdfd8ce
#define DEFAULT_BG_COLOR 0x282828

void init_console(void *fb_addr, u16 fb_width, u16 fb_height, u16 fb_pitch);
void putc(char c);
void puts(const char *str); 
void cls(void);
void printk(const char *fmt, ...);

#endif // CONSOLE_H
