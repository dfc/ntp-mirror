#include <config.h>
#include "log.h"
#include "sntp-opts.h"

int init = 0;
int filelog = 0;

FILE *log_file;


void log_msg(char *message, int type) {
	if (init) {
		time_t cur_time = time(NULL);
		char *timestamp = ctime(&cur_time);

		fprintf(log_file, "%s: %s\n", timestamp, message);
		fflush(log_file);
	} else {
		syslog(type, "%s", message);
	}
}

void debug_msg(char *message) {
	if(HAVE_OPT(FILELOG)) {
		time_t cur_time = time(NULL);
		char *timestamp = ctime(&cur_time);

		fprintf(stderr, "%s: %s\n", timestamp, message);
	}
	else {
		syslog(LOG_DEBUG
#ifdef LOG_PERROR
			| LOG_PERROR
#endif
		       | LOG_CONS, "%s", message);
	}
}

void init_log(
	const char *logfile
	)
{
	char error_msg[80];

	log_file = fopen(logfile, "a");	
	if (log_file == NULL) {
		filelog = 0;

		snprintf(error_msg, 80, "init_log(): Cannot open logfile %s", logfile);
		debug_msg(error_msg);

		return;
	} else {
		filelog = 1;
		init = 1;
		atexit(cleanup_log);
	}
}

void cleanup_log(void) {
	init = 0;
	fflush(log_file);
	fclose(log_file);
}
