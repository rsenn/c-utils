/**
 * @defgroup   thread
 * @brief      Thread module.
 * @{
 */
#ifndef THREAD_H
#define THREAD_H 1

#ifndef thread_local
#if defined(__TINYC__)
#define thread_local
#elif defined(_Thread_local) || defined(__TINYC__)
#define thread_local _Thread_local
#elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#define thread_local __thread
#elif defined(_WIN32)
#define thread_local __declspec(thread)
#else
#error No TLS implementation found.
#endif
#endif

/** @} */

#endif /* THREAD_H */
