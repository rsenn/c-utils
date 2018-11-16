#ifndef PROCESS_H
#define PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

int process_create(const char* filename, const char* argv);

#ifdef __cplusplus
}
#endif
#endif /* defined(PROCESS_H) */
