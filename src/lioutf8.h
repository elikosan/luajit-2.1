#ifndef lioutf8_h
#define lioutf8_h

#include <stddef.h>
#include <stdio.h>

#include "lua.h"

wchar_t* cc2wc(const char *c);
char* wc2cc(const wchar_t* wc);

FILE* utf8_fopen(const char *filename, const char *mode);
FILE* utf8_popen(const char *filename, const char *mode);
// callers are responsible for freeing the returned char*
char* utf8_getenv(const char* varname);

#endif
