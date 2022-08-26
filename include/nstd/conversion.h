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

#endif /* NSTD_CONVERSION_H */
