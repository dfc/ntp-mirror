/*
 * ntpd.c - main program for the fixed point NTP daemon
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ntp_machine.h"
#include "ntpd.h"
#include "ntp_io.h"
#include "ntp_stdlib.h"
#include <ntp_random.h>

#include "ntp_syslog.h"
#include "ntp_assert.h"
#include "isc/error.h"
#include "isc/strerror.h"
#include "isc/formatcheck.h"

#ifdef SIM
# include "ntpsim.h"
#endif

#include "ntpd-opts.h"

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#include <stdio.h>
#ifdef HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif
#ifdef HAVE_SYS_SIGNAL_H
# include <sys/signal.h>
#else
# include <signal.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>
#endif /* HAVE_SYS_IOCTL_H */
#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif /* HAVE_SYS_RESOURCE_H */
#if defined(HAVE_RTPRIO)
# ifdef HAVE_SYS_RESOURCE_H
#  include <sys/resource.h>
# endif
# ifdef HAVE_SYS_LOCK_H
#  include <sys/lock.h>
# endif
# include <sys/rtprio.h>
#else
# ifdef HAVE_PLOCK
#  ifdef HAVE_SYS_LOCK_H
#	include <sys/lock.h>
#  endif
# endif
#endif
#if defined(HAVE_SCHED_SETSCHEDULER)
# ifdef HAVE_SCHED_H
#  include <sched.h>
# else
#  ifdef HAVE_SYS_SCHED_H
#   include <sys/sched.h>
#  endif
# endif
#endif
#if defined(HAVE_SYS_MMAN_H)
# include <sys/mman.h>
#endif

#ifdef HAVE_TERMIOS_H
# include <termios.h>
#endif

#ifdef SYS_DOMAINOS
# include <apollo/base.h>
#endif /* SYS_DOMAINOS */

#include "recvbuff.h"
#include "ntp_cmdargs.h"

#if 0				/* HMS: I don't think we need this. 961223 */
#ifdef LOCK_PROCESS
# ifdef SYS_SOLARIS
#  include <sys/mman.h>
# else
#  include <sys/lock.h>
# endif
#endif
#endif

#ifdef _AIX
# include <ulimit.h>
#endif /* _AIX */

#ifdef SCO5_CLOCK
# include <sys/ci/ciioctl.h>
#endif

#ifdef HAVE_DROPROOT
# include <ctype.h>
# include <grp.h>
# include <pwd.h>
#ifdef HAVE_LINUX_CAPABILITIES
# include <sys/capability.h>
# include <sys/prctl.h>
#endif
#endif

/*
 * Signals we catch for debugging.	If not debugging we ignore them.
 */
#define MOREDEBUGSIG	SIGUSR1
#define LESSDEBUGSIG	SIGUSR2

/*
 * Signals which terminate us gracefully.
 */
#ifndef SYS_WINNT
# define SIGDIE1	SIGHUP
# define SIGDIE3	SIGQUIT
# define SIGDIE2	SIGINT
# define SIGDIE4	SIGTERM
#endif /* SYS_WINNT */

#ifdef HAVE_DNSREGISTRATION
#include <dns_sd.h>
DNSServiceRef mdns;
#endif

#ifdef HAVE_SETPGRP_0
#define ntp_setpgrp(x, y)	setpgrp()
#else
#define ntp_setpgrp(x, y)	setpgrp(x, y)
#endif

/*
 * Scheduling priority we run at
 */
#define NTPD_PRIO	(-12)

int priority_done = 2;		/* 0 - Set priority */
				/* 1 - priority is OK where it is */
				/* 2 - Don't set priority */
				/* 1 and 2 are pretty much the same */

#ifdef DEBUG
/*
 * Debugging flag
 */
volatile int debug = 0;		/* No debugging by default */
#endif

int	listen_to_virtual_ips = 1;

/*
 * No-fork flag.  If set, we do not become a background daemon.
 */
int nofork = 0;			/* Fork by default */

#ifdef HAVE_DNSREGISTRATION
/*
 * mDNS registration flag. If set, we attempt to register with the mDNS system, but only
 * after we have synched the first time. If the attempt fails, then try again once per 
 * minute for up to 5 times. After all, we may be starting before mDNS.
 */
int mdnsreg = 1;
int mdnstries = 5;
#endif  /* HAVE_DNSREGISTRATION */

#ifdef HAVE_DROPROOT
int droproot;
int root_dropped;
char *user = NULL;		/* User to switch to */
char *group = NULL;		/* group to switch to */
const char *chrootdir = NULL;	/* directory to chroot to */
int sw_uid;
int sw_gid;
char *endp;
struct group *gr;
struct passwd *pw;
#endif /* HAVE_DROPROOT */

#ifdef HAVE_WORKING_FORK
int	waitsync_fd_to_close = -1;	/* -w/--wait-sync */
#endif

/*
 * Initializing flag.  All async routines watch this and only do their
 * thing when it is clear.
 */
int initializing;

/*
 * Version declaration
 */
extern const char *Version;

char const *progname;

int was_alarmed;

#ifdef DECL_SYSCALL
/*
 * We put this here, since the argument profile is syscall-specific
 */
extern int syscall	(int, ...);
#endif /* DECL_SYSCALL */


#if !defined(SIM) && defined(SIGDIE2)
static	RETSIGTYPE	finish		(int);
#endif

#if !defined(SIM) && defined(HAVE_WORKING_FORK)
static int	wait_child_sync_if	(int, long);
#endif

#if !defined(SIM) && !defined(SYS_WINNT)
# ifdef	DEBUG
static	RETSIGTYPE	moredebug	(int);
static	RETSIGTYPE	lessdebug	(int);
# else	/* !DEBUG follows */
static	RETSIGTYPE	no_debug	(int);
# endif	/* !DEBUG */
#endif	/* !SIM && !SYS_WINNT */

#ifndef SIM
int		ntpdmain		(int, char **);
static void	set_process_priority	(void);
static void	assertion_failed	(const char *, int,
					 isc_assertiontype_t,
					 const char *);
static void	library_fatal_error	(const char *, int, 
					 const char *, va_list)
					ISC_FORMAT_PRINTF(3, 0);
static void	library_unexpected_error(const char *, int,
					 const char *, va_list)
					ISC_FORMAT_PRINTF(3, 0);
#endif	/* !SIM */
void		init_logging		(char const *, int);
void		setup_logfile		(int);


/*
 * Initialize the logging
 *
 * Called once per process, including forked children.
 */
void
init_logging(
	const char *name,
	int log_version
	)
{
	const char *cp;

	/*
	 * ntpd defaults to only logging sync-category events, when
	 * NLOG() is used to conditionalize.  Other libntp clients
	 * leave it alone so that all NLOG() conditionals will fire.
	 * This presumes all bits lit in ntp_syslogmask can't be
	 * configured via logconfig and all lit is thereby a sentinel
	 * that ntp_syslogmask is still at its default from libntp,
	 * keeping in mind this function is called in forked children
	 * where it has already been called in the parent earlier.
	 */
	if (~(u_long)0 == ntp_syslogmask)
		ntp_syslogmask = NLOG_SYNCMASK; /* set more via logconfig */

	/*
	 * Logging.  This may actually work on the gizmo board.  Find a name
	 * to log with by using the basename
	 */
	cp = strrchr(name, '/');
	if (cp == 0)
		cp = name;
	else
		cp++;
	progname = cp;

#if !defined(VMS)

# ifndef LOG_DAEMON
	openlog(progname, LOG_PID);
# else /* LOG_DAEMON */

#  ifndef LOG_NTP
#	define	LOG_NTP LOG_DAEMON
#  endif
	openlog(progname, LOG_PID | LOG_NDELAY, LOG_NTP);
#  ifdef DEBUG
	if (debug)
		setlogmask(LOG_UPTO(LOG_DEBUG));
	else
#  endif /* DEBUG */
		setlogmask(LOG_UPTO(LOG_DEBUG)); /* @@@ was INFO */
# endif /* LOG_DAEMON */
#endif	/* !VMS */

	if (log_version)
		msyslog(LOG_NOTICE, "%s", Version);
}


/*
 * change_logfile()
 *
 * Used to change from syslog to a logfile, or from one logfile to
 * another, and to reopen logfiles after forking.  On systems where
 * ntpd forks, deals with converting relative logfile paths to
 * absolute (root-based) because we reopen logfiles after the current
 * directory has changed.
 */
int
change_logfile(
	const char *fname,
	int log_version
	)
{
	FILE *		new_file;
	const char *	log_fname;
	char *		abs_fname;
#if !defined(SYS_WINNT) && !defined(SYS_VXWORKS) && !defined(VMS)
	char		curdir[512];
	size_t		cd_octets;
	size_t		octets;
#endif	/* POSIX */

	NTP_REQUIRE(fname != NULL);
	log_fname = fname;

	/*
	 * In a forked child of a parent which is logging to a file
	 * instead of syslog, syslog_file will be NULL and both
	 * syslog_fname and syslog_abs_fname will be non-NULL.
	 * If we are given the same filename previously opened
	 * and it's still open, there's nothing to do here.
	 */
	if (syslog_file != NULL && syslog_fname != NULL &&
	    (log_fname == syslog_fname ||
	     0 == strcmp(syslog_fname, log_fname)))
		return 0;

	if (0 == strcmp(log_fname, "stderr")) {
		new_file = stderr;
		abs_fname = estrdup(log_fname);
	} else if (0 == strcmp(log_fname, "stdout")) {
		new_file = stdout;
		abs_fname = estrdup(log_fname);
	} else {
		if (syslog_fname != NULL &&
		    0 == strcmp(log_fname, syslog_fname))
			log_fname = syslog_abs_fname;
#if !defined(SYS_WINNT) && !defined(SYS_VXWORKS) && !defined(VMS)
		if (log_fname != syslog_abs_fname &&
		    DIR_SEP != log_fname[0] &&
		    0 != strcmp(log_fname, "stderr") &&
		    0 != strcmp(log_fname, "stdout") &&
		    NULL != getcwd(curdir, sizeof(curdir))) {
			cd_octets = strlen(curdir);
			/* trim any trailing '/' */
			if (cd_octets > 1 &&
			    DIR_SEP == curdir[cd_octets - 1])
				cd_octets--;
			octets = cd_octets;
			octets += 1;	/* separator '/' */
			octets += strlen(log_fname);
			octets += 1;	/* NUL terminator */
			abs_fname = emalloc(octets);
			snprintf(abs_fname, octets, "%.*s%c%s",
				 (int)cd_octets, curdir, DIR_SEP,
				 log_fname);
		} else
#endif
			abs_fname = estrdup(log_fname);
		DPRINTF(1, ("attempting to open log %s", abs_fname));
		new_file = fopen(abs_fname, "a");
	}

	if (NULL == new_file) {
		free(abs_fname);
		return -1;
	}

	/* leave a pointer in the old log */
	if (log_fname != syslog_abs_fname)
		msyslog(LOG_NOTICE, "switching logging to file %s",
			abs_fname);

	if (syslog_file != NULL &&
	    syslog_file != stderr && syslog_file != stdout &&
	    fileno(syslog_file) != fileno(new_file))
		fclose(syslog_file);
	syslog_file = new_file;
	if (log_fname != syslog_abs_fname) {
		if (syslog_abs_fname != NULL &&
		    syslog_abs_fname != syslog_fname)
			free(syslog_abs_fname);
		if (syslog_fname != NULL)
			free(syslog_fname);
		syslog_fname = estrdup(log_fname);
		syslog_abs_fname = abs_fname;
	}
	syslogit = 0;
	if (log_version)
		msyslog(LOG_NOTICE, "%s", Version);

	return 0;
}


/*
 * setup_logfile()
 *
 * Redirect logging to a file if requested with -l/--logfile or via
 * ntp.conf logfile directive.
 *
 * This routine is invoked three different times in the sequence of a
 * typical daemon ntpd with DNS lookups to do.  First it is invoked in
 * the original ntpd process, then again in the daemon after closing
 * all descriptors.  In both of those cases, ntp.conf has not been
 * processed, so only -l/--logfile will trigger logfile redirection in
 * those invocations.  Finally, if DNS names are resolved, the worker
 * child invokes this routine after its fork and close of all
 * descriptors.  In this case, ntp.conf has been processed and any
 * "logfile" directive needs to be honored in the child as well.
 */
void
setup_logfile(
	int log_version
	)
{
	if (NULL == syslog_fname && HAVE_OPT(LOGFILE)) {
		if (-1 == change_logfile(OPT_ARG(LOGFILE), log_version))
			msyslog(LOG_ERR, "Cannot open log file %s, %m",
				OPT_ARG(LOGFILE));
	} else if (NULL != syslog_fname) {
		if (-1 == change_logfile(syslog_fname, log_version))
			msyslog(LOG_ERR, "Cannot reopen log file %s, %m",
				syslog_fname);
	}
}


void
parse_cmdline_opts(
	int *	pargc,
	char ***pargv
	)
{
	static int	parsed;
	static int	optct;

	if (!parsed)
		optct = optionProcess(&ntpdOptions, *pargc, *pargv);

	parsed = 1;
	
	*pargc -= optct;
	*pargv += optct;
}


#ifdef SIM
int
main(
	int argc,
	char *argv[]
	)
{
	parse_cmdline_opts(&argc, &argv);
#ifdef DEBUG
	debug = DESC(DEBUG_LEVEL).optOccCt;
	DPRINTF(1, ("%s\n", Version));
#endif

	return ntpsim(argc, argv);
}
#else	/* !SIM follows */
#ifdef NO_MAIN_ALLOWED
CALL(ntpd,"ntpd",ntpdmain);
#else	/* !NO_MAIN_ALLOWED follows */
#ifndef SYS_WINNT
int
main(
	int argc,
	char *argv[]
	)
{
	return ntpdmain(argc, argv);
}
#endif /* !SYS_WINNT */
#endif /* !NO_MAIN_ALLOWED */
#endif /* !SIM */

#ifdef _AIX
/*
 * OK. AIX is different than solaris in how it implements plock().
 * If you do NOT adjust the stack limit, you will get the MAXIMUM
 * stack size allocated and PINNED with you program. To check the
 * value, use ulimit -a.
 *
 * To fix this, we create an automatic variable and set our stack limit
 * to that PLUS 32KB of extra space (we need some headroom).
 *
 * This subroutine gets the stack address.
 *
 * Grover Davidson and Matt Ladendorf
 *
 */
static char *
get_aix_stack(void)
{
	char ch;
	return (&ch);
}

/*
 * Signal handler for SIGDANGER.
 */
static void
catch_danger(int signo)
{
	msyslog(LOG_INFO, "ntpd: setpgid(): %m");
	/* Make the system believe we'll free something, but don't do it! */
	return;
}
#endif /* _AIX */

/*
 * Set the process priority
 */
#ifndef SIM
static void
set_process_priority(void)
{

# ifdef DEBUG
	if (debug > 1)
		msyslog(LOG_DEBUG, "set_process_priority: %s: priority_done is <%d>",
			((priority_done)
			 ? "Leave priority alone"
			 : "Attempt to set priority"
				),
			priority_done);
# endif /* DEBUG */

# if defined(HAVE_SCHED_SETSCHEDULER)
	if (!priority_done) {
		extern int config_priority_override, config_priority;
		int pmax, pmin;
		struct sched_param sched;

		pmax = sched_get_priority_max(SCHED_FIFO);
		sched.sched_priority = pmax;
		if ( config_priority_override ) {
			pmin = sched_get_priority_min(SCHED_FIFO);
			if ( config_priority > pmax )
				sched.sched_priority = pmax;
			else if ( config_priority < pmin )
				sched.sched_priority = pmin;
			else
				sched.sched_priority = config_priority;
		}
		if ( sched_setscheduler(0, SCHED_FIFO, &sched) == -1 )
			msyslog(LOG_ERR, "sched_setscheduler(): %m");
		else
			++priority_done;
	}
# endif /* HAVE_SCHED_SETSCHEDULER */
# ifdef HAVE_RTPRIO
#  ifdef RTP_SET
	if (!priority_done) {
		struct rtprio srtp;

		srtp.type = RTP_PRIO_REALTIME;	/* was: RTP_PRIO_NORMAL */
		srtp.prio = 0;		/* 0 (hi) -> RTP_PRIO_MAX (31,lo) */

		if (rtprio(RTP_SET, getpid(), &srtp) < 0)
			msyslog(LOG_ERR, "rtprio() error: %m");
		else
			++priority_done;
	}
#  else	/* !RTP_SET follows */
	if (!priority_done) {
		if (rtprio(0, 120) < 0)
			msyslog(LOG_ERR, "rtprio() error: %m");
		else
			++priority_done;
	}
#  endif	/* !RTP_SET */
# endif	/* HAVE_RTPRIO */
# if defined(NTPD_PRIO) && NTPD_PRIO != 0
#  ifdef HAVE_ATT_NICE
	if (!priority_done) {
		errno = 0;
		if (-1 == nice (NTPD_PRIO) && errno != 0)
			msyslog(LOG_ERR, "nice() error: %m");
		else
			++priority_done;
	}
#  endif	/* HAVE_ATT_NICE */
#  ifdef HAVE_BSD_NICE
	if (!priority_done) {
		if (-1 == setpriority(PRIO_PROCESS, 0, NTPD_PRIO))
			msyslog(LOG_ERR, "setpriority() error: %m");
		else
			++priority_done;
	}
#  endif	/* HAVE_BSD_NICE */
# endif	/* NTPD_PRIO && NTPD_PRIO != 0 */
	if (!priority_done)
		msyslog(LOG_ERR, "set_process_priority: No way found to improve our priority");
}
#endif	/* !SIM */


/*
 * Main program.  Initialize us, disconnect us from the tty if necessary,
 * and loop waiting for I/O and/or timer expiries.
 */
#ifndef SIM
int
ntpdmain(
	int argc,
	char *argv[]
	)
{
	l_fp now;
	struct recvbuf *rbuf;
# ifdef HAVE_UMASK
	mode_t	uv;
# endif
# if defined(HAVE_GETUID) && !defined(MPE) /* MPE lacks the concept of root */
	uid_t	uid;
# endif
# if defined(HAVE_WORKING_FORK)
	long	wait_sync = 0;
	int	pipe_fds[2];
	int	rc;
	int	exit_code;
#  ifdef _AIX
	struct sigaction sa;
#  endif
#  if !defined(HAVE_SETSID) && !defined (HAVE_SETPGID) && defined(TIOCNOTTY)
	int	fid;
#  endif
# endif	/* HAVE_WORKING_FORK*/
# ifdef SCO5_CLOCK
	int	fd;
	int	zero;
# endif
# if defined(HAVE_MLOCKALL) && defined(MCL_CURRENT) && defined(MCL_FUTURE)
#  ifdef HAVE_SETRLIMIT
	struct rlimit rl;
#  endif
# endif

	progname = argv[0];
	initializing = 1;		/* mark that we are initializing */
	parse_cmdline_opts(&argc, &argv);
	init_logging(progname, 1);	/* Open the log file */

	/*
	 * Install trap handlers to log errors and assertion failures.
	 * Default handlers print to stderr which doesn't work if detached.
	 */
	isc_assertion_setcallback(assertion_failed);
	isc_error_setfatal(library_fatal_error);
	isc_error_setunexpected(library_unexpected_error);

# ifdef HAVE_UMASK
	uv = umask(0);
	if (uv)
		umask(uv);
	else
		umask(022);
# endif

	/* MPE lacks the concept of root */
# if defined(HAVE_GETUID) && !defined(MPE)
	uid = getuid();
	if (uid && !HAVE_OPT( SAVECONFIGQUIT )) {
		msyslog(LOG_ERR,
			"must be run as root, not uid %ld", (long)uid);
		printf("%s must be run as root, not uid %ld\n",
		       progname, (long)uid);
		exit(1);
	}
# endif

# ifdef DEBUG
	debug = DESC(DEBUG_LEVEL).optOccCt;
	DPRINTF(1, ("%s\n", Version));
# endif

	/* honor -l/--logfile option to log to a file */
	setup_logfile(1);

/*
 * Enable the Multi-Media Timer for Windows?
 */
# ifdef SYS_WINNT
	if (HAVE_OPT( MODIFYMMTIMER ))
		set_mm_timer(MM_TIMER_HIRES);
# endif

	if (HAVE_OPT( NOFORK ) || HAVE_OPT( QUIT )
# ifdef DEBUG
	    || debug
# endif
	    || HAVE_OPT( SAVECONFIGQUIT ))
		nofork = 1;

	if (HAVE_OPT( NOVIRTUALIPS ))
		listen_to_virtual_ips = 0;

	/*
	 * --interface, listen on specified interfaces
	 */
	if (HAVE_OPT( INTERFACE )) {
		int		ifacect = STACKCT_OPT( INTERFACE );
		const char**	ifaces  = STACKLST_OPT( INTERFACE );
		sockaddr_u	addr;

		while (ifacect-- > 0) {
			add_nic_rule(
				is_ip_address(*ifaces, AF_UNSPEC, &addr)
					? MATCH_IFADDR
					: MATCH_IFNAME,
				*ifaces, -1, ACTION_LISTEN);
			ifaces++;
		}
	}

	if (HAVE_OPT( NICE ))
		priority_done = 0;

# ifdef HAVE_SCHED_SETSCHEDULER
	if (HAVE_OPT( PRIORITY )) {
		config_priority = OPT_VALUE_PRIORITY;
		config_priority_override = 1;
		priority_done = 0;
	}
# endif

# ifdef HAVE_WORKING_FORK
	do {					/* 'loop' once */
		if (!HAVE_OPT( WAIT_SYNC ))
			break;
/* should be 	wait_sync = OPT_VALUE_WAIT_SYNC;  after Autogen 5.10 fixed */
	 	wait_sync = DESC(WAIT_SYNC).optArg.argInt;
		if (wait_sync <= 0) {
			wait_sync = 0;
			break;
		}
		/* -w requires a fork() even with debug > 0 */
		nofork = 0;
		if (pipe(pipe_fds)) {
			exit_code = (errno) ? errno : -1;
			msyslog(LOG_ERR,
				"Pipe creation failed for --wait-sync: %m");
			exit(exit_code);
		}
		waitsync_fd_to_close = pipe_fds[1];
	} while (0);				/* 'loop' once */
# endif	/* HAVE_WORKING_FORK */

# ifdef SYS_WINNT
	/*
	 * Start interpolation thread, must occur before first
	 * get_systime()
	 */
	init_winnt_time();
# endif
	/*
	 * Initialize random generator and public key pair
	 */
	get_systime(&now);

	ntp_srandom((int)(now.l_i * now.l_uf));

	/*
	 * Detach us from the terminal.  May need an #ifndef GIZMO.
	 */
	if (!nofork) {

# ifdef HAVE_WORKING_FORK
		rc = fork();
		if (-1 == rc) {
			exit_code = (errno) ? errno : -1;
			msyslog(LOG_ERR, "fork: %m");
			exit(exit_code);
		}
		if (rc > 0) {	
			/* parent */
			exit_code = wait_child_sync_if(pipe_fds[0],
						       wait_sync);
			exit(exit_code);
		}
		
		/*
		 * child/daemon 
		 * close all open files excepting waitsync_fd_to_close.
		 * msyslog() unreliable until after init_logging().
		 */
		closelog();
		if (syslog_file != NULL) {
			fclose(syslog_file);
			syslog_file = NULL;
			syslogit = 1;
		}
		close_all_except(waitsync_fd_to_close);
		open("/dev/null", 0);
		dup2(0, 1);
		dup2(0, 2);

		init_logging(progname, 0);
		/* we lost our logfile (if any) daemonizing */
		setup_logfile(0);

#  ifdef SYS_DOMAINOS
		{
			uid_$t puid;
			status_$t st;

			proc2_$who_am_i(&puid);
			proc2_$make_server(&puid, &st);
		}
#  endif	/* SYS_DOMAINOS */
#  ifdef HAVE_SETSID
		if (setsid() == (pid_t)-1)
			msyslog(LOG_ERR, "setsid(): %m");
#  elif defined(HAVE_SETPGID)
		if (setpgid(0, 0) == -1)
			msyslog(LOG_ERR, "setpgid(): %m");
#  else		/* !HAVE_SETSID && !HAVE_SETPGID follows */
#   ifdef TIOCNOTTY
		fid = open("/dev/tty", 2);
		if (fid >= 0) {
			ioctl(fid, (u_long)TIOCNOTTY, NULL);
			close(fid);
		}
#   endif	/* TIOCNOTTY */
		ntp_setpgrp(0, getpid());
#  endif	/* !HAVE_SETSID && !HAVE_SETPGID */
#  ifdef _AIX
		/* Don't get killed by low-on-memory signal. */
		sa.sa_handler = catch_danger;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sigaction(SIGDANGER, &sa, NULL);
#  endif	/* _AIX */
# endif		/* HAVE_WORKING_FORK */
	}

# ifdef SCO5_CLOCK
	/*
	 * SCO OpenServer's system clock offers much more precise timekeeping
	 * on the base CPU than the other CPUs (for multiprocessor systems),
	 * so we must lock to the base CPU.
	 */
	fd = open("/dev/at1", O_RDONLY);		
	if (fd >= 0) {
		zero = 0;
		if (ioctl(fd, ACPU_LOCK, &zero) < 0)
			msyslog(LOG_ERR, "cannot lock to base CPU: %m");
		close(fd);
	}
# endif

# if defined(HAVE_MLOCKALL) && defined(MCL_CURRENT) && defined(MCL_FUTURE)
#  ifdef HAVE_SETRLIMIT
	/*
	 * Set the stack limit to something smaller, so that we don't lock a lot
	 * of unused stack memory.
	 */
	/* HMS: must make the rlim_cur amount configurable */
	if (getrlimit(RLIMIT_STACK, &rl) != -1
	    && (rl.rlim_cur = 50 * 4096) < rl.rlim_max
	    && setrlimit(RLIMIT_STACK, &rl) == -1)
		msyslog(LOG_ERR,
			"Cannot adjust stack limit for mlockall: %m");
#   ifdef RLIMIT_MEMLOCK
	/*
	 * The default RLIMIT_MEMLOCK is very low on Linux systems.
	 * Unless we increase this limit malloc calls are likely to
	 * fail if we drop root privlege.  To be useful the value
	 * has to be larger than the largest ntpd resident set size.
	 */
	rl.rlim_cur = rl.rlim_max = 32 * 1024 * 1024;
	if (setrlimit(RLIMIT_MEMLOCK, &rl) == -1)
		msyslog(LOG_ERR, "Cannot set RLIMIT_MEMLOCK: %m");
#   endif	/* RLIMIT_MEMLOCK */
#  endif	/* HAVE_SETRLIMIT */
	/*
	 * lock the process into memory
	 */
	if (mlockall(MCL_CURRENT|MCL_FUTURE) < 0)
		msyslog(LOG_ERR, "mlockall(): %m");
# else	/* !HAVE_MLOCKALL || !MCL_CURRENT || !MCL_FUTURE follows */
#  ifdef HAVE_PLOCK
#   ifdef PROCLOCK
#    ifdef _AIX
	/*
	 * set the stack limit for AIX for plock().
	 * see get_aix_stack() for more info.
	 */
	if (ulimit(SET_STACKLIM, (get_aix_stack() - 8 * 4096)) < 0)
		msyslog(LOG_ERR,
			"Cannot adjust stack limit for plock: %m");
#    endif	/* _AIX */
	/*
	 * lock the process into memory
	 */
	if (plock(PROCLOCK) < 0)
		msyslog(LOG_ERR, "plock(PROCLOCK): %m");
#   else	/* !PROCLOCK follows  */
#    ifdef TXTLOCK
	/*
	 * Lock text into ram
	 */
	if (plock(TXTLOCK) < 0)
		msyslog(LOG_ERR, "plock(TXTLOCK) error: %m");
#    else	/* !TXTLOCK follows */
	msyslog(LOG_ERR, "plock() - don't know what to lock!");
#    endif	/* !TXTLOCK */
#   endif	/* !PROCLOCK */
#  endif	/* HAVE_PLOCK */
# endif	/* !HAVE_MLOCKALL || !MCL_CURRENT || !MCL_FUTURE */

	/*
	 * Set up signals we pay attention to locally.
	 */
# ifdef SIGDIE1
	signal_no_reset(SIGDIE1, finish);
# endif
# ifdef SIGDIE2
	signal_no_reset(SIGDIE2, finish);
# endif
# ifdef SIGDIE3
	signal_no_reset(SIGDIE3, finish);
# endif
# ifdef SIGDIE4
	signal_no_reset(SIGDIE4, finish);
# endif
# ifdef SIGBUS
	signal_no_reset(SIGBUS, finish);
# endif

# if !defined(SYS_WINNT) && !defined(VMS)
#  ifdef DEBUG
	(void) signal_no_reset(MOREDEBUGSIG, moredebug);
	(void) signal_no_reset(LESSDEBUGSIG, lessdebug);
#  else
	(void) signal_no_reset(MOREDEBUGSIG, no_debug);
	(void) signal_no_reset(LESSDEBUGSIG, no_debug);
#  endif	/* DEBUG */
# endif	/* !SYS_WINNT && !VMS */

	/*
	 * Set up signals we should never pay attention to.
	 */
# ifdef SIGPIPE
	signal_no_reset(SIGPIPE, SIG_IGN);
# endif

	/*
	 * Call the init_ routines to initialize the data structures.
	 *
	 * Exactly what command-line options are we expecting here?
	 */
	init_auth();
	init_util();
	init_restrict();
	init_mon();
	init_timer();
	init_lib();
	init_request();
	init_control();
	init_peer();
# ifdef REFCLOCK
	init_refclock();
# endif
	set_process_priority();
	init_proto();		/* Call at high priority */
	init_io();
	init_loopfilter();
	mon_start(MON_ON);	/* monitor on by default now	  */
				/* turn off in config if unwanted */

	/*
	 * Get the configuration.  This is done in a separate module
	 * since this will definitely be different for the gizmo board.
	 */
	getconfig(argc, argv);
	report_event(EVNT_SYSRESTART, NULL, NULL);
	loop_config(LOOP_DRIFTCOMP, old_drift);
	initializing = 0;

# ifdef HAVE_DROPROOT
	if (droproot) {
		/* Drop super-user privileges and chroot now if the OS supports this */

#  ifdef HAVE_LINUX_CAPABILITIES
		/* set flag: keep privileges accross setuid() call (we only really need cap_sys_time): */
		if (prctl( PR_SET_KEEPCAPS, 1L, 0L, 0L, 0L ) == -1) {
			msyslog( LOG_ERR, "prctl( PR_SET_KEEPCAPS, 1L ) failed: %m" );
			exit(-1);
		}
#  else
		/* we need a user to switch to */
		if (user == NULL) {
			msyslog(LOG_ERR, "Need user name to drop root privileges (see -u flag!)" );
			exit(-1);
		}
#  endif	/* HAVE_LINUX_CAPABILITIES */

		if (user != NULL) {
			if (isdigit((unsigned char)*user)) {
				sw_uid = (uid_t)strtoul(user, &endp, 0);
				if (*endp != '\0')
					goto getuser;

				if ((pw = getpwuid(sw_uid)) != NULL) {
					user = strdup(pw->pw_name);
					if (NULL == user) {
						msyslog(LOG_ERR, "strdup() failed: %m");
						exit (-1);
					}
					sw_gid = pw->pw_gid;
				} else {
					errno = 0;
					msyslog(LOG_ERR, "Cannot find user ID %s", user);
					exit (-1);
				}

			} else {
getuser:
				errno = 0;
				if ((pw = getpwnam(user)) != NULL) {
					sw_uid = pw->pw_uid;
					sw_gid = pw->pw_gid;
				} else {
					if (errno)
					    msyslog(LOG_ERR, "getpwnam(%s) failed: %m", user);
					else
					    msyslog(LOG_ERR, "Cannot find user `%s'", user);
					exit (-1);
				}
			}
		}
		if (group != NULL) {
			if (isdigit((unsigned char)*group)) {
				sw_gid = (gid_t)strtoul(group, &endp, 0);
				if (*endp != '\0')
					goto getgroup;
			} else {
getgroup:
				if ((gr = getgrnam(group)) != NULL) {
					sw_gid = gr->gr_gid;
				} else {
					errno = 0;
					msyslog(LOG_ERR, "Cannot find group `%s'", group);
					exit (-1);
				}
			}
		}

		if (chrootdir ) {
			/* make sure cwd is inside the jail: */
			if (chdir(chrootdir)) {
				msyslog(LOG_ERR, "Cannot chdir() to `%s': %m", chrootdir);
				exit (-1);
			}
			if (chroot(chrootdir)) {
				msyslog(LOG_ERR, "Cannot chroot() to `%s': %m", chrootdir);
				exit (-1);
			}
			if (chdir("/")) {
				msyslog(LOG_ERR, "Cannot chdir() to`root after chroot(): %m");
				exit (-1);
			}
		}
		if (user && initgroups(user, sw_gid)) {
			msyslog(LOG_ERR, "Cannot initgroups() to user `%s': %m", user);
			exit (-1);
		}
		if (group && setgid(sw_gid)) {
			msyslog(LOG_ERR, "Cannot setgid() to group `%s': %m", group);
			exit (-1);
		}
		if (group && setegid(sw_gid)) {
			msyslog(LOG_ERR, "Cannot setegid() to group `%s': %m", group);
			exit (-1);
		}
		if (user && setuid(sw_uid)) {
			msyslog(LOG_ERR, "Cannot setuid() to user `%s': %m", user);
			exit (-1);
		}
		if (user && seteuid(sw_uid)) {
			msyslog(LOG_ERR, "Cannot seteuid() to user `%s': %m", user);
			exit (-1);
		}

#  ifndef HAVE_LINUX_CAPABILITIES
		/*
		 * for now assume that the privilege to bind to privileged ports
		 * is associated with running with uid 0 - should be refined on
		 * ports that allow binding to NTP_PORT with uid != 0
		 */
		disable_dynamic_updates |= (sw_uid != 0);  /* also notifies routing message listener */
#  endif

		if (disable_dynamic_updates && interface_interval) {
			interface_interval = 0;
			msyslog(LOG_INFO, "running as non-root disables dynamic interface tracking");
		}

#  ifdef HAVE_LINUX_CAPABILITIES
		{
			/*
			 *  We may be running under non-root uid now, but we still hold full root privileges!
			 *  We drop all of them, except for the crucial one or two: cap_sys_time and
			 *  cap_net_bind_service if doing dynamic interface tracking.
			 */
			cap_t caps;
			char *captext;
			
			captext = (interface_interval)
				      ? "cap_sys_time,cap_net_bind_service=pe"
				      : "cap_sys_time=pe";
			caps = cap_from_text(captext);
			if (!caps) {
				msyslog(LOG_ERR,
					"cap_from_text(%s) failed: %m",
					captext);
				exit(-1);
			}
			if (-1 == cap_set_proc(caps)) {
				msyslog(LOG_ERR,
					"cap_set_proc() failed to drop root privs: %m");
				exit(-1);
			}
			cap_free(caps);
		}
#  endif	/* HAVE_LINUX_CAPABILITIES */
		root_dropped = 1;
		fork_deferred_worker();
	}	/* if (droproot) */
# endif	/* HAVE_DROPROOT */

	/*
	 * Use select() on all on all input fd's for unlimited
	 * time.  select() will terminate on SIGALARM or on the
	 * reception of input.	Using select() means we can't do
	 * robust signal handling and we get a potential race
	 * between checking for alarms and doing the select().
	 * Mostly harmless, I think.
	 */
	/* On VMS, I suspect that select() can't be interrupted
	 * by a "signal" either, so I take the easy way out and
	 * have select() time out after one second.
	 * System clock updates really aren't time-critical,
	 * and - lacking a hardware reference clock - I have
	 * yet to learn about anything else that is.
	 */
# ifdef HAVE_IO_COMPLETION_PORT

	for (;;) {
		GetReceivedBuffers();
# else /* normal I/O */

	BLOCK_IO_AND_ALARM();
	was_alarmed = 0;
	for (;;)
	{
#  ifndef HAVE_SIGNALED_IO
		fd_set rdfdes;
		int nfound;
#  endif

		if (alarm_flag)		/* alarmed? */
		{
			was_alarmed = 1;
			alarm_flag = 0;
		}

		if (!was_alarmed && has_full_recv_buffer() == ISC_FALSE)
		{
			/*
			 * Nothing to do.  Wait for something.
			 */
#  ifndef HAVE_SIGNALED_IO
			rdfdes = activefds;
#   if defined(VMS) || defined(SYS_VXWORKS)
			/* make select() wake up after one second */
			{
				struct timeval t1;

				t1.tv_sec = 1; t1.tv_usec = 0;
				nfound = select(maxactivefd+1, &rdfdes, (fd_set *)0,
						(fd_set *)0, &t1);
			}
#   else
			nfound = select(maxactivefd+1, &rdfdes, (fd_set *)0,
					(fd_set *)0, (struct timeval *)0);
#   endif /* VMS */
			if (nfound > 0)
			{
				l_fp ts;

				get_systime(&ts);

				(void)input_handler(&ts);
			}
			else if (nfound == -1 && errno != EINTR)
				msyslog(LOG_ERR, "select() error: %m");
#   ifdef DEBUG
			else if (debug > 5)
				msyslog(LOG_DEBUG, "select(): nfound=%d, error: %m", nfound);
#   endif /* DEBUG */
#  else /* HAVE_SIGNALED_IO */

			wait_for_signal();
#  endif /* HAVE_SIGNALED_IO */
			if (alarm_flag)		/* alarmed? */
			{
				was_alarmed = 1;
				alarm_flag = 0;
			}
		}

		if (was_alarmed)
		{
			UNBLOCK_IO_AND_ALARM();
			/*
			 * Out here, signals are unblocked.  Call timer routine
			 * to process expiry.
			 */
			timer();
			was_alarmed = 0;
			BLOCK_IO_AND_ALARM();
		}

# endif		/* !HAVE_IO_COMPLETION_PORT */

# ifdef DEBUG_TIMING
		{
			l_fp pts;
			l_fp tsa, tsb;
			int bufcount = 0;

			get_systime(&pts);
			tsa = pts;
# endif
			rbuf = get_full_recv_buffer();
			while (rbuf != NULL)
			{
				if (alarm_flag)
				{
					was_alarmed = 1;
					alarm_flag = 0;
				}
				UNBLOCK_IO_AND_ALARM();

				if (was_alarmed)
				{	/* avoid timer starvation during lengthy I/O handling */
					timer();
					was_alarmed = 0;
				}

				/*
				 * Call the data procedure to handle each received
				 * packet.
				 */
				if (rbuf->receiver != NULL)	/* This should always be true */
				{
# ifdef DEBUG_TIMING
					l_fp dts = pts;

					L_SUB(&dts, &rbuf->recv_time);
					DPRINTF(2, ("processing timestamp delta %s (with prec. fuzz)\n", lfptoa(&dts, 9)));
					collect_timing(rbuf, "buffer processing delay", 1, &dts);
					bufcount++;
# endif
					(rbuf->receiver)(rbuf);
				} else {
					msyslog(LOG_ERR, "receive buffer corruption - receiver found to be NULL - ABORTING");
					abort();
				}

				BLOCK_IO_AND_ALARM();
				freerecvbuf(rbuf);
				rbuf = get_full_recv_buffer();
			}
# ifdef DEBUG_TIMING
			get_systime(&tsb);
			L_SUB(&tsb, &tsa);
			if (bufcount) {
				collect_timing(NULL, "processing", bufcount, &tsb);
				DPRINTF(2, ("processing time for %d buffers %s\n", bufcount, lfptoa(&tsb, 9)));
			}
		}
# endif

		/*
		 * Go around again
		 */

# ifdef HAVE_DNSREGISTRATION
		if (mdnsreg && (current_time - mdnsreg ) > 60 && mdnstries && sys_leap != LEAP_NOTINSYNC) {
			mdnsreg = current_time;
			msyslog(LOG_INFO, "Attemping to register mDNS");
			if ( DNSServiceRegister (&mdns, 0, 0, NULL, "_ntp._udp", NULL, NULL, 
			    htons(NTP_PORT), 0, NULL, NULL, NULL) != kDNSServiceErr_NoError ) {
				if (!--mdnstries) {
					msyslog(LOG_ERR, "Unable to register mDNS, giving up.");
				} else {	
					msyslog(LOG_INFO, "Unable to register mDNS, will try later.");
				}
			} else {
				msyslog(LOG_INFO, "mDNS service registered.");
				mdnsreg = 0;
			}
		}
# endif /* HAVE_DNSREGISTRATION */

	}
	UNBLOCK_IO_AND_ALARM();
	return 1;
}
#endif	/* !SIM */


#if !defined (SIM) && defined(SIGDIE2)
/*
 * finish - exit gracefully
 */
static RETSIGTYPE
finish(
	int sig
	)
{
	msyslog(LOG_NOTICE, "ntpd exiting on signal %d", sig);
# ifdef HAVE_DNSREGISTRATION
	if (mdns != NULL)
		DNSServiceRefDeallocate(mdns);
# endif
	switch (sig) {
# ifdef SIGBUS
	case SIGBUS:
		printf("\nfinish(SIGBUS)\n");
		exit(0);
# endif
	case 0:			/* Should never happen... */
		return;

	default:
		exit(0);
	}
}
#endif	/* !SIM && SIGDIE2 */

#ifndef SIM
/*
 * wait_child_sync_if - implements parent side of -w/--wait-sync
 */
# ifdef HAVE_WORKING_FORK
static int
wait_child_sync_if(
	int	pipe_read_fd,
	long	wait_sync
	)
{
	int	rc;
	int	exit_code;
	time_t	wait_end_time;
	time_t	cur_time;
	time_t	wait_rem;
	fd_set	readset;
	struct timeval wtimeout;

	if (0 == wait_sync) 
		return 0;

	/* waitsync_fd_to_close used solely by child */
	close(waitsync_fd_to_close);
	wait_end_time = time(NULL) + wait_sync;
	do {
		cur_time = time(NULL);
		wait_rem = (wait_end_time > cur_time)
				? (wait_end_time - cur_time)
				: 0;
		wtimeout.tv_sec = wait_rem;
		wtimeout.tv_usec = 0;
		FD_ZERO(&readset);
		FD_SET(pipe_read_fd, &readset);
		rc = select(pipe_read_fd + 1, &readset, NULL, NULL,
			    &wtimeout);
		if (-1 == rc) {
			if (EINTR == errno)
				continue;
			exit_code = (errno) ? errno : -1;
			msyslog(LOG_ERR,
				"--wait-sync select failed: %m");
			return exit_code;
		}
		if (0 == rc) {
			/*
			 * select() indicated a timeout, but in case
			 * its timeouts are affected by a step of the
			 * system clock, select() again with a zero 
			 * timeout to confirm.
			 */
			FD_ZERO(&readset);
			FD_SET(pipe_read_fd, &readset);
			wtimeout.tv_sec = 0;
			wtimeout.tv_usec = 0;
			rc = select(pipe_read_fd + 1, &readset, NULL,
				    NULL, &wtimeout);
			if (0 == rc)	/* select() timeout */
				break;
			else		/* readable */
				return 0;
		} else			/* readable */
			return 0;
	} while (wait_rem > 0);

	fprintf(stderr, "%s: -w/--wait-sync %ld timed out.\n",
		progname, wait_sync);
	return ETIMEDOUT;
}
# endif	/* HAVE_WORKING_FORK */


/*
 * assertion_failed - Redirect assertion failures to msyslog().
 */
static void
assertion_failed(
	const char *file,
	int line,
	isc_assertiontype_t type,
	const char *cond
	)
{
	isc_assertion_setcallback(NULL);    /* Avoid recursion */

	msyslog(LOG_ERR, "%s:%d: %s(%s) failed",
		file, line, isc_assertion_typetotext(type), cond);
	msyslog(LOG_ERR, "exiting (due to assertion failure)");

#if defined(DEBUG) && defined(SYS_WINNT)
	if (debug)
		DebugBreak();
#endif

	abort();
}


/*
 * library_fatal_error - Handle fatal errors from our libraries.
 */
static void
library_fatal_error(
	const char *file,
	int line,
	const char *format,
	va_list args
	)
{
	char errbuf[256];

	isc_error_setfatal(NULL);  /* Avoid recursion */

	msyslog(LOG_ERR, "%s:%d: fatal error:", file, line);
	vsnprintf(errbuf, sizeof(errbuf), format, args);
	msyslog(LOG_ERR, "%s", errbuf);
	msyslog(LOG_ERR, "exiting (due to fatal error in library)");

#if defined(DEBUG) && defined(SYS_WINNT)
	if (debug)
		DebugBreak();
#endif

	abort();
}


/*
 * library_unexpected_error - Handle non fatal errors from our libraries.
 */
# define MAX_UNEXPECTED_ERRORS 100
int unexpected_error_cnt = 0;
static void
library_unexpected_error(
	const char *file,
	int line,
	const char *format,
	va_list args
	)
{
	char errbuf[256];

	if (unexpected_error_cnt >= MAX_UNEXPECTED_ERRORS)
		return;	/* avoid clutter in log */

	msyslog(LOG_ERR, "%s:%d: unexpected error:", file, line);
	vsnprintf(errbuf, sizeof(errbuf), format, args);
	msyslog(LOG_ERR, "%s", errbuf);

	if (++unexpected_error_cnt == MAX_UNEXPECTED_ERRORS)
		msyslog(LOG_ERR, "Too many errors.  Shutting up.");

}
#endif	/* !SIM */

#if !defined(SIM) && !defined(SYS_WINNT)
# ifdef DEBUG

/*
 * moredebug - increase debugging verbosity
 */
static RETSIGTYPE
moredebug(
	int sig
	)
{
	int saved_errno = errno;

	if (debug < 255)
	{
		debug++;
		msyslog(LOG_DEBUG, "debug raised to %d", debug);
	}
	errno = saved_errno;
}


/*
 * lessdebug - decrease debugging verbosity
 */
static RETSIGTYPE
lessdebug(
	int sig
	)
{
	int saved_errno = errno;

	if (debug > 0)
	{
		debug--;
		msyslog(LOG_DEBUG, "debug lowered to %d", debug);
	}
	errno = saved_errno;
}

# else	/* !DEBUG follows */


/*
 * no_debug - We don't do the debug here.
 */
static RETSIGTYPE
no_debug(
	int sig
	)
{
	int saved_errno = errno;

	msyslog(LOG_DEBUG, "ntpd not compiled for debugging (signal %d)", sig);
	errno = saved_errno;
}
# endif	/* !DEBUG */
#endif	/* !SIM && !SYS_WINNT */
