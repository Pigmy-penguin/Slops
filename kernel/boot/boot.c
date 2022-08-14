#include <kernel/types.h>
#include <boot/limine.h>
#include <kernel/console.h>
#include <arch/amd64/include/emu.h>
#include <drivers/char/serial.h>
#include <drivers/firmware/smbios.h>
#include <kernel/console.h>

/* The Limine requests can be placed anywhere, but it is important that
the compiler does not optimise them away, so, usually, they should
be made volatile or equivalent. */

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

struct limine_boot_time_request boot_time_request = {
    .id = LIMINE_BOOT_TIME_REQUEST,
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
    if (init_serial() == 0) {
       serial_print("Successfully set up serial console\n");
    }

    if (smbios_request.response == NULL) {
       serial_print("No SMBIOS found");
       done();
    }
    if (smbios_request.response->entry_64 != NULL) {
       parse_smbios_ep64_struct(smbios_request.response->entry_64);
    }
    else if (smbios_request.response->entry_32 != NULL) {
       parse_smbios_ep32_struct(smbios_request.response->entry_32);
    }
    else {
       serial_print("No SMBIOS found");
       done();
    }
    // We need the proccesor max speed in order to set up the TSC
    struct smbios_proc_info *proc_info = get_proc_info();
    serial_print("Processor max speed : %d MHz\n", proc_info->max_speed);

    if (framebuffer_request.response != NULL 
          || framebuffer_request.response->framebuffer_count == 1) {
       init_console(framebuffer_request.response->framebuffers[0]->address, framebuffer_request.response->framebuffers[0]->width, framebuffer_request.response->framebuffers[0]->height, framebuffer_request.response->framebuffers[0]->pitch);
       serial_print("Successfully initialized framebuffer console\n");
    }
    else
       serial_print("Error while initializing fb console\n");

    // We're done, just hang...
    done();
}