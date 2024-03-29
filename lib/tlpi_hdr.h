#pragma once

#include <sys/types.h>			// Type definitions used by many programs
#include <stdio.h>				// Stadart I/O functions
#include <stdlib.h>				// prototypes of commonly used library function, plus EXIT_SUCCESS and EXIT_FAILURE constants
#include <unistd.h>				// Prototypes for many system calls
#include <errno.h>				// Declares errno and defines error constants
#include <string.h>				// Commonly used string-handling functions

#include "get_num.h"			// Declares our functions for handling numeric arguments (getInt(), getLong())
#include "error_functions.h"	// Declares our error-handling functions

typedef enum { FALSE, TRUE } Boolean;

#define min(m,n) ((m) < (n)) ? (m) : (n))
#define max(m,n) ((m) > (n)) ? (m) : (n))




