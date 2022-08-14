#ifndef PIO_H
#define PIO_H

#include <kernel/types.h>

static inline u8 inb(u16 port) {
    u8 ret;
    asm volatile("in %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u16 inw(u16 port) {
    u16 ret;
    asm volatile("in %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u32 ind(u16 port) {
    u32 ret;
    asm volatile("in %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(u16 port, u8 data) {
    asm volatile("out %0, %1" :: "a"(data), "Nd"(port));
}

static inline void outw(u16 port, u16 data) {
    asm volatile("out %0, %1" :: "a"(data), "Nd"(port));
}

static inline void outd(u16 port, u32 data) {
    asm volatile("out %0, %1" :: "a"(data), "Nd"(port));
}

#endif
