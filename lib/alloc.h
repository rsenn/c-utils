#ifndef NEMO_ALLOC_H
#define NEMO_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

void* alloc(unsigned long n);
void alloc_free(void* x);

unsigned int alloc_re(void** x, unsigned long m, unsigned long n);

#ifdef __cplusplus
}
#endif

#endif
