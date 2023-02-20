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

/**
 * Main function
 */
void game_main(HMODULE module_handle, uint32_t zero, const char *args, uint32_t one);

/**
 * Entry point for ASM
 */
void game_main_entry(void);

#endif
