#include <stdarg.h>
#include "error_functions.h"
#include "tlpi_hdr.h"
#include "ename.c.inc"						// Defines ename and MAX_ENAME


#ifdef __GNUC__
__attribute__	((__noreturn__))
#endif	// #ifdef __GNUC__

static void terminate(Boolean useExit3)
{
	char	*s = NULL;

	/* Dump core if EF_DUMPCORE environment variable is defined and
	   is a nonempty string: otherwise call exit(3) or _exit(2),
	   depending on the value of 'useExit3'. */
	s = getenv("EF_DUMPCORE");
	if ((s != NULL) && (*s != '\0'))
	{
		abort();
	}
	else if (useExit3)
	{
		exit(EXIT_FAILURE);
	}
	else
	{
		_exit(EXIT_FAILURE);
	}
}

static void outputError(Boolean useErr, int err, Boolean flushStdout, const char* format, va_list ap)
{
#define	BUFF_SIZE			500

	char	buf[BUFF_SIZE];
	char	userMsg[BUFF_SIZE];
	char	errText[BUFF_SIZE];

	memset(buf, 0x00, sizeof(buf));
	memset(userMsg, 0x00, sizeof(userMsg));
	memset(errText, 0x00, sizeof(errText));

	vsnprintf(userMsg, BUFF_SIZE, format, ap);

	if (useErr)
	{
		snprintf(errText, BUFF_SIZE, " [%s %s]", (((err > 0) && (err <= MAX_ENAME)) ? ename[err] : "?UNKNOWN?"), strerror(err));
	}
	else
	{
		snprintf(errText, BUFF_SIZE, ":");
	}

	snprintf(buf, BUFF_SIZE, "ERROR%s %s\n", errText, userMsg);

	if (flushStdout)
	{
		fflush(stdout);				// Flush any pending stdout
	}
	fputs(buf, stderr);
	fflush(stderr);					// In case stderr is not line-buffered
}

void errMsg(const char* format, ...)
{
	va_list		argList;
	int			savedErrno;

	savedErrno = errno;				// In case we change it here

	va_start(argList, format);
	outputError(TRUE, errno, TRUE, format, argList);
	va_end(argList);

	errno = savedErrno;
}

void errExit(const char* format, ...)
{
	va_list		argList;

	va_start(argList, format);
	outputError(TRUE, errno, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);
}

void err_exit(const char* format, ...)
{
	va_list		argList;

	va_start(argList, format);
	outputError(TRUE, errno, FALSE, format, argList);
	va_end(argList);

	terminate(FALSE);
}

void err_exitEN(int errnum, const char* format, ...)
{
	va_list		argList;

	va_start(argList, format);
	outputError(TRUE, errnum, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);
}

void fatal(const char* format, ...)
{
	va_list		argList;

	va_start(argList, format);
	outputError(FALSE, 0, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);
}

void usageErr(const char* format, ...)
{
	va_list		argList;

	fflush(stdout);				// Flush any pending stdout

	fprintf(stderr, "Usage: ");
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);

	fflush(stderr);				// Is case stderr is not line-bufferde
	
	exit(EXIT_FAILURE);
}

void cmdLineErr(const char* format, ...)
{
	va_list		argList;

	fflush(stdout);				// Flush any pending stdout

	fprintf(stderr, "Command-line usage error: ");
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);

	fflush(stderr);				// Is case stderr is not line-bufferde

	exit(EXIT_FAILURE);
}
