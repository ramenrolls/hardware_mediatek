/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* #if !defined(lint)
 * static char sccsid[] = "@(#)getopt.c 8.2 (Berkeley) 4/2/94";
 * #endif
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ares_getopt.h"

int opterr = 1,      /* if error message should be printed */
        optind = 1;  /* index into parent argv vector */
int optopt = 0;      /* character checked for validity */
static int optreset; /* reset getopt */
char* optarg;        /* argument associated with option */

#define BADCH (int)'?'
#define BADARG (int)':'
#define EMSG (char*)""

/*
 * ares_getopt --
 *    Parse argc/argv argument vector.
 */
int ares_getopt(int nargc, char* const nargv[], const char* ostr) {
    static char* place = EMSG; /* option letter processing */
    char* oli;                 /* option letter list index */

    if (optreset || !*place) { /* update scanning pointer */
        optreset = 0;
        if (optind >= nargc || *(place = nargv[optind]) != '-') {
            place = EMSG;
            return (EOF);
        }
        if (place[1] && *++place == '-') { /* found "--" */
            ++optind;
            place = EMSG;
            return (EOF);
        }
    } /* option letter okay? */
    if ((optopt = (int)*place++) == (int)':' || (oli = strchr(ostr, optopt)) == NULL) {
        /*
         * if the user didn't specify '-' as an option,
         * assume it means EOF.
         */
        if (optopt == (int)'-') return (EOF);
        if (!*place) ++optind;
        if (opterr && *ostr != ':')
            (void)fprintf(stderr, "%s: illegal option -- %c\n", __FILE__, optopt);
        return (BADCH);
    }
    if (*++oli != ':') { /* don't need argument */
        optarg = NULL;
        if (!*place) ++optind;
    } else {        /* need an argument */
        if (*place) /* no white space */
            optarg = place;
        else if (nargc <= ++optind) { /* no arg */
            place = EMSG;
            if (*ostr == ':') return (BADARG);
            if (opterr)
                (void)fprintf(stderr, "%s: option requires an argument -- %c\n", __FILE__, optopt);
            return (BADCH);
        } else /* white space */
            optarg = nargv[optind];
        place = EMSG;
        ++optind;
    }
    return (optopt); /* dump back option letter */
}
