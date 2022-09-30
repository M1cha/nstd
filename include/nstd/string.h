#ifndef NSTD_STRING_H
#define NSTD_STRING_H

#include <stddef.h>

const char *strip_prefix(const char *string, const char *prefix);
size_t string_count_matching_chars(const char *string, char needle);

#endif /* NSTD_STRING_H */
