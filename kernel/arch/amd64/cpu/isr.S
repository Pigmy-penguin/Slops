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

#define INTERRUPT_NAME(int_no) .quad __interrupt##int_no

#define INTERRUPT_ERR(int_no)	__interrupt##int_no: \
									pushq  $int_no; \
									jmp __interrupt_common;

#define INTERRUPT_NOERR(int_no)	__interrupt##int_no: \
									pushq  $0; \
									pushq  $int_no; \
									jmp __interrupt_common;

#define __pusha	\
    push %rax; \
    push %rbx; \
    push %rcx; \
    push %rdx; \
    push %rsi; \
    push %rdi; \
    push %rbp; \
    push %r8; \
    push %r9; \
    push %r10; \
    push %r11; \
    push %r12; \
    push %r13; \
    push %r14; \
    push %r15;

#define __popa \
    pop %r15;	\
    pop %r14;	\
    pop %r13;	\
    pop %r12;	\
    pop %r11;	\
    pop %r10;	\
    pop %r9;	\
    pop %r8;	\
    pop %rbp;	\
    pop %rdi;	\
    pop %rsi;	\
    pop %rdx;	\
    pop %rcx;	\
    pop %rbx;	\
    pop %rax;

.extern interrupts_handler

# TODO: maybe we need to clear interrupts
__interrupt_common:
    cld

    __pusha

    mov %rsp, %rdi

    call interrupts_handler

    # mov %rax, %rsp

    __popa

    add $16, %rsp # pop errcode and int number

    iretq

INTERRUPT_NOERR(0)
INTERRUPT_NOERR(1)
INTERRUPT_NOERR(2)
INTERRUPT_NOERR(3)
INTERRUPT_NOERR(4)
INTERRUPT_NOERR(5)
INTERRUPT_NOERR(6)
INTERRUPT_NOERR(7)
INTERRUPT_ERR(8)
INTERRUPT_NOERR(9)
INTERRUPT_ERR(10)
INTERRUPT_ERR(11)
INTERRUPT_ERR(12)
INTERRUPT_ERR(13)
INTERRUPT_ERR(14)
INTERRUPT_NOERR(15)
INTERRUPT_NOERR(16)
INTERRUPT_ERR(17)
INTERRUPT_NOERR(18)
INTERRUPT_NOERR(19)
INTERRUPT_NOERR(20)
INTERRUPT_NOERR(21)
INTERRUPT_NOERR(22)
INTERRUPT_NOERR(23)
INTERRUPT_NOERR(24)
INTERRUPT_NOERR(25)
INTERRUPT_NOERR(26)
INTERRUPT_NOERR(27)
INTERRUPT_NOERR(28)
INTERRUPT_NOERR(29)
INTERRUPT_ERR(30)
INTERRUPT_NOERR(31)

INTERRUPT_NOERR(32)
INTERRUPT_NOERR(33)
INTERRUPT_NOERR(34)
INTERRUPT_NOERR(35)
INTERRUPT_NOERR(36)
INTERRUPT_NOERR(37)
INTERRUPT_NOERR(38)
INTERRUPT_NOERR(39)
INTERRUPT_NOERR(40)
INTERRUPT_NOERR(41)
INTERRUPT_NOERR(42)
INTERRUPT_NOERR(43)
INTERRUPT_NOERR(44)
INTERRUPT_NOERR(45)
INTERRUPT_NOERR(46)
INTERRUPT_NOERR(47)

INTERRUPT_NOERR(127)
INTERRUPT_NOERR(128)

.globl __interrupt_vector

__interrupt_vector:
INTERRUPT_NAME(0)
INTERRUPT_NAME(1)
INTERRUPT_NAME(2)
INTERRUPT_NAME(3)
INTERRUPT_NAME(4)
INTERRUPT_NAME(5)
INTERRUPT_NAME(6)
INTERRUPT_NAME(7)
INTERRUPT_NAME(8)
INTERRUPT_NAME(9)
INTERRUPT_NAME(10)
INTERRUPT_NAME(11)
INTERRUPT_NAME(12)
INTERRUPT_NAME(13)
INTERRUPT_NAME(14)
INTERRUPT_NAME(15)
INTERRUPT_NAME(16)
INTERRUPT_NAME(17)
INTERRUPT_NAME(18)
INTERRUPT_NAME(19)
INTERRUPT_NAME(20)
INTERRUPT_NAME(21)
INTERRUPT_NAME(22)
INTERRUPT_NAME(23)
INTERRUPT_NAME(24)
INTERRUPT_NAME(25)
INTERRUPT_NAME(26)
INTERRUPT_NAME(27)
INTERRUPT_NAME(28)
INTERRUPT_NAME(29)
INTERRUPT_NAME(30)
INTERRUPT_NAME(31)

INTERRUPT_NAME(32)
INTERRUPT_NAME(33)
INTERRUPT_NAME(34)
INTERRUPT_NAME(35)
INTERRUPT_NAME(36)
INTERRUPT_NAME(37)
INTERRUPT_NAME(38)
INTERRUPT_NAME(39)
INTERRUPT_NAME(40)
INTERRUPT_NAME(41)
INTERRUPT_NAME(42)
INTERRUPT_NAME(43)
INTERRUPT_NAME(44)
INTERRUPT_NAME(45)
INTERRUPT_NAME(46)
INTERRUPT_NAME(47)

INTERRUPT_NAME(127)
INTERRUPT_NAME(128)
