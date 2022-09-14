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

#include <arch/amd64/include/pio.h>
#include <arch/amd64/timers/pit.h>

void pit_wait(u64 ms) {
  outb(PIT_COMMAND_REG, 0b00110000);
  while (ms--) {
    outb(PIT_CHANNEL0_DATA, 0xa9);
    outb(PIT_CHANNEL0_DATA, 0x04);

    while (1 == 1) {
      u8 hi;
      inb(PIT_CHANNEL0_DATA);
      hi = inb(PIT_CHANNEL0_DATA);
      // check for overflow
      if (hi > 0x04)
        break;
    }
  }
}
