#ifndef PANIC_H
#define PANIC_H

#include <kernel/types.h>

#define PANIC(msg) (                    \
{                                   \
panic(msg, __FILE__, __LINE__); \
__builtin_unreachable();        \
})

void panic(const char *msg, const char *file, u32 line);

#endif
