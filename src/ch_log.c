/**
 * Copyright (C) Tadas Vilkeliskis
 */
#include <stdarg.h>
#include <stdio.h>
#include "ch_log.h"


static const char *error_levels[] = {
    NULL,
    "error",
    "info",
    "debug"
};


void ch_log_write(unsigned int level, const char *fmt, ...)
{
    int written;
    char log_buffer[8192];
    char *p;
    va_list vargs;

    written = snprintf(log_buffer, sizeof(log_buffer), "chimp: [%s] ", error_levels[level]);
    p = log_buffer + written;
    va_start(vargs, fmt);
    written = vsnprintf(p, sizeof(log_buffer) - written, fmt, vargs);
    va_end(vargs);
    p += written;
    *p++ = '\n';
    *p = '\0';

    fprintf(stdout, "%s", log_buffer);
}
