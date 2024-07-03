#ifndef __CARES_RULES_H
#define __CARES_RULES_H

/*
 * Verify that some macros are actually defined.
 */

#ifndef CARES_SIZEOF_LONG
#error "CARES_SIZEOF_LONG definition is missing!"
Error Compilation_aborted_CARES_SIZEOF_LONG_is_missing
#endif

#ifndef CARES_TYPEOF_ARES_SOCKLEN_T
#error "CARES_TYPEOF_ARES_SOCKLEN_T definition is missing!"
        Error Compilation_aborted_CARES_TYPEOF_ARES_SOCKLEN_T_is_missing
#endif

#ifndef CARES_SIZEOF_ARES_SOCKLEN_T
#error "CARES_SIZEOF_ARES_SOCKLEN_T definition is missing!"
                Error Compilation_aborted_CARES_SIZEOF_ARES_SOCKLEN_T_is_missing
#endif

/*
 * Macros private to this header file.
 */

#define CareschkszEQ(t, s) sizeof(t) == s ? 1 : -1

#define CareschkszGE(t1, t2) sizeof(t1) >= sizeof(t2) ? 1 : -1

        /*
         * Verify that the size previously defined and expected for long
         * is the same as the one reported by sizeof() at compile time.
         */

        typedef char __cares_rule_01__[CareschkszEQ(long, CARES_SIZEOF_LONG)];

/*
 * Verify that the size previously defined and expected for
 * ares_socklen_t is actually the the same as the one reported
 * by sizeof() at compile time.
 */

typedef char __cares_rule_02__[CareschkszEQ(ares_socklen_t, CARES_SIZEOF_ARES_SOCKLEN_T)];

/*
 * Verify at compile time that the size of ares_socklen_t as reported
 * by sizeof() is greater or equal than the one reported for int for
 * the current compilation.
 */

typedef char __cares_rule_03__[CareschkszGE(ares_socklen_t, int)];

/* ================================================================ */
/*          EXTERNALLY AND INTERNALLY VISIBLE DEFINITIONS           */
/* ================================================================ */

/*
 * Get rid of macros private to this header file.
 */

#undef CareschkszEQ
#undef CareschkszGE

/*
 * Get rid of macros not intended to exist beyond this point.
 */

#undef CARES_PULL_WS2TCPIP_H
#undef CARES_PULL_SYS_TYPES_H
#undef CARES_PULL_SYS_SOCKET_H

#undef CARES_TYPEOF_ARES_SOCKLEN_T

#endif /* __CARES_RULES_H */
