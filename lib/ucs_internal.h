#ifndef UCS_INTERNAL_H
#define UCS_INTERNAL_H

#include "ucs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PUNYCODE_BASE 36
#define PUNYCODE_TMIN 1
#define PUNYCODE_TMAX 26
#define PUNYCODE_INITIAL_N 128
#define PUNYCODE_INITIAL_BIAS 72

/* bias adaptation of RFC 3492 section 6.1 */
uint32 ucs_punycode_adapt(uint32 delta, uint32 numpoints, int first);

/* threshold t(k) for digit position k and the current bias */
#define PUNYCODE_T(k, bias) ((k) <= (bias) ? PUNYCODE_TMIN : (k) >= (bias) + PUNYCODE_TMAX ? PUNYCODE_TMAX : (k) - (bias))

#ifdef __cplusplus
}
#endif

#endif /* defined(UCS_INTERNAL_H) */
