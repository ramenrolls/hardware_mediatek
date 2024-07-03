#ifndef HEADER_CARES_WRITEV_H
#define HEADER_CARES_WRITEV_H

#include "ares_setup.h"
#include "ares.h"

#ifndef HAVE_WRITEV

/* Structure for scatter/gather I/O. */
struct iovec {
    void* iov_base; /* Pointer to data. */
    size_t iov_len; /* Length of data.  */
};

extern ssize_t ares_writev(ares_socket_t s, const struct iovec* iov, int iovcnt);

#endif

#endif /* HEADER_CARES_WRITEV_H */
