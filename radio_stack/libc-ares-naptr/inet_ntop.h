#ifndef __ARES_INET_NTOP_H
#define __ARES_INET_NTOP_H

#ifdef HAVE_INET_NTOP
#define ares_inet_ntop(w, x, y, z) inet_ntop(w, x, y, z)
#else
const char* ares_inet_ntop(int af, const void* src, char* dst, size_t size);
#endif

#endif /* __ARES_INET_NTOP_H */
