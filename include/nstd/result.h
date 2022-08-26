#ifndef NSTD_RESULT_H
#define NSTD_RESULT_H

struct result {
    /// 0 if successful, negative errno value on failure
    ///
    /// Even though unspecified, any positive value must be treated as an error
    /// as well. Do not assume they're positive errno values though.
    int reason;
};

static inline struct result result_new(const int reason)
{
    return (struct result) {
        .reason = reason,
    };
}

#endif /* NSTD_RESULT_H */
