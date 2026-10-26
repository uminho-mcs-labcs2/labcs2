/*
 * use-after-free (UAF) -- MINIMAL, didactic example.
 *
 * "Use-after-free" means the program keeps using a chunk of memory after it has
 * already been handed back (freed) for reuse. This is the *class* of bug behind
 * CVE-2026-109619 (interpreter state kept between requests whose lifetime is
 * mishandled). This is NOT the PHP bug -- it is the bare idea, in a few lines.
 *
 * Background: the "heap" is the pool of memory a C program uses for dynamic
 * allocations, requested with malloc() and released with free(). A "function
 * pointer" is a variable that stores the address of a function to call.
 *
 * -----------------------------------------------------------------------------
 * There are TWO ways to run this, and they show DIFFERENT things. Run both.
 * -----------------------------------------------------------------------------
 *
 * (A) WITH AddressSanitizer (ASan) -- a compiler tool that detects memory errors
 *     at runtime and prints a precise report:
 *
 *         gcc -fsanitize=address -g uaf.c -o uaf_asan && ./uaf_asan
 *
 *     ASan STOPS the program the instant 's' is used after free() and reports a
 *     "heap-use-after-free", pointing to exactly where the memory was freed and
 *     where it was wrongly reused. NOTE: ASan quarantines freed memory, so the
 *     "attacker" malloc below will NOT reoccupy the same block -- ASan catches the
 *     mistake before the reuse scenario can play out. ASan is the DIAGNOSIS view.
 *
 * (B) WITHOUT AddressSanitizer -- an ordinary build:
 *
 *         gcc -g uaf.c -o uaf && ./uaf
 *
 *     Here the standard allocator typically DOES reuse the just-freed block for the
 *     next same-sized malloc. So 's->on_close' ends up reading the attacker's bytes
 *     and the program jumps to a bogus address -- usually crashing. This is the
 *     CONSEQUENCE view: it shows why the bug is dangerous, without the diagnosis.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct session {
    char user[16];
    void (*on_close)(void);   /* a function pointer stored inside the struct */
};

static void legit_close(void)   { puts("[ok] session closed normally"); }

int main(void) {
    struct session *s = malloc(sizeof *s);
    strcpy(s->user, "alice");
    s->on_close = legit_close;

    free(s);                  /* (1) the session's memory is released... */

    /* (2) ...and a "next request" asks for a same-sized block. In a real server
     * its contents could be attacker-controlled. (Under ASan this will NOT reuse
     * the same address -- see note (A) above.) */
    char *attacker = malloc(sizeof *s);
    memset(attacker, 0x41, sizeof *s);   /* fill with 'AAAA...' where on_close was */

    /* (3) THE BUG: we use 's' after it was freed. Its function pointer no longer
     * points to legit_close. In a real exploit the attacker arranges for it to
     * point to code of their choosing -- this is a "control-flow hijack": the
     * program is made to run where the attacker decides, not where the author did. */
    printf("[uaf] invoking on_close through a freed pointer...\n");
    s->on_close();

    free(attacker);
    return 0;
}
