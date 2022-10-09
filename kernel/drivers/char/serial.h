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

#ifndef SERIAL_H
#define SERIAL_H

#define COM1 0x3f8
#define COM2 0x2f8

void serial_print(const char *fmt, ...);
void serial_puts(const char *str);
void serial_putchar(const char c);
int init_serial(void);

#endif
