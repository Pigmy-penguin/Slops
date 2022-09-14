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

#ifndef EMU_H
#define EMU_H
#include <arch/amd64/include/pio.h>
#include <kernel/types.h>

static inline void debugcon_print(char *string, u16 len) {
  for (int i = 0; i != len; i++) {
    outb(0xe9, string[i]);
  }
}

#endif
