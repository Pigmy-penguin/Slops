#ifndef PIT_H
#define PIT_H
#include <kernel/types.h>

#define PIT_COMMAND_REG 0x43
#define PIT_CHANNEL0_DATA 0x40

void pit_wait(u64 ms);
void pit_init(void);

#endif
