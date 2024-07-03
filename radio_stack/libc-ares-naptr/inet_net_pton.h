#ifndef HEADER_CARES_INET_NET_PTON_H
#define HEADER_CARES_INET_NET_PTON_H

#ifdef HAVE_INET_PTON
#define ares_inet_pton(x, y, z) inet_pton(x, y, z)
#else
int ares_inet_pton(int af, const char* src, void* dst);
#endif

#ifdef HAVE_INET_NET_PTON
#define ares_inet_net_pton(w, x, y, z) inet_net_pton(w, x, y, z)
#else
int ares_inet_net_pton(int af, const char* src, void* dst, size_t size);
#endif

#endif /* HEADER_CARES_INET_NET_PTON_H */
