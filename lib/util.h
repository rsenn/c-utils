#ifndef UTIL_H
#define UTIL_H 1

#define countof(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))

#if __STDC_VERSION__ >= 202311L
#define CTOR [[constructor]]
#define VISIBLE [[visibility("default")]]
#define HIDDEN [[visibility("hidden")]]
#else
#define CTOR __attribute__((constructor))
#define VISIBLE __attribute__((visibility("default")))
#define HIDDEN __attribute__((visibility("hidden")))
#endif

#endif /* defined UTIL_H */
