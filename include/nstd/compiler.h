#ifndef NSTD_COMPILER_H
#define NSTD_COMPILER_H

#define NSTD_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#define NSTD_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")

#define NSTD_DIAGNOSTIC_IGNORED_GCC_(directive) _Pragma(#directive)
#define NSTD_DIAGNOSTIC_IGNORED_GCC(warning) \
    NSTD_DIAGNOSTIC_IGNORED_GCC_(GCC diagnostic ignored warning)

#endif /* NSTD_COMPILER_H */
