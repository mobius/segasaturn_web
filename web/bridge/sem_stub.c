// sem_timedwait stub for single-threaded Emscripten builds.
//
// ymir-core links moodycamel::BlockingConcurrentQueue (SCSP / VDP renderer
// event queues), whose LightweightSemaphore references sem_timedwait.
// Emscripten's libc only provides sem_timedwait in pthread builds, which this
// project intentionally avoids (no SharedArrayBuffer requirement).
//
// The timed-wait path is only ever exercised by Ymir's optional worker
// threads, all of which are disabled in this build (threadedVDP1/2 off,
// threadedSCSP off by default). This stub therefore never runs in practice;
// if it ever did, failing with ETIMEDOUT is liveness-safe: callers treat it
// as "no event available" and retry later.

#include <errno.h>
#include <semaphore.h>
#include <time.h>

int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    (void)sem;
    (void)abs_timeout;
    errno = ETIMEDOUT;
    return -1;
}
