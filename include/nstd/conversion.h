#ifndef NSTD_CONVERSION_H
#define NSTD_CONVERSION_H

#include <nstd/result.h>

#include <errno.h>

static inline struct result int_to_size(size_t *const dst, const int src)
{
    if (src < 0 || sizeof(src) > sizeof(*dst)) {
        *dst = 0;
        return result_new(-ERANGE);
    }

    *dst = (size_t)src;
    return result_new(0);
}

static inline struct result double_to_u64(uint64_t *const dst, const double src)
{
    if (src < 0 || src > UINT64_MAX) {
        *dst = 0;
        return result_new(-ERANGE);
    }

    *dst = (uint64_t)src;
    return result_new(0);
}

static inline struct result u64_to_i64(int64_t *const dst, uint64_t src)
{
    if (src > INT64_MAX) {
        return result_new(-ERANGE);
    }

    *dst = (int64_t)src;
    return result_new(0);
}

static inline struct result u64_to_u32(uint32_t *const dst, const uint64_t src)
{
    if (src > UINT32_MAX) {
        *dst = 0;
        return result_new(-ERANGE);
    }

    *dst = (uint32_t)src;
    return result_new(0);
}

static inline struct result u32_to_i32(int32_t *const dst, uint32_t src)
{
    if (src > INT32_MAX) {
        return result_new(-ERANGE);
    }

    *dst = (int32_t)src;
    return result_new(0);
}

static inline struct result i64_to_u32(uint32_t *const dst, const int64_t src)
{
    if (src < 0 || src > UINT32_MAX) {
        *dst = 0;
        return result_new(-ERANGE);
    }

    *dst = (uint32_t)src;
    return result_new(0);
}
#endif /* NSTD_CONVERSION_H */
