#include <nstd/string.h>

#include <stddef.h>

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
