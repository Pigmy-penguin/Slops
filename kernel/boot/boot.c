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

#include <kernel/types.h>
#include <boot/limine.h>
#include <kernel/console.h>
#include <arch/amd64/include/emu.h>
#include <drivers/char/serial.h>
#include <drivers/firmware/smbios.h>
#include <kernel/console.h>
#include <arch/amd64/cpu/gdt.h>
#include <arch/amd64/cpu/cpu_detect.h>
#include <arch/amd64/timers/tsc.h>

#define MODULE_NAME "boot"

u64 __time_at_boot;
u64 __physical_base;
u64 __virtual_base;

struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

struct limine_smbios_request smbios_request = {
    .id = LIMINE_SMBIOS_REQUEST,
    .revision = 0   
};

struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

static void done(void) {
 for (;;) {
        __asm__("hlt");
    }
}

// The following will be our kernel's entry point.
void _start(void) 
{
    __time_at_boot = get_tsc();
    if (init_serial() == 0) {
       serial_print("Successfully set up serial console\n");
    }

    if (framebuffer_request.response != NULL 
          || framebuffer_request.response->framebuffer_count == 1) {
       init_console(framebuffer_request.response->framebuffers[0]->address, framebuffer_request.response->framebuffers[0]->width, framebuffer_request.response->framebuffers[0]->height, framebuffer_request.response->framebuffers[0]->pitch);
       serial_print("Successfully initialized framebuffer console\n");
    }
    else
       serial_print("Error while initializing fb console\n");

    if (kernel_address_request.response != NULL) {
       __physical_base = kernel_address_request.response->physical_base;
       __virtual_base = kernel_address_request.response->virtual_base;
       pr_info("Kernel physical base address: %x", __physical_base);
       pr_info("Kernel virtual base address: %x", __virtual_base);
    }

    detect_cpu();

    calibrate_tsc();
    pr_info("Milliseconds since boot: %d", tsc_get_ms());
    if (bootloader_info_request.response != NULL)
       pr_info("Bootloader: %s %s", bootloader_info_request.response->name, bootloader_info_request.response->version);
    else
       pr_warn("Unable to get bootloader information");

    if (smbios_request.response == NULL) {
       pr_warn("No SMBIOS found");
       done();
    }
    if (smbios_request.response->entry_64 != NULL) {
       parse_smbios_ep64_struct(smbios_request.response->entry_64);
    }
    else if (smbios_request.response->entry_32 != NULL) {
       parse_smbios_ep32_struct(smbios_request.response->entry_32);
    }
    else {
       pr_warn("No SMBIOS found");
       done();
    }
    // Get some information about the cpu using smbios
    struct smbios_proc_info *proc_info = get_proc_info();
    pr_info("Processor max speed : %d MHz", proc_info->max_speed);

    gdt_load();
    pr_warn("End of kernel");
    done();
}
