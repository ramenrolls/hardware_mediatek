#ifndef ARES_GETOPT_H
#define ARES_GETOPT_H

int ares_getopt(int nargc, char* const nargv[], const char* ostr);

#undef optarg
#undef optind
#undef opterr
#undef optopt
#undef optreset

#define optarg ares_optarg
#define optind ares_optind
#define opterr ares_opterr
#define optopt ares_optopt
#define optreset ares_optreset

extern char* optarg;
extern int optind;
extern int opterr;
extern int optopt;

#endif /* ARES_GETOPT_H */
