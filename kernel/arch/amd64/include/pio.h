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
