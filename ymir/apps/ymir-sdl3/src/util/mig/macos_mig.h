// Provides the mig server implementation for the current architecture
// These files are generated from cmake using the 'mig' command

#if defined(__x86_64__)
    #define mig_external extern "C"
    #include "x64/mach_exc_server.h"
#elif defined(__aarch64__) || defined(__arm64__)
    #define mig_external extern "C"
    #include "a64/mach_exc_server.h"
#else
    #error "Unsupported mig architecture
#endif