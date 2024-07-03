#ifndef HEADER_CARES_GETENV_H
#define HEADER_CARES_GETENV_H

#include "ares_setup.h"

#ifndef HAVE_GETENV
extern char* ares_getenv(const char* name);
#endif

#endif /* HEADER_CARES_GETENV_H */
