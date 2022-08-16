#ifndef MSR_H
#define MSR_H
#include <kernel/types.h>

// TODO: implement a function that checks if MSRs are available
// using this: const u32 CPUID_FLAG_MSR = 1 << 5;

void cpu_get_msr(u32 msr, u32 *lo, u32 *hi)
{
   asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void cpu_set_msr(u32 msr, u32 lo, u32 hi)
{
   asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

#endif
