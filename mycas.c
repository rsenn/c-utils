#include <stdint.h>

#if defined(__x86_64__)
uint64_t
__compare_and_swap(uint64_t* ptr, uint64_t new_val, uint64_t old_val) {
  uint64_t out;
#else
uint32_t
__compare_and_swap(uint32_t* ptr, uint32_t new_val, uint32_t old_val) {
  uint32_t out;
#endif 

  // newline after `lock' for the work around of apple's gas(?) bug.
  asm volatile("lock "
#if defined(__x86_64__)
      "cmpxchgq %2,%1"
#else
      "cmpxchgl %2,%1"
#endif 
      : "=a"(out), "+m"(*ptr) : "q"(new_val), "0"(old_val) : "cc");

  return out;
}

