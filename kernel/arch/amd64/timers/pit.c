#include <arch/amd64/timers/pit.h>
#include <arch/amd64/include/pio.h>

void pit_wait(u64 ms)
{
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
