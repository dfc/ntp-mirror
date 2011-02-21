#include <config.h>

#include "log.h"

char *progname;		/* for msyslog use too */

static void cleanup_log(void);

void
sntp_init_logging(
	const char *prog
	)
{
	msyslog_term = TRUE;
	init_logging(prog, 0, NULL, FALSE);
}


void
open_logfile(
	const char *logfile
	)
{
	change_logfile(logfile, Version);
	atexit(cleanup_log);
}


static void
cleanup_log(void)
{
	syslogit = TRUE;
	fflush(syslog_file);
	fclose(syslog_file);
	syslog_file = NULL;
}
