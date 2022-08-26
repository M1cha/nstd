#include <nstd/assert.h>
#include <nstd/buf.h>
#include <nstd/conversion.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

struct result
buf_vprintf(struct buf *const buf, const char *const fmt, va_list ap)
{
    BUILD_ASSERT(sizeof(size_t) >= sizeof(int));

    // UNDERFLOW: we rely on our code never leaving those fields in a bad state.
    //            `len` should always be >= `used`.
    size_t maxlen = buf->len - buf->used;

    // NOTE: we truncate the length to make sure it fits into the return value
    int err = vsnprintf(buf->p + buf->used, (size_t)(int)maxlen, fmt, ap);
    if (err < 0) {
        return result_new(err);
    }

    size_t num_printed;
    struct result res = int_to_size(&num_printed, err);
    if (res.reason) {
        return res;
    }
    if (num_printed >= maxlen) {
        return result_new(-ENOMEM);
    }

    // OVERFLOW: If `vsnprintf` works correctly, `num_printed` and thus `used`
    //           should never exceed `len`. They're all `size_t`.
    buf->used += num_printed;

    return result_new(0);
}

struct result buf_printf(struct buf *const buf, const char *const fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    const struct result res = buf_vprintf(buf, fmt, ap);
    va_end(ap);

    return res;
}

struct result
buf_append_raw(struct buf *buf, const void *const data, size_t len)
{
    if (len > buf_remaining(buf)) {
        return result_new(-ENOMEM);
    }

    memmove(buf->p + buf->used, data, len);
    buf->used += len;

    return result_new(0);
}
