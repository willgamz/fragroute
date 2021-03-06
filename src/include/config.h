/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to empty if the keyword does not work.  */
#undef const

/* Define to `long' if <sys/types.h> doesn't define.  */
#undef off_t

/* Define as the return type of signal handlers (int or void).  */
#undef RETSIGTYPE

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
#undef size_t

/* Define if you have the ANSI C header files.  */
#undef STDC_HEADERS

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#undef TIME_WITH_SYS_TIME

/* Define if your <sys/time.h> declares struct tm.  */
#undef TM_IN_SYS_TIME

/* Define if you have the arc4random function.  */
#undef HAVE_ARC4RANDOM

/* Define if you have the getopt function.  */
#undef HAVE_GETOPT

/* Define if you have the gettimeofday function.  */
#undef HAVE_GETTIMEOFDAY

/* Define if you have the select function.  */
#undef HAVE_SELECT

/* Define if you have the strdup function.  */
#undef HAVE_STRDUP

/* Define if you have the strlcat function.  */
#undef HAVE_STRLCAT

/* Define if you have the strlcpy function.  */
#undef HAVE_STRLCPY

/* Define if you have the <fcntl.h> header file.  */
#undef HAVE_FCNTL_H

/* Define if you have the <inttypes.h> header file.  */
#define HAVE_INTTYPES_H

/* Define if you have the <malloc.h> header file.  */
#undef HAVE_MALLOC_H

/* Define if you have the <stdint.h> header file.  */
#define HAVE_STDINT_H

/* Define if you have the <strings.h> header file.  */
#undef HAVE_STRINGS_H

/* Define if you have the <sys/ioctl.h> header file.  */
#undef HAVE_SYS_IOCTL_H

/* Define if you have the <sys/time.h> header file.  */
#undef HAVE_SYS_TIME_H

/* Define if you have the <unistd.h> header file.  */
#undef HAVE_UNISTD_H

/* Define if you have the <windows.h> header file.  */
#define HAVE_WINDOWS_H 1

/* Define if you have the <winsock.h> header file.  */
#define HAVE_WINSOCK_H 1

/* Define if you have the nsl library (-lnsl).  */
#undef HAVE_LIBNSL

/* Define if you have the socket library (-lsocket).  */
#undef HAVE_LIBSOCKET

/* Define if you have the str library (-lstr).  */
#undef HAVE_LIBSTR

/* Name of package */
#undef PACKAGE

/* Version number of package */
#undef VERSION

/* Define for faster code generation. */
#define WIN32_LEAN_AND_MEAN 1

/* Use MingW32's internal snprintf */
#undef snprintf

/* Use MingW32's internal vsnprintf */
#undef vsnprintf

/* Location of configuration file. */
#undef FRAGROUTE_CONF

/* XXX - for strl* definitions below */
#include <sys/types.h>

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#ifdef HAVE_WINSOCK_H
# include <winsock2.h>
# include <ws2tcpip.h>
#endif

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif


#ifndef HAVE_GETOPT
int	getopt(int argc, char * const *argv, const char *optstring);
#endif

#ifndef HAVE_STRLCAT
size_t  strlcat(char *, const char *, size_t);
#endif

#ifndef HAVE_STRLCPY
size_t  strlcpy(char *, const char *, size_t);
#endif

#pragma once
#define FRAGROUTE_CONF "fragroute.conf";

#if defined(WIN32) || defined(WIN64)
#define strcasecmp _stricmp
#define snprintf _snprintf
#endif /* Def WIN32 or Def WIN64 */
