#ifndef NSTD_CONVERSION_H
#define NSTD_CONVERSION_H

#include <nstd/result.h>

#include <errno.h>
#include <limits.h>
#include <nstd/compiler.h>
#include <nstd/num.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// NOTE: These are not supposed to be used by users of this library
//       The reason they exist is because it makes the `IMPL_` macros much
//       easier.
/// \section nstd_numbers
/// \internal
typedef unsigned char nstd_uchar_t;
typedef unsigned short nstd_ushort_t;
typedef unsigned int nstd_uint_t;
typedef unsigned long nstd_ulong_t;
typedef unsigned long long nstd_ulonglong_t;
typedef signed char nstd_schar_t;
typedef signed short nstd_sshort_t;
typedef signed int nstd_sint_t;
typedef signed long nstd_slong_t;
typedef signed long long nstd_slonglong_t;
/// \endsection

// NOTE: This works for both signed and unsigned because the source value is
//       always positive.
/// Converts an unsigned number to a number of any type
/// \internal
#define IMPL_UX_TO_Y(type_src, type_dst) \
    static inline struct result type_src##_to_##type_dst( \
        nstd_##type_dst##_t *const dst, const nstd_##type_src##_t src) \
    { \
        NSTD_DIAGNOSTIC_PUSH; \
        NSTD_DIAGNOSTIC_IGNORED_GCC("-Wtype-limits"); \
        if (src > GENERIC_MAX_VALUE(*dst)) { \
            return result_new(-ERANGE); \
        } \
        NSTD_DIAGNOSTIC_POP; \
\
        *dst = (nstd_##type_dst##_t)src; \
        return result_new(0); \
    }

// NOTE: Compared to `IMPL_UX_TO_Y`, this adds the `MIN` check which can't be
//       part of that macro since we'd be comparing a signed value against an
//       unsigned `GENERIC_MIN_VALUE`.
/// Converts a signed number to a signed number of a different type
/// \internal
#define IMPL_SX_TO_SY(type_src, type_dst) \
    static inline struct result type_src##_to_##type_dst( \
        nstd_##type_dst##_t *const dst, const nstd_##type_src##_t src) \
    { \
        NSTD_DIAGNOSTIC_PUSH; \
        NSTD_DIAGNOSTIC_IGNORED_GCC("-Wtype-limits"); \
        if (src < GENERIC_MIN_VALUE(*dst) || src > GENERIC_MAX_VALUE(*dst)) { \
            return result_new(-ERANGE); \
        } \
        NSTD_DIAGNOSTIC_POP; \
\
        *dst = (nstd_##type_dst##_t)src; \
        return result_new(0); \
    }

// NOTE: After checking that the value is not negative, we cast to the
//       corresponding unsigned type. It will be able to hold all possible
//       positive values of the source type and will not trigger
//        `-Wsign-compare`.
/// Converts a signed number to an unsigned number
/// \internal
#define IMPL_SX_TO_UY(type_src, type_dst) \
    static inline struct result s##type_src##_to_##type_dst( \
        nstd_##type_dst##_t *const dst, const nstd_s##type_src##_t src) \
    { \
        NSTD_DIAGNOSTIC_PUSH; \
        NSTD_DIAGNOSTIC_IGNORED_GCC("-Wtype-limits"); \
        if (src < 0 \
            || ((nstd_u##type_src##_t)src) > GENERIC_MAX_VALUE(*dst)) { \
            return result_new(-ERANGE); \
        } \
        NSTD_DIAGNOSTIC_POP; \
\
        *dst = (nstd_##type_dst##_t)src; \
        return result_new(0); \
    }

/// copies `src` to `*dst` of same data types
/// \internal
#define IMPL_X_TO_X(type) \
    static inline struct result type##_to_##type( \
        nstd_##type##_t *const dst, const nstd_##type##_t src) \
    { \
        *dst = src; \
        return result_new(0); \
    }

IMPL_X_TO_X(uchar);
IMPL_UX_TO_Y(ushort, uchar);
IMPL_UX_TO_Y(uint, uchar);
IMPL_UX_TO_Y(ulong, uchar);
IMPL_UX_TO_Y(ulonglong, uchar);
IMPL_SX_TO_UY(char, uchar)
IMPL_SX_TO_UY(short, uchar)
IMPL_SX_TO_UY(int, uchar)
IMPL_SX_TO_UY(long, uchar)
IMPL_SX_TO_UY(longlong, uchar)

IMPL_UX_TO_Y(uchar, ushort);
IMPL_X_TO_X(ushort);
IMPL_UX_TO_Y(uint, ushort);
IMPL_UX_TO_Y(ulong, ushort);
IMPL_UX_TO_Y(ulonglong, ushort);
IMPL_SX_TO_UY(char, ushort)
IMPL_SX_TO_UY(short, ushort)
IMPL_SX_TO_UY(int, ushort)
IMPL_SX_TO_UY(long, ushort)
IMPL_SX_TO_UY(longlong, ushort)

IMPL_UX_TO_Y(uchar, uint);
IMPL_UX_TO_Y(ushort, uint);
IMPL_X_TO_X(uint);
IMPL_UX_TO_Y(ulong, uint);
IMPL_UX_TO_Y(ulonglong, uint);
IMPL_SX_TO_UY(char, uint)
IMPL_SX_TO_UY(short, uint)
IMPL_SX_TO_UY(int, uint)
IMPL_SX_TO_UY(long, uint)
IMPL_SX_TO_UY(longlong, uint)

IMPL_UX_TO_Y(uchar, ulong);
IMPL_UX_TO_Y(ushort, ulong);
IMPL_UX_TO_Y(uint, ulong);
IMPL_X_TO_X(ulong);
IMPL_UX_TO_Y(ulonglong, ulong);
IMPL_SX_TO_UY(char, ulong);
IMPL_SX_TO_UY(short, ulong);
IMPL_SX_TO_UY(int, ulong);
IMPL_SX_TO_UY(long, ulong);
IMPL_SX_TO_UY(longlong, ulong);

IMPL_UX_TO_Y(uchar, ulonglong);
IMPL_UX_TO_Y(ushort, ulonglong);
IMPL_UX_TO_Y(uint, ulonglong);
IMPL_UX_TO_Y(ulong, ulonglong);
IMPL_X_TO_X(ulonglong);
IMPL_SX_TO_UY(char, ulonglong);
IMPL_SX_TO_UY(short, ulonglong);
IMPL_SX_TO_UY(int, ulonglong);
IMPL_SX_TO_UY(long, ulonglong);
IMPL_SX_TO_UY(longlong, ulonglong);

IMPL_UX_TO_Y(uchar, schar);
IMPL_UX_TO_Y(ushort, schar);
IMPL_UX_TO_Y(uint, schar);
IMPL_UX_TO_Y(ulong, schar);
IMPL_UX_TO_Y(ulonglong, schar);
IMPL_X_TO_X(schar);
IMPL_SX_TO_SY(sshort, schar);
IMPL_SX_TO_SY(sint, schar);
IMPL_SX_TO_SY(slong, schar);
IMPL_SX_TO_SY(slonglong, schar);

IMPL_UX_TO_Y(uchar, sshort);
IMPL_UX_TO_Y(ushort, sshort);
IMPL_UX_TO_Y(uint, sshort);
IMPL_UX_TO_Y(ulong, sshort);
IMPL_UX_TO_Y(ulonglong, sshort);
IMPL_SX_TO_SY(schar, sshort);
IMPL_X_TO_X(sshort);
IMPL_SX_TO_SY(sint, sshort);
IMPL_SX_TO_SY(slong, sshort);
IMPL_SX_TO_SY(slonglong, sshort);

IMPL_UX_TO_Y(uchar, sint);
IMPL_UX_TO_Y(ushort, sint);
IMPL_UX_TO_Y(uint, sint);
IMPL_UX_TO_Y(ulong, sint);
IMPL_UX_TO_Y(ulonglong, sint);
IMPL_SX_TO_SY(schar, sint);
IMPL_SX_TO_SY(sshort, sint);
IMPL_X_TO_X(sint);
IMPL_SX_TO_SY(slong, sint);
IMPL_SX_TO_SY(slonglong, sint);

IMPL_UX_TO_Y(uchar, slong);
IMPL_UX_TO_Y(ushort, slong);
IMPL_UX_TO_Y(uint, slong);
IMPL_UX_TO_Y(ulong, slong);
IMPL_UX_TO_Y(ulonglong, slong);
IMPL_SX_TO_SY(schar, slong);
IMPL_SX_TO_SY(sshort, slong);
IMPL_SX_TO_SY(sint, slong);
IMPL_X_TO_X(slong);
IMPL_SX_TO_SY(slonglong, slong);

IMPL_UX_TO_Y(uchar, slonglong);
IMPL_UX_TO_Y(ushort, slonglong);
IMPL_UX_TO_Y(uint, slonglong);
IMPL_UX_TO_Y(ulong, slonglong);
IMPL_UX_TO_Y(ulonglong, slonglong);
IMPL_SX_TO_SY(schar, slonglong);
IMPL_SX_TO_SY(sshort, slonglong);
IMPL_SX_TO_SY(sint, slonglong);
IMPL_SX_TO_SY(slong, slonglong);
IMPL_X_TO_X(slonglong);

#undef IMPL_UX_TO_Y
#undef IMPL_SX_TO_SY
#undef IMPL_SX_TO_UY
#undef IMPL_X_TO_X

// NOTE: we need the casts since the compiler still tries to evaluate
//       the false cases. I guess this is because nested generics aren't a
//       typical usecase.
// clang-format off
/// Generates a generic expression that calls conversion functions for type `dst`
/// \internal
#define GENERIC_TO_TYPE(dst, src, suffix) \
    _Generic(src,  \
        unsigned char : uchar_to_##suffix((void *)dst, (unsigned char)src), \
        unsigned short : ushort_to_##suffix((void *)dst, (unsigned short)src), \
        unsigned int: uint_to_##suffix((void *)dst, (unsigned int)src), \
        unsigned long : ulong_to_##suffix((void *)dst, (unsigned long)src), \
        unsigned long long : ulonglong_to_##suffix((void *)dst, (unsigned long long)src), \
        signed char : schar_to_##suffix((void *)dst, (signed char)src), \
        signed short : sshort_to_##suffix((void *)dst, (signed short)src), \
        signed int : sint_to_##suffix((void *)dst, (signed int)src), \
        signed long : slong_to_##suffix((void *)dst, (signed long)src), \
        signed long long : slonglong_to_##suffix((void *)dst, (signed long long)src) \
    )

/// convert between any two primitive number types with checking bounds
///
/// \param dst A pointer to a variable where the result will be stored if a
///            lossless conversion can be performed.
/// \param src A number of any primitive type
/// \return `struct result`
#define CONVERT_NUM(dst, src) \
    ({ \
        typeof((dst)) __convert_dst = (dst); \
        typeof((src)) __convert_src = (src); \
        _Generic(*__convert_dst, \
            unsigned char : GENERIC_TO_TYPE(__convert_dst, __convert_src, uchar), \
            unsigned short : GENERIC_TO_TYPE(__convert_dst, __convert_src, ushort), \
            unsigned int : GENERIC_TO_TYPE(__convert_dst, __convert_src, uint), \
            unsigned long : GENERIC_TO_TYPE(__convert_dst, __convert_src, ulong), \
            unsigned long long : GENERIC_TO_TYPE(__convert_dst, __convert_src, ulonglong), \
            signed char : GENERIC_TO_TYPE(__convert_dst, __convert_src, schar), \
            signed short : GENERIC_TO_TYPE(__convert_dst, __convert_src, sshort), \
            signed int : GENERIC_TO_TYPE(__convert_dst, __convert_src, sint), \
            signed long : GENERIC_TO_TYPE(__convert_dst, __convert_src, slong), \
            signed long long : GENERIC_TO_TYPE(__convert_dst, __convert_src, slonglong) \
        ); \
    })
// clang-format on

static inline struct result int_to_size(size_t *const dst, const int src)
{
    NSTD_DIAGNOSTIC_PUSH;
    NSTD_DIAGNOSTIC_IGNORED_GCC("-Wtype-limits");
    if (src < 0 || ((unsigned int)src) > SIZE_MAX) {
        return result_new(-ERANGE);
    }
    NSTD_DIAGNOSTIC_POP;

    *dst = (size_t)src;
    return result_new(0);
}

static inline struct result
ulonglong_to_usize(size_t *const dst, const unsigned long long src)
{
    if (src > SIZE_MAX) {
        return result_new(-ERANGE);
    }

    *dst = (size_t)src;
    return result_new(0);
}

static inline struct result double_to_u64(uint64_t *const dst, const double src)
{
    if (src < 0 || src > UINT64_MAX) {
        return result_new(-ERANGE);
    }

    *dst = (uint64_t)src;
    return result_new(0);
}

static inline struct result double_to_i64(int64_t *const dst, const double src)
{
    if (src < INT64_MIN || src > INT64_MAX) {
        return result_new(-ERANGE);
    }

    *dst = (int64_t)src;
    return result_new(0);
}

static inline struct result
string_to_ull(unsigned long long *const dst, const char *src)
{
    char *end = NULL;

    unsigned long long res = strtoull(src, &end, 10);
    if (res == ULLONG_MAX && errno) {
        // out of range
        return result_new(-errno);
    }
    if (*end) {
        // trailing garbage
        return result_new(-EINVAL);
    }

    *dst = res;
    return result_new(0);
}

static inline struct result string_to_usize(size_t *const dst, const char *src)
{
    unsigned long long value;
    struct result res = string_to_ull(&value, src);
    if (res.reason) {
        return res;
    }

    return ulonglong_to_usize(dst, value);
}

#endif /* NSTD_CONVERSION_H */
