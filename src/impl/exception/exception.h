#ifndef DEMON__IMPL_EXCEPTION_EXCEPTION_H
#define DEMON__IMPL_EXCEPTION_EXCEPTION_H

/**
 * Terminate the process with a format.
 *
 * @param fmt format
 */
void crashf(const char *fmt, ...);

#define STR2(s) # s
#define STR(s) STR2(s)

#define CRASHF_DEBUG(...) crashf(__FILE__ ":" STR(__LINE__) ": " __VA_ARGS__)

#endif
