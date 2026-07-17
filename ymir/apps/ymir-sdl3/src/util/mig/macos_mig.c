// Provides the mig server implementation for the current architecture
// These files are generated from cmake using the 'mig' command

#if defined(__x86_64__)
#include "x64/mach_exc_server.c"
#elif defined(__aarch64__) || defined(__arm64__)
#include "a64/mach_exc_server.c" 
#else
#error "Unsupported MIG architecture
#endif