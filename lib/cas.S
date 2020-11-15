#ifdef __aarch64__
.global __atomic_compare_and_swap __atomic_compare_and_swap
    :.weak __sync_val_compare_and_swap __sync_val_compare_and_swap : sub sp,
    sp, #16 add x4, sp, 8 str x0,
    [ sp, 8 ]
    // atomic64_cmpxchg
    1 : ldxr x0,
        [x4] cmp x0,
        x1 b.ne 2f stxr w3,
        x2,
        [x4] cbnz w3,
        1b 2 : add sp,
               sp,
               16 ret
#endif
