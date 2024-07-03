#ifndef HEADER_CARES_LIBRARY_INIT_H
#define HEADER_CARES_LIBRARY_INIT_H

#include "ares_setup.h"

#ifdef USE_WINSOCK

#include <iphlpapi.h>
#include <ares_iphlpapi.h>

typedef DWORD(WINAPI* fpGetNetworkParams_t)(FIXED_INFO*, DWORD*);
typedef BOOLEAN(APIENTRY* fpSystemFunction036_t)(void*, ULONG);
typedef ULONG(WINAPI* fpGetAdaptersAddresses_t)(ULONG, ULONG, void*, IP_ADAPTER_ADDRESSES*, ULONG*);

/* Forward-declaration of variables defined in ares_library_init.c */
/* that are global and unique instances for whole c-ares library.  */

extern fpGetNetworkParams_t ares_fpGetNetworkParams;
extern fpSystemFunction036_t ares_fpSystemFunction036;
extern fpGetAdaptersAddresses_t ares_fpGetAdaptersAddresses;

#endif /* USE_WINSOCK */

#endif /* HEADER_CARES_LIBRARY_INIT_H */
