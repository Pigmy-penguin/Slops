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

#include <arch/amd64/timers/tsc.h>
#include <drivers/char/serial.h>
#include <arch/amd64/include/msr.h>
#include <kernel/console.h>
#include <arch/amd64/timers/pit.h>
#include <cpuid.h>

#define MODULE_NAME "tsc"

static u64 tsc_freq = 0;
extern u64 __time_at_boot;

// TODO: use our cpuid function
// TODO: only use the cpu to get the tsc, using MSRs
void calibrate_tsc(void)
{
   u32 a, b, c, d;
   u64 tsc_1, tsc_2;

   // Get the cpu/tsc frequency using cpuid
   u32 maxleaf = __get_cpuid_max(0, NULL);

   if (maxleaf >= 0x15) {
      __cpuid(0x15, a, b, c, d);
      // EBX : TSC/Crystal ratio, ECX : Crystal Hz 
      if (b && c)
         tsc_freq = (c * (b / a));
   }
   // Else calculate it
   tsc_1 = get_tsc();
   pit_wait(100);
   tsc_2 = get_tsc();

   tsc_freq = ((tsc_2 - tsc_1) * 10) - 11111;
   pr_info("TSC frequency (not accurate): %d KHz", tsc_freq / 1000);
}

u64 tsc_get_ms(void)
{
   if (tsc_freq == 0)
      return 0;
   return ((get_tsc()-__time_at_boot) / (tsc_freq/1000));
}

u64 get_tsc(void)
{
   u32 lo, hi;
   // We cannot use "=A", since this would use %rax on x86_64 //
   __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
   return (u64)hi << 32 | lo;
};

