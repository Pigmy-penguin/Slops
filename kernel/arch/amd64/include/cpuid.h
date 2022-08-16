#ifndef CPUID_H
#define CPUID_H
#include <kernel/types.h>

static inline void cpuid(int code, u32 *a, u32 *d) {
  asm volatile("cpuid":"=a"(*a),"=d"(*d):"a"(code):"ecx","ebx");
}
 
static inline int cpuid_string(int code, int where[4]) {
  __asm__ volatile("cpuid":"=a"(*where),"=b"(*(where+0)),
               "=d"(*(where+1)),"=c"(*(where+2)):"a"(code));
  return (int)where[0];
}

#endif
