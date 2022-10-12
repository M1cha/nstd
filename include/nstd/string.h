#ifndef NSTD_STRING_H
#define NSTD_STRING_H

#include <stdbool.h>
#include <stddef.h>

const char *strip_prefix(const char *string, const char *prefix);
size_t string_count_matching_chars(const char *string, char needle);

/// return true if `a` and `b` are equal
///
/// - if only one of them is NULL, they're not equal
/// - if both are NULL, they're equal
/// - otherwise, their contents are checked for equality
bool string_eq(const char *const a, const char *const b);

#endif /* NSTD_STRING_H */
