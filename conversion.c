#include <nstd/conversion.h>

#include <ctype.h>

struct result buf_to_usize(size_t *const dst, struct buf *const buf)
{
    uint8_t *const data = buf->p;

    size_t number = 0;
    for (size_t index = 0; index < buf->len; index += 1) {
        uint8_t digit = data[index];

        if (!isdigit(digit)) {
            return result_new(-EINVAL);
        }
        digit = (uint8_t)(digit - '0');

        if (digit >= 10) {
            return result_new(-EINVAL);
        }

        struct result res = usize_checked_mul(number, 10, &number);
        if (res.reason) {
            return res;
        }

        res = usize_checked_add(number, digit, &number);
        if (res.reason) {
            return res;
        }
    }

    *dst = number;
    return result_new(0);
}

struct result usize_to_string(struct buf *const dst, const size_t src)
{
    const __auto_type res = buf_printf(dst, "%zu", src);
    if (res.reason) {
        return res;
    }

    return buf_append_byte(dst, 0x00);
}
