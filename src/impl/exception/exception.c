#include <windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "exception.h"

void exception_handler(void) {
    CRASHF_DEBUG("Exception handler tripped. Possible segfault detected.");
}

void crashf(const char *fmt, ...) {
    // Don't print another crash message
    static bool printed = false;
    if(!printed) {
        fprintf(stderr, "FATAL ERROR! ");

        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);

        fputs("\n", stderr);

        printed = true;
    }
    ExitProcess(197);
}
