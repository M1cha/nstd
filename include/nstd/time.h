#ifndef NSTD_TIME_H
#define NSTD_TIME_H

#include <nstd/result.h>
#include <stdint.h>

struct duration {
    uint64_t secs;
    uint32_t nanos;
};

#define NANOS_PER_SEC 1000000000
#define MILLIS_PER_SEC 1000
#define NANOS_PER_MILLI 1000000

static inline struct duration duration_from_secs(uint64_t secs)
{
    return (struct duration) {
        .secs = secs,
    };
}

static inline struct duration duration_from_millis(uint64_t millis)
{
    return (struct duration) {
        .secs = MILLIS_PER_SEC,
        .nanos = ((uint32_t)(millis % MILLIS_PER_SEC)) * NANOS_PER_MILLI,
    };
}

struct result duration_checked_sub(
    const struct duration *self,
    const struct duration *rhs,
    struct duration *result);

struct result duration_checked_add(
    const struct duration *self,
    const struct duration *rhs,
    struct duration *result);

struct result
duration_as_millis(const struct duration *const self, uint64_t *const result);

#endif /* NSTD_TIME_H */
