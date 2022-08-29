#include <kernel/types.h>
#include <kernel/string.h>
#include <kernel/console.h>
#include <arch/amd64/cpu/cpu_detect.h>
#include <arch/amd64/include/cpuid.h>

#define MODULE_NAME "cpu"

static void detect_intel(void) {
   // TODO: add more things to detect
   u32 hi, lo;
   u64 type;
   cpuid(0, &hi, &lo);
   type = (hi >> 12) & 0x3;
   switch (type) {
      case 0:
         pr_info("CPU type: Original equipment manufacturer (OEM) Processor");
         break;
      case 1:
         pr_info("CPU type: Intel Overdrive Processor");
         break;
      case 2:
         pr_info("CPU type: Dual capable");
         break;
      case 3:
         pr_warn("CPU type: Reserved");
   }
}

static void detect_amd(void) {
   // TODO: detect amd cpus
}

int detect_cpu(void) {
   // let's detect the cpu vendor
   static char magic[16] = "";
   cpuid_string(0, (int*)(magic));
   pr_info("CPU Vendor string: %s", magic);
   if (strcmp(magic, CPUID_VENDOR_INTEL) == 0) 
   {
      pr_info("Intel specific detection");
      detect_intel();
      return 0;
   }
   else if (strcmp(magic, CPUID_VENDOR_AMD) == 0) 
   {
      pr_info("AMD specific detection");
      detect_amd();
      return 0;
   }
   else
      pr_info("Unknown x86 CPU");
   return 1;
}
