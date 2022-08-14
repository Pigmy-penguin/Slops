#ifndef EMU_H
#define EMU_H
#include <kernel/types.h>
#include <arch/amd64/include/pio.h>

static inline void debugcon_print(char *string, u16 len) {
	for(int i = 0; i != len; i++) {
		outb(0xe9, string[i]);
	}
}

#endif
