#ifndef NSTD_ASSERT_H
#define NSTD_ASSERT_H

#include <stdlib.h>

/* C++11 has static_assert built in */
#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define BUILD_ASSERT(EXPR, MSG...) static_assert(EXPR, "" MSG)

/*
 * GCC 4.6 and higher have the C11 _Static_assert built in, and its
 * output is easier to understand than the common BUILD_ASSERT macros.
 */
#elif (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) \
    || (__STDC_VERSION__) >= 201100
#define BUILD_ASSERT(EXPR, MSG...) _Static_assert(EXPR, "" MSG)

/* Compile-time assertion that makes the build to fail.
 * Common implementation swallows the message.
 */
#else
#define BUILD_ASSERT(EXPR, MSG...) \
    enum _CONCAT(__build_assert_enum, __COUNTER__) { \
        _CONCAT(__build_assert, __COUNTER__) = 1 / !!(EXPR) \
    }
#endif

#define ASSERT(EXPR) \
    do { \
        if (!(EXPR)) { \
            abort(); \
        } \
    } while (0)

#endif /* NSTD_ASSERT_H */
