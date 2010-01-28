#ifndef IOSIGNAL_H
#define IOSIGNAL_H

#include "ntp_refclock.h"

 /*
 * Some systems (MOST) define SIGPOLL == SIGIO, others SIGIO == SIGPOLL, and
 * a few have separate SIGIO and SIGPOLL signals.  This code checks for the
 * SIGIO == SIGPOLL case at compile time.
 * Do not define USE_SIGPOLL or USE_SIGIO.
 * these are interal only to iosignal.c and ntpd/work_fork.c!
 */
#if defined(USE_SIGPOLL)
# undef USE_SIGPOLL
#endif
#if defined(USE_SIGIO)
# undef USE_SIGIO
#endif

#if defined(HAVE_SIGNALED_IO)
# if defined(USE_TTY_SIGPOLL) || defined(USE_UDP_SIGPOLL)
#  define USE_SIGPOLL
# endif

# if !defined(USE_TTY_SIGPOLL) || !defined(USE_UDP_SIGPOLL)
#  define USE_SIGIO
# endif

# if defined(USE_SIGIO) && defined(USE_SIGPOLL)
#  if SIGIO == SIGPOLL
#   define USE_SIGIO
#   undef USE_SIGPOLL
#  endif	/* SIGIO == SIGPOLL */
# endif		/* USE_SIGIO && USE_SIGPOLL */

extern void		block_sigio	(void);
extern void		unblock_sigio	(void);
extern int		init_clock_sig	(struct refclockio *);
extern void		init_socket_sig	(int);
extern void		set_signal	(void);
RETSIGTYPE		sigio_handler	(int);

# define BLOCKIO()	block_sigio()
# define UNBLOCKIO()	unblock_sigio()

#else	/* !HAVE_SIGNALED_IO follows */
# define BLOCKIO()	do {} while (0)
# define UNBLOCKIO()	do {} while (0)
#endif

#endif	/* IOSIGNAL_H */
