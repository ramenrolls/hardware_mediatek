#ifndef __ARES_BITNCMP_H
#define __ARES_BITNCMP_H

#ifndef HAVE_BITNCMP
int ares_bitncmp(const void* l, const void* r, int n);
#else
#define ares_bitncmp(x, y, z) bitncmp(x, y, z)
#endif

#endif /* __ARES_BITNCMP_H */
