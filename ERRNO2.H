/*  errno2.h

    Defines the system error variable errno and the error
    numbers set by system calls. An updated version of ERRNO.H
	contains additional error codes respective to LIB_MGMT.CPP

    Copyright (c) 2016 by Menahi Shayan
    Some Rights Reserved.
*/

#ifndef __ERRNO2_H
#define __ERRNO2_H

#if !defined( __ERRNO_H )
#include <errno.h>
#endif

/*  LIB_MGMT Error Codes */
#define ENOFNSH 37    	/* Unfinished part of program */
#define ETIMOUT 38    	/* Operation timed out */
#define ENOLOAD 39    	/* No file loaded */
#define ENODATE 40		/* Invalid date */
#define ENOREC 41		/* Record not found */
#define EREXIST 42		/* Record already exists */
#define EISSUED 43		/* Modifying disabled */
#define ENOAVAL 44		/* Out of stock */
#define ELIDATE 45		/* Impractical date */

#endif
