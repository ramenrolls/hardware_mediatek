#ifndef HEADER_CARES_STRCASECMP_H
#define HEADER_CARES_STRCASECMP_H

#include "ares_setup.h"

#ifndef HAVE_STRCASECMP
extern int ares_strcasecmp(const char* a, const char* b);
#endif

#ifndef HAVE_STRNCASECMP
extern int ares_strncasecmp(const char* a, const char* b, size_t n);
#endif

#endif /* HEADER_CARES_STRCASECMP_H */
