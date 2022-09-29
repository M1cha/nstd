#ifndef NSTD_MEMORY_H
#define NSTD_MEMORY_H

#define SIZEOF_FIELD(type, field) sizeof((((type *)0)->field))
#define TYPEOF_FIELD(type, field) typeof((((type *)0)->field))
#define ARRAY_SIZE_FIELD(type, field) \
    sizeof((((type *)0)->field)) / sizeof((((type *)0)->field[0]))

#endif /* NSTD_MEMORY_H */
