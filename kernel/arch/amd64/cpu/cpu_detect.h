#ifndef CPU_DETECT_H
#define CPU_DETECT_H

#define CPUID_VENDOR_AMD "AuthenticAMD"
#define CPUID_VENDOR_INTEL "GenuineIntel"

int detect_cpu(void);

#endif
