#include <nstd/time.h>

#include <nstd/assert.h>
#include <nstd/num.h>

struct result duration_checked_sub(
    const struct duration *const self,
    const struct duration *const rhs,
    struct duration *const result)
{
    uint64_t secs;
    if (u64_checked_sub(self->secs, rhs->secs, &secs).reason) {
        return result_new(-ERANGE);
    }

    uint32_t nanos;
    uint64_t sub_secs;

    if (self->nanos >= rhs->nanos) {
        nanos = self->nanos - rhs->nanos;
    } else if (!u64_checked_sub(secs, 1, &sub_secs).reason) {
        secs = sub_secs;
        nanos = self->nanos + NANOS_PER_SEC - rhs->nanos;
    } else {
        return result_new(-ERANGE);
    }

    ASSERT(nanos < NANOS_PER_SEC);

    *result = (struct duration) {
        .secs = secs,
        .nanos = nanos,
    };
    return result_new(0);
}

struct result duration_checked_add(
    const struct duration *self,
    const struct duration *rhs,
    struct duration *result)
{
    uint64_t secs;
    if (u64_checked_add(self->secs, rhs->secs, &secs).reason) {
        return result_new(-ERANGE);
    }

    uint32_t nanos = self->nanos + rhs->nanos;
    if (nanos >= NANOS_PER_SEC) {
        nanos -= NANOS_PER_SEC;

        uint64_t new_secs;
        if (!u64_checked_add(secs, 1, &new_secs).reason) {
            secs = new_secs;
        } else {
            return result_new(-ERANGE);
        }
    }

    ASSERT(nanos < NANOS_PER_SEC);

    *result = (struct duration) {
        .secs = secs,
        .nanos = nanos,
    };
    return result_new(0);
}

struct result
duration_as_millis(const struct duration *const self, uint64_t *const result)
{
    uint64_t millis;
    const struct result res
        = u64_checked_mul(self->secs, MILLIS_PER_SEC, &millis);
    if (res.reason) {
        return res;
    }

    *result = millis + (self->nanos / NANOS_PER_MILLI);
    return result_new(0);
}
