#ifndef _VSNPRINTF_H
#define _VSNPRINTF_H

#include <stddef.h>
#include <stdarg.h>

int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);

#endif

