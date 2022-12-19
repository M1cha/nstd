#ifndef NSTD_BUF_H
#define NSTD_BUF_H

#include <nstd/result.h>

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct buf {
    void *p;
    size_t len;
    size_t used;
};

#define BUF_STACK(name, len) \
    uint8_t _bufdata_##name[(len)]; \
    struct buf name = buf_new(_bufdata_##name, (len))

#define BUF_STACK_STATIC(name, _len) \
    static uint8_t _bufdata_##name[(_len)]; \
    static struct buf name = { \
        .p = _bufdata_##name, \
        .len = (_len), \
        .used = 0, \
    }

/// create a new buffer on the given memory range
static inline struct buf buf_new(void *const p, const size_t len)
{
    return (struct buf) {
        .p = p,
        .len = len,
        .used = 0,
    };
}

static inline void buf_clear(struct buf *const buf)
{
    buf->used = 0;
}

static inline void *buf_uninit_ptr(struct buf *const buf)
{
    return ((char *)buf->p) + buf->used;
}

static inline size_t buf_remaining(struct buf *const buf)
{
    return buf->len - buf->used;
}

static inline struct result
buf_assume_init(struct buf *const buf, const size_t len)
{
    if (buf_remaining(buf) < len) {
        return result_new(-ENOMEM);
    }

    buf->used += len;

    return result_new(0);
}

#define BUF_ASSUME_INIT(buf, var) \
    ({ \
        (var) = buf_uninit_ptr((buf)); \
        buf_assume_init((buf), sizeof(*(var))); \
    })

static inline void
buf_assume_init_unchecked(struct buf *const buf, const size_t len)
{
    buf->used += len;
}

static inline void buf_remove_unchecked(struct buf *const buf, const size_t len)
{
    buf->used -= len;
}

struct result
buf_append_raw(struct buf *buf, const void *const data, size_t len);

static inline struct result
buf_append_byte(struct buf *const buf, const uint8_t byte)
{
    return buf_append_raw(buf, &byte, 1);
}

static inline struct result
buf_append_str(struct buf *const buf, const char *const str)
{
    return buf_append_raw(buf, str, strlen(str));
}

/// append the result of `vsnprintf` to the buffer
struct result buf_vprintf(struct buf *buf, const char *fmt, va_list ap);

/// calls `buf_vprintf`
struct result buf_printf(struct buf *buf, const char *fmt, ...)
    __attribute__((__format__(__printf__, 2, 3)));

/// generate arguments for `%.*s` format specifier
///
/// NOTE: `vsnprintf` uses `int` for the string lengths, so this may not print
///       the whole string.
#define BUF_PRINTF_ARGS(buf) (int)((buf)->used), (const char *)((buf)->p)

#endif /* NSTD_BUF_H */
