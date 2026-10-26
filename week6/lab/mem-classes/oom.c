/*
 * Mishandled "out-of-memory" (OOM) -- MINIMAL, didactic example.
 *
 * "Out of memory" is the situation where a request for memory is refused: in C,
 * malloc() returns NULL instead of a valid address. The code that is supposed to
 * cope with that refusal is rarely exercised, because under normal load it never
 * runs -- so bugs can hide there for years. This example introduces unsafe
 * allocation-failure handling, a related error-path family to CVE-2026-109618.
 * It is NOT the PHP bug: PHP's OOM early exit can expose partially initialized
 * state to cleanup, rather than simply writing through an unchecked NULL pointer.
 *
 * Run it:
 *     gcc -fsanitize=address -g oom.c -o oom && ./oom
 *
 * (AddressSanitizer, "ASan", is a compiler tool that detects memory errors at
 * runtime. Here it will report an invalid write to a NULL/zero address.)
 *
 * The moral: the error path -- the "sad path" -- is security-critical code, even
 * though, or precisely because, it almost never executes in normal operation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Stands in for an allocation that FAILS, as one would under memory pressure. */
static void *alloc_that_fails(size_t n) {
    (void) n;                /* unused; we always fail on purpose */
    return NULL;             /* OOM: the allocation did not succeed */
}

int main(void) {
    size_t n = 32;
    char *buf = alloc_that_fails(n);

    /* THE BUG: the code assumes the allocation succeeded and never checks for NULL.
     * Writing through that NULL pointer is an invalid memory access: here it simply
     * crashes. Other OOM handling mistakes, such as exposing partially initialized
     * state to cleanup, can instead produce corruption. This demo does not model
     * that corruption mechanism. */
    printf("[oom] writing into a buffer that was never allocated...\n");
    memset(buf, 0x42, n);    /* invalid write through a NULL pointer */

    puts("(never reached when buf == NULL)");
    free(buf);
    return 0;
}

/*
 * The CORRECT version always checks the allocation's return value before using it:
 *
 *     char *buf = alloc_that_fails(n);
 *     if (buf == NULL) {
 *         // handle the failure safely: log it, abort this request, free partial
 *         // state, and return an error -- do NOT continue as if it had succeeded.
 *         return 1;
 *     }
 */
