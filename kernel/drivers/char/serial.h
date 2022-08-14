#ifndef SERIAL_H
#define SERIAL_H

#define COM1 0x3f8
#define COM2 0x2f8

void serial_print(const char *fmt, ...);
int init_serial(void);

#endif
