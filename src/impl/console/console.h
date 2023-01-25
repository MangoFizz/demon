#ifndef DEMON__IMPL_CONSOLE_H
#define DEMON__IMPL_CONSOLE_H

/**
 * Set console prompt display parameters. This is run on startup.
 */
void set_console_prompt_display_params(void);

/**
 * Print a formatted line of text to the console.
 *
 * @param color (optional)
 * @param fmt   format
 */
void console_printf(const struct ColorARGB *color, const char *fmt, ...);

/**
 * Print a red formatted line of text to the console.
 *
 * The syntax is the same as console_printf except no color is passed.
 */
#define console_printf_debug_err(...) { ColorARGB dbg_err_color_print = { 1.0, 1.0, 0.1, 0.1 }; console_printf(&dbg_err_color_print, __VA_ARGS__); }

#ifndef DEMON_ENABLE_DEBUG_MESSAGES
#undef console_printf_debug_err
#define console_printf_debug_err(...) {}
#endif

#endif
