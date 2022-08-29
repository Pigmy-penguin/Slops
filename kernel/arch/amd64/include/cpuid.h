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
