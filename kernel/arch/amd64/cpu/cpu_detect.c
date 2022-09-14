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

#include <arch/amd64/cpu/cpu_detect.h>
#include <arch/amd64/include/cpuid.h>
#include <kernel/console.h>
#include <kernel/string.h>
#include <kernel/types.h>

#define MODULE_NAME "cpu"

/* Print Registers */
static void printregs(int eax, int ebx, int ecx, int edx) {
  int j;
  char string[17];
  string[16] = '\0';
  for (j = 0; j < 4; j++) {
    string[j] = eax >> (8 * j);
    string[j + 4] = ebx >> (8 * j);
    string[j + 8] = ecx >> (8 * j);
    string[j + 12] = edx >> (8 * j);
  }
  puts(string);
}

static void detect_intel(void) {
  // TODO: add more things to detect
  u32 a, b, c, d;
  u32 max_leaf;
  u32 family;
  u32 model;

  cpuid(1, &a, &b, &c, &d);
  struct cpu_version *proc_info = (struct cpu_version *)&a;

  switch (proc_info->type) {
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

  if (proc_info->family_id == 0xf)
    family = proc_info->family_id + proc_info->extended_family_id;
  else
    family = proc_info->family_id;

  if (proc_info->family_id == 0xf || proc_info->family_id == 0x06)
    model = (proc_info->extended_model_id << 4) + proc_info->model;
  else
    model = proc_info->model;

  pr_info("CPU family: %d", family);
  pr_info("Model: %d", model);

  // Print brand string
  cpuid(0x80000000, &max_leaf, &b, &c, &d);

  if (max_leaf >= 0x80000004) {
    printk("*" MODULE_NAME "~: Model Name: ");
    if (max_leaf >= 0x80000002) {
      cpuid(0x80000002, &a, &b, &c, &d);
      printregs(a, b, c, d);
    }
    if (max_leaf >= 0x80000003) {
      cpuid(0x80000003, &a, &b, &c, &d);
      printregs(a, b, c, d);
    }
    if (max_leaf >= 0x80000004) {
      cpuid(0x80000004, &a, &b, &c, &d);
      printregs(a, b, c, d);
    }
    putc('\n');
  }
}

static void detect_amd(void) {
  // TODO: detect amd cpus
}

int detect_cpu(void) {
  // let's detect the cpu vendor
  static char magic[16] = "";
  cpuid_string(0, (int *)(magic));
  pr_info("CPU Vendor string: %s", magic);
  if (strcmp(magic, CPUID_VENDOR_INTEL) == 0) {
    pr_info("Intel specific detection");
    detect_intel();
    return 0;
  } else if (strcmp(magic, CPUID_VENDOR_AMD) == 0) {
    pr_info("AMD specific detection");
    detect_amd();
    return 0;
  } else
    pr_info("Unknown x86 CPU");
  return 1;
}
