#ifndef HEADER_CARES_NOWARN_H
#define HEADER_CARES_NOWARN_H

int aresx_uztosi(size_t uznum);

int aresx_sltosi(long slnum);

int aresx_sztosi(ssize_t sznum);

unsigned int aresx_sztoui(ssize_t sznum);

#if defined(__INTEL_COMPILER) && defined(__unix__)

int aresx_FD_ISSET(int fd, fd_set* fdset);

void aresx_FD_SET(int fd, fd_set* fdset);

void aresx_FD_ZERO(fd_set* fdset);

unsigned short aresx_htons(unsigned short usnum);

unsigned short aresx_ntohs(unsigned short usnum);

#ifndef BUILDING_ARES_NOWARN_C
#undef FD_ISSET
#define FD_ISSET(a, b) aresx_FD_ISSET((a), (b))
#undef FD_SET
#define FD_SET(a, b) aresx_FD_SET((a), (b))
#undef FD_ZERO
#define FD_ZERO(a) aresx_FD_ZERO((a))
#undef htons
#define htons(a) aresx_htons((a))
#undef ntohs
#define ntohs(a) aresx_ntohs((a))
#endif

#endif /* __INTEL_COMPILER && __unix__ */

#endif /* HEADER_CARES_NOWARN_H */
