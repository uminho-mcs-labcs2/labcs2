/*
 * P1 - "Dangling" : a use-after-free challenge.
 *
 * This is the SAME bug class as CVE-2026-109619 (the PHP2Shell use-after-free),
 * shrunk to a target you can understand completely. Your job: make the program
 * call win(), which reads and prints /flag.
 *
 * Protocol (one command per line, over the network):
 *     new            allocate a session (its 'action' points to a harmless default)
 *     del            free the current session   <-- note what is NOT done here
 *     set <hex>      allocate a same-sized buffer and fill it with your bytes
 *     go             invoke the current session's 'action'
 *     exit           quit
 *
 * Build (deterministic addresses - no PIE, no stack protector):
 *     gcc -no-pie -fno-stack-protector -w -o chall chall.c
 *
 * Hint: `nm chall | grep ' win'` tells you where win() lives. The struct is
 *       { char name[16]; void (*action)(void); } - so 'action' sits right after
 *       16 bytes. Think about which chunk `set` reuses after `del`.
 *
 * There is NO memory-safety tooling here on purpose: this is the attacker's view.
 * (To see the DEFENDER's view of the same bug, build it with
 *  `-fsanitize=address` and watch AddressSanitizer catch the use-after-free.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct session {
    char name[16];
    void (*action)(void);
};

static struct session *slot = NULL;

static void default_action(void) {
    puts("[default] a harmless action");
}

/* The target. Not called anywhere by normal control flow. */
void win(void) {
    puts("[win] control reached win() - reading the flag:");
    FILE *f = fopen("/flag", "r");
    if (!f) { puts("[win() reached, but /flag is missing on this host]"); return; }
    char buf[128];
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    buf[n] = '\0';
    fputs(buf, stdout);
    fputc('\n', stdout);
    fclose(f);
}

static int readline(char *buf, int sz) {
    if (!fgets(buf, sz, stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

static int hexval(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    char line[1024];
    puts("== Dangling ==  commands: new | del | set <hex> | go | exit");

    while (readline(line, sizeof line)) {
        if (!strncmp(line, "new", 3)) {
            slot = malloc(sizeof *slot);
            if (slot) { memset(slot, 0, sizeof *slot); slot->action = default_action; }
            puts("[+] new session");
        } else if (!strncmp(line, "del", 3)) {
            free(slot);                 /* BUG: 'slot' is freed but kept (dangling) */
            puts("[+] session freed");
        } else if (!strncmp(line, "set ", 4)) {
            const char *hex = line + 4;
            unsigned char raw[64];
            int rn = 0;
            for (int i = 0; hex[i] && hex[i + 1] && rn < (int) sizeof raw; i += 2) {
                int hi = hexval(hex[i]), lo = hexval(hex[i + 1]);
                if (hi < 0 || lo < 0) break;
                raw[rn++] = (unsigned char) ((hi << 4) | lo);
            }
            unsigned char *p = malloc(sizeof(struct session)); /* same size -> reuses freed chunk */
            int cp = rn < (int) sizeof(struct session) ? rn : (int) sizeof(struct session);
            memcpy(p, raw, cp);
            printf("[+] wrote %d byte(s)\n", cp);
        } else if (!strncmp(line, "go", 2)) {
            if (slot && slot->action) slot->action();  /* UAF: may call attacker-chosen pointer */
            else puts("[-] nothing to do");
        } else if (!strncmp(line, "exit", 4)) {
            break;
        } else {
            puts("[-] unknown command");
        }
    }
    return 0;
}
