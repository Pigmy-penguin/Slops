#ifndef TSC_H
#define TSC_H
#include <kernel/types.h>

u64 get_tsc(void); 
u64 tsc_get_ms(void);
void calibrate_tsc(void);

#endif
