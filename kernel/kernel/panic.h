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

#ifndef PANIC_H
#define PANIC_H

#include <kernel/types.h>

#define PANIC(msg, ...)                                                        \
  ({                                                                           \
    panic(msg, __FILE__, __LINE__, ##__VA_ARGS__);                             \
    __builtin_unreachable();                                                   \
  })

void panic(const char *msg, const char *file, u32 line, ...);

#endif
