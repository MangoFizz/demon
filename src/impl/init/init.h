#ifndef DEMON__IMPL_INIT_INIT_H
#define DEMON__IMPL_INIT_INIT_H

/**
 * Get the argument value if it exists.
 *
 * @param argument argument to check
 * @param value    (out) if not NULL, set to a pointer of the value of the argument, or NULL if none given
 *
 * @return true if found, false if not
 */
bool get_exe_argument_value(const char *argument, const char **value);

#endif
