#ifndef HEADER_CARES_STRDUP_H
#define HEADER_CARES_STRDUP_H

#include "ares_setup.h"

#ifndef HAVE_STRDUP
extern char* ares_strdup(const char* s1);
#endif

#endif /* HEADER_CARES_STRDUP_H */
