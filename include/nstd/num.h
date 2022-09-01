#ifndef NSTD_NUM_H
#define NSTD_NUM_H

#include <errno.h>
#include <limits.h>
#include <nstd/result.h>
#include <stdint.h>

#define IS_UNSIGNED(x) (((x)-1) > 0)
#define IS_UNSIGNED_VALUE(x) (((typeof((x)))-1) > 0)

#define GENERIC_MAX_VALUE(x) \
    _Generic((x), signed char \
             : CHAR_MAX, short \
             : SHRT_MAX, int \
             : INT_MAX, long \
             : LONG_MAX, long long \
             : LLONG_MAX, unsigned char \
             : UCHAR_MAX, unsigned short \
             : USHRT_MAX, unsigned int \
             : UINT_MAX, unsigned long \
             : ULONG_MAX, unsigned long long \
             : ULLONG_MAX)

#define GENERIC_MIN_VALUE(x) \
    _Generic((x), signed char \
             : CHAR_MIN, short \
             : SHRT_MIN, int \
             : INT_MIN, long \
             : LONG_MIN, long long \
             : LLONG_MIN, \
               unsigned char : 0, \
               unsigned short : 0, \
               unsigned int : 0, \
               unsigned long : 0, \
               unsigned long long : 0)

static inline struct result
u64_checked_sub(const uint64_t self, const uint64_t rhs, uint64_t *const result)
{
    if (self < rhs) {
        return result_new(-ERANGE);
    }

    *result = self - rhs;
    return result_new(0);
}

static inline struct result
u64_checked_add(const uint64_t self, const uint64_t rhs, uint64_t *const result)
{
    if (rhs > UINT64_MAX - self) {
        return result_new(-ERANGE);
    }

    *result = self + rhs;
    return result_new(0);
}

/// Based on: https://stackoverflow.com/a/1815371
static inline struct result
u64_checked_mul(const uint64_t self, const uint64_t rhs, uint64_t *const result)
{
    uint64_t value = self * rhs;
    if (value != 0 && value / self != rhs) {
        return result_new(-ERANGE);
    }

    *result = value;
    return result_new(0);
}

static inline struct result
u32_checked_sub(const uint32_t self, const uint32_t rhs, uint32_t *const result)
{
    if (self < rhs) {
        return result_new(-ERANGE);
    }

    *result = self - rhs;
    return result_new(0);
}

#endif /* NSTD_NUM_H */
