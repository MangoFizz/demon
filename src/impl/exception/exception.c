#include <windows.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void exception_handler(void) {
    static bool printed = false;

    if(!printed) {
        fputs("SEGFAULT!! WE'RE GOING DOWN!\n", stderr);
        printed = true;
    }

    ExitProcess(197);
}
