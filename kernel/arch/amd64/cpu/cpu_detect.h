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

#ifndef CPU_DETECT_H
#define CPU_DETECT_H

#include <kernel/types.h>

#define CPUID_VENDOR_AMD "AuthenticAMD"

// Intel
#define CPUID_VENDOR_INTEL "GenuineIntel"

struct cpu_version {
  u8 stepping_id : 4;
  u8 model : 4;
  u8 family_id : 4;
  u8 type : 2;
  u8 reserved0 : 2;
  u8 extended_model_id : 4;
  u8 extended_family_id;
  u8 reserved1 : 4;
} __attribute((packed));

int detect_cpu(void);

#endif
