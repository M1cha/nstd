#include <nstd/string.h>

#include <stddef.h>
#include <string.h>

const char *strip_prefix(const char *string, const char *prefix)
{
    if (!*prefix) {
        // we don't support empty prefixes
        return NULL;
    }

    for (; *string && *prefix; string += 1, prefix += 1) {
        if (*string != *prefix) {
            // they're different
            return NULL;
        }
    }

    if (*prefix) {
        // the prefix is longer than the string
        return NULL;
    }

    return string;
}

size_t string_count_matching_chars(const char *string, char needle)
{
    size_t count = 0;

    for (; *string; string += 1) {
        if (*string == needle) {
            count += 1;
        }
    }

    return count;
}

bool string_eq(const char *const a, const char *const b)
{
    // both are NULL, treat them as being equal
    if (a == NULL && b == NULL) {
        return true;
    }

    // only one of them is NULL
    if (a == NULL || b == NULL) {
        return false;
    }

    return !strcmp(a, b);
}
