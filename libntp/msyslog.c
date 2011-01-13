/*
 * msyslog - either send a message to the terminal or print it on
 *	     the standard output.
 *
 * Converted to use varargs, much better ... jks
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/types.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <stdio.h>

#include "ntp.h"
#include "ntp_string.h"
#include "ntp_syslog.h"

#ifdef SYS_WINNT
# include <stdarg.h>
# include "..\ports\winnt\libntp\messages.h"
#endif


int	syslogit = 1;
int	msyslog_term = FALSE;	/* duplicate to stdout/err */
FILE *	syslog_file;
char *	syslog_fname;
char *	syslog_abs_fname;

u_long ntp_syslogmask =  ~(u_long)0;	/* libntp default is all lit */

extern	char *	progname;

/* Declare the local functions */
void	addto_syslog	(int, const char *);
void	format_errmsg	(char *, size_t, const char *, int);


/*
 * errno_to_str() - a thread-safe strerror() replacement.
 *		    Hides the varied signatures of strerror_r().
 *		    For Windows, we have:
 *			#define errno_to_str isc_strerror
 */
#ifndef errno_to_str
void
errno_to_str(
	int	err,
	char *	buf,
	size_t	bufsiz
	)
{
	char *	pstatic;

# ifdef STRERROR_R_CHAR_P
	/*
	 * For older GNU strerror_r, the return value either points to
	 * buf, or to static storage.  We want the result always in buf
	 */
	pstatic = strerror_r(err, buf, bufsiz);
# else
	pstatic = strerror(err);
# endif
	if (pstatic != buf)
		strncpy(buf, pstatic, bufsiz);
}
#endif	/* errno_to_str */


/*
 * addto_syslog()
 * This routine adds the contents of a buffer to the syslog or an
 * application-specific logfile.
 */
void
addto_syslog(
	int		level,
	const char *	msg
	)
{
	static char *	prevcall_progname;
	static char *	prog;
	const char	nl[] = "\n";
	const char	empty[] = "";
	FILE *		term_file;
	int		log_to_term;
	int		log_to_file;
	const char *	nl_or_empty;
	const char *	human_time;

	/* setup program basename static var prog if needed */
	if (progname != prevcall_progname) {
		prevcall_progname = progname;
		prog = strrchr(progname, DIR_SEP);
		if (prog != NULL)
			prog++;
		else
			prog = progname;
	}

	log_to_term = msyslog_term;
	log_to_file = FALSE;
#if !defined(VMS) && !defined(SYS_VXWORKS)
	if (syslogit)
		syslog(level, "%s", msg);
	else
#endif
		if (syslog_file != NULL)
			log_to_file = TRUE;
		else
			log_to_term = TRUE;
#if DEBUG
	if (debug > 0)
		log_to_term = TRUE;
#endif
	if (!(log_to_file || log_to_term))
		return;

	/* syslog() adds the timestamp, name, and pid */
	human_time = humanlogtime();

	/* syslog() adds trailing \n if not present */
	if ('\n' != msg[strlen(msg) - 1])
		nl_or_empty = nl;
	else
		nl_or_empty = empty;

	if (log_to_term) {
		term_file = (level <= LOG_ERR)
				? stderr
				: stdout;
		fprintf(term_file, "%s %s[%d]: %s%s", human_time, prog,
			(int)getpid(), msg, nl_or_empty);
		fflush(term_file);
	}

	if (log_to_file) {
		fprintf(syslog_file, "%s %s[%d]: %s%s", human_time,
			prog, (int)getpid(), msg, nl_or_empty);
		fflush(syslog_file);
	}
}


void
format_errmsg(
	char *		nfmt,
	size_t		lennfmt,
	const char *	fmt,
	int		errval
	)
{
	char errmsg[256];
	char c;
	char *n;
	const char *f;
	size_t len;

	n = nfmt;
	f = fmt;
	while ((c = *f++) != '\0' && n < (nfmt + lennfmt - 1)) {
		if (c != '%') {
			*n++ = c;
			continue;
		}
		if ((c = *f++) != 'm') {
			*n++ = '%';
			if ('\0' == c)
				break;
			*n++ = c;
			continue;
		}
		errno_to_str(errval, errmsg, sizeof(errmsg));
		len = strlen(errmsg);

		/* Make sure we have enough space for the error message */
		if ((n + len) < (nfmt + lennfmt - 1)) {
			memcpy(n, errmsg, len);
			n += len;
		}
	}
	*n = '\0';
}


int
mvsnprintf(
	char *		buf,
	size_t		bufsiz,
	const char *	fmt,
	va_list		ap
	)
{
	char	nfmt[256];
	int	errval;
	int	rc;

	/*
	 * Save the error value as soon as possible
	 */
#ifdef SYS_WINNT
	errval = GetLastError();
	if (NO_ERROR == errval)
#endif /* SYS_WINNT */
		errval = errno;

	format_errmsg(nfmt, sizeof(nfmt), fmt, errval);

	rc = vsnprintf(buf, bufsiz, nfmt, ap);
	/* terminate buf, as MS vsnprintf() does not when truncating */
	buf[bufsiz - 1] = '\0';

	return rc;
}


int
msnprintf(
	char *		buf,
	size_t		bufsiz,
	const char *	fmt,
	...
	)
{
	va_list	ap;
	size_t	rc;

	va_start(ap, fmt);
	rc = mvsnprintf(buf, bufsiz, fmt, ap);
	va_end(ap);

	return rc;
}


void
msyslog(
	int		level,
	const char *	fmt,
	...
	)
{
	char	buf[1024];
	va_list	ap;

	va_start(ap, fmt);
	mvsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	addto_syslog(level, buf);
}
