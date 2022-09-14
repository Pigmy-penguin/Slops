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

static inline void cpuid(int code, u32 *eax, u32 *ebx, u32 *ecx, u32 *edx) {
  asm volatile("cpuid"
               : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
               : "a"(code)
               :);
}

static inline u32 get_cpuid_max(unsigned int ext, unsigned int *sig) {
  u32 b, c, d;
  u32 max_leaf;

  cpuid(ext, &max_leaf, &b, &c, &d);
  if (sig)
    *sig = b;

  return max_leaf;
}

static inline int cpuid_string(int code, int where[4]) {
  __asm__ volatile("cpuid"
                   : "=a"(*where), "=b"(*(where + 0)), "=d"(*(where + 1)),
                     "=c"(*(where + 2))
                   : "a"(code));
  return (int)where[0];
}

#endif
