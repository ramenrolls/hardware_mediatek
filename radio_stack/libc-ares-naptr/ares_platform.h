#ifndef HEADER_CARES_PLATFORM_H
#define HEADER_CARES_PLATFORM_H

#include "ares_setup.h"

#if defined(WIN32) && !defined(MSDOS)

typedef enum { WIN_UNKNOWN, WIN_3X, WIN_9X, WIN_NT, WIN_CE } win_platform;

win_platform ares__getplatform(void);

#endif

#if defined(_WIN32_WCE)

struct servent* getservbyport(int port, const char* proto);

#endif

#endif /* HEADER_CARES_PLATFORM_H */
