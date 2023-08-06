#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LEN(a) (sizeof (a) / sizeof (a)[0])

/* Given an entry of the form:
 *   ./hello(main+0x1c) [0x55932b2ae0dc]
 *    ^       ^    ^      ^
 *   elf     sym  off    addr
 * extract the symbol (here, "main") from it.
 * NOTE: symbol won't be present if -rdynamic wasn't passed to ld
 */
static inline char * __attribute__((no_instrument_function, always_inline))
get_sym(const char *entry) {
    char *paren = strchr(entry, '(');
    if (!paren) return NULL;
    char *plus = strchr(paren + 1, '+');
    if (!plus) return NULL;
    int toklen = plus - paren - 1;
    if (toklen == 0) return strdup("unknown");
    char *tok = malloc(toklen + 1);
    memcpy(tok, paren + 1, toklen);
    tok[toklen] = '\0';
    return tok;
}

static inline void __attribute__((no_instrument_function, always_inline))
print_call() {
    /* get 3 frames:
     *   __cyg_profile_func_enter
     *   callee
     *   caller
     */
    void *bt_frames[3];
    /* may have to pass -fno-omit-frame-pointer for this to work properly */
    int num_frames = backtrace(bt_frames, ARRAY_LEN(bt_frames));
    char **bt_syms = backtrace_symbols(bt_frames, num_frames);
    if (!bt_syms) return;
    if (num_frames < 3) return;
    char *caller = get_sym(bt_syms[2]);
    char *callee = get_sym(bt_syms[1]);
    printf("%s -> %s\n", caller, callee);
    free(caller);
    free(callee);
    free(bt_syms);
}

void __attribute__((no_instrument_function))
__cyg_profile_func_enter(void *this_fn, void *call_site) {
    (void)this_fn;
    (void)call_site;
    print_call();
}

void say_hello() {
#if 0
start:
    printf("%s: %p\n", __func__, &&start);
#endif
    printf("Hello, world!\n");
}

int main() {
#if 0
start:
    printf("%s: %p\n", __func__, &&start);
#endif
    say_hello();
}
