#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#if defined (HAVE_IO_COMPLETION_PORT)

#include <stddef.h>
#include <stdio.h>
#include <process.h>
#include <syslog.h>

#include "ntpd.h"
#include "ntp_machine.h"
#include "ntp_iocompletionport.h"
#include "ntp_request.h"
#include "ntp_assert.h"
#include "ntp_io.h"
#include "ntp_lists.h"


#define CONTAINEROF(p, type, member) \
	((type *)((char *)(p) - offsetof(type, member)))

/*
 * Request types
 */
enum {
	SOCK_RECV,
	SOCK_SEND,
	SERIAL_WAIT,
	SERIAL_READ,
	RAW_SERIAL_READ,
	SERIAL_WRITE
};

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 201)		/* nonstd extension nameless union */
#endif

typedef struct olplus_tag {
	OVERLAPPED		ol;
	int			request_type;
	union {
		recvbuf_t *	recv_buf;
		void *		trans_buf;
	};
#ifdef DEBUG
	struct olplus_tag *	link;
#endif
} olplus;

#ifdef _MSC_VER
# pragma warning(pop)
#endif

/*
 * local function definitions
 */
	void	ntpd_addremove_semaphore(HANDLE, int);
static	int	QueueSerialWait(struct refclockio *, recvbuf_t *buff,
				olplus *lpo, BOOL clear_timestamp);
static	int	QueueRawSerialRead(struct refclockio *, recvbuf_t *buff,
				   olplus *lpo);
static	int	OnSocketRecv(ULONG_PTR, olplus *, DWORD, int);
static	int	OnSerialWaitComplete(struct refclockio *, olplus *,
				     DWORD, int);
static	int	OnSerialReadComplete(struct refclockio *, olplus *,
				     DWORD, int);
static	int	OnRawSerialReadComplete(struct refclockio *, olplus *,
					DWORD, int);
static	int	OnWriteComplete(ULONG_PTR, olplus *, DWORD, int);


/* keep a list to traverse to free memory on debug builds */
#ifdef DEBUG
static void free_io_completion_port_mem(void);
olplus *		compl_info_list;
CRITICAL_SECTION	compl_info_lock;
#define LOCK_COMPL()	EnterCriticalSection(&compl_info_lock)
#define UNLOCK_COMPL()	LeaveCriticalSection(&compl_info_lock)
#endif

/* #define USE_HEAP */

#ifdef USE_HEAP
static HANDLE hHeapHandle = NULL;
#endif

	HANDLE WaitableExitEventHandle;
static	HANDLE hIoCompletionPort = NULL;
static	HANDLE WaitableIoEventHandle = NULL;

#ifdef NTPNEEDNAMEDHANDLE
#define WAITABLEIOEVENTHANDLE "WaitableIoEventHandle"
#else
#define WAITABLEIOEVENTHANDLE NULL
#endif

DWORD	ActiveWaitHandles;
HANDLE	WaitHandles[16];

olplus *
GetHeapAlloc(char *fromfunc)
{
	olplus *lpo;

#ifdef USE_HEAP
	lpo = HeapAlloc(hHeapHandle,
			HEAP_ZERO_MEMORY,
			sizeof(olplus));
#else
	lpo = emalloc_zero(sizeof(*lpo));
#endif
	DPRINTF(3, ("Allocation %d memory for %s, ptr %x\n", sizeof(olplus), fromfunc, lpo));

#ifdef DEBUG
	LOCK_COMPL();
	LINK_SLIST(compl_info_list, lpo, link);
	UNLOCK_COMPL();
#endif

	return (lpo);
}

void
FreeHeap(olplus *lpo, char *fromfunc)
{
#ifdef DEBUG
	olplus *unlinked;

	DPRINTF(3, ("Freeing memory for %s, ptr %x\n", fromfunc, lpo));

	LOCK_COMPL();
	UNLINK_SLIST(unlinked, compl_info_list, lpo, link,
	    olplus);
	UNLOCK_COMPL();
#endif

#ifdef USE_HEAP
	HeapFree(hHeapHandle, 0, lpo);
#else
	free(lpo);
#endif
}

/*  This function will add an entry to the I/O completion port
 *  that will signal the I/O thread to exit (gracefully)
 */
static void
signal_io_completion_port_exit()
{
	if (!PostQueuedCompletionStatus(hIoCompletionPort, 0, 0, 0)) {
		msyslog(LOG_ERR, "Can't request service thread to exit: %m");
		exit(1);
	}
}

static unsigned WINAPI
iocompletionthread(void *NotUsed)
{
	BOOL			bSuccess;
	int			err;
	DWORD			octets;
	ULONG_PTR		key;
	struct refclockio *	rio;
	OVERLAPPED *		pol;
	olplus *		lpo;


	UNUSED_ARG(NotUsed);

	/*
	 *	socket and refclock receive call gettimeofday()
	 *	so the I/O thread needs to be on the same 
	 *	processor as the main and timing threads
	 *	to ensure consistent QueryPerformanceCounter()
	 *	results.
	 */
	lock_thread_to_processor(GetCurrentThread());

	/*	Set the thread priority high enough so I/O will
	 *	preempt normal recv packet processing, but not
	 * 	higher than the timer sync thread.
	 */
	if (!SetThreadPriority(GetCurrentThread(),
			       THREAD_PRIORITY_ABOVE_NORMAL))
		msyslog(LOG_ERR, "Can't set thread priority: %m");

	while (TRUE) {
		bSuccess = GetQueuedCompletionStatus(
					hIoCompletionPort, 
					&octets, 
					&key, 
					&pol, 
					INFINITE);
		if (NULL == pol) {
			DPRINTF(2, ("Overlapped IO Thread Exiting\n"));
			break; /* fail */
		}
		handler_calls++;
		lpo = CONTAINEROF(pol, olplus, ol);
		rio = (struct refclockio *)key;
		
		/*
		 * Deal with errors
		 */
		if (bSuccess)
			err = 0;
		else
			err = GetLastError();

		/*
		 * Invoke the appropriate function based on
		 * the value of the request_type
		 */
		switch (lpo->request_type) {

		case SOCK_RECV:
			OnSocketRecv(key, lpo, octets, err);
			break;

		case SOCK_SEND:
			NTP_INSIST(0);
			break;

		case SERIAL_WAIT:
			OnSerialWaitComplete(rio, lpo, octets, err);
			break;

		case SERIAL_READ:
			OnSerialReadComplete(rio, lpo, octets, err);
			break;

		case RAW_SERIAL_READ:
			OnRawSerialReadComplete(rio, lpo, octets, err);
			break;

		case SERIAL_WRITE:
			OnWriteComplete(key, lpo, octets, err);
			break;

		default:
			DPRINTF(1, ("Unknown request type %d found in completion port\n",
				    lpo->request_type));
			break;
		}
	}

	return 0;
}

/*  Create/initialise the I/O creation port
 */
void
init_io_completion_port(
	void
	)
{
	unsigned tid;
	HANDLE thread;

#ifdef DEBUG
	InitializeCriticalSection(&compl_info_lock);
	atexit(&free_io_completion_port_mem);
#endif

#ifdef USE_HEAP
	/*
	 * Create a handle to the Heap
	 */
	hHeapHandle = HeapCreate(0, 20*sizeof(olplus), 0);
	if (hHeapHandle == NULL)
	{
		msyslog(LOG_ERR, "Can't initialize Heap: %m");
		exit(1);
	}
#endif

	/* Create the event used to signal an IO event
	 */
	WaitableIoEventHandle = CreateEvent(NULL, FALSE, FALSE, WAITABLEIOEVENTHANDLE);
	if (WaitableIoEventHandle == NULL) {
		msyslog(LOG_ERR,
		"Can't create I/O event handle: %m - another process may be running - EXITING");
		exit(1);
	}
	/* Create the event used to signal an exit event
	 */
	WaitableExitEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (WaitableExitEventHandle == NULL) {
		msyslog(LOG_ERR,
		"Can't create exit event handle: %m - EXITING");
		exit(1);
	}

	/* Create the IO completion port
	 */
	hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hIoCompletionPort == NULL) {
		msyslog(LOG_ERR, "Can't create I/O completion port: %m");
		exit(1);
	}


	/*
	 * Initialize the Wait Handles
	 */
	WaitHandles[0] = WaitableIoEventHandle;
	WaitHandles[1] = WaitableExitEventHandle; /* exit request */
	WaitHandles[2] = WaitableTimerHandle;
	ActiveWaitHandles = 3;

	/*
	 * Supply ntp_worker.c with function to add or remove a
	 * semaphore to the ntpd I/O loop which is signalled by a worker
	 * when a response is ready.  The callback is invoked in the
	 * parent.
	 */
	addremove_io_semaphore = &ntpd_addremove_semaphore;

	/*
	 * Have one thread servicing I/O - there were 4, but this would 
	 * somehow cause NTP to stop replying to ntpq requests; TODO
 	 */
	thread = (HANDLE)_beginthreadex(
		NULL, 
		0, 
		iocompletionthread, 
		NULL, 
		CREATE_SUSPENDED, 
		&tid);
	ResumeThread(thread);
	CloseHandle(thread);
}


void
ntpd_addremove_semaphore(
	HANDLE	sem,
	int	remove
	)
{
	const size_t	hnd_sz = sizeof(WaitHandles[0]);
	u_int		hi;

	if (!remove) {
		INSIST((ActiveWaitHandles + 1) < COUNTOF(WaitHandles));
		WaitHandles[ActiveWaitHandles] = sem;
		ActiveWaitHandles++;

		return;
	}

	/* removing semaphore */
	for (hi = 3; hi < ActiveWaitHandles; hi++)
		if (sem == WaitHandles[hi])
			break;

	if (hi == ActiveWaitHandles)
		return;

	ActiveWaitHandles--;
	if (hi < ActiveWaitHandles)
		memmove(&WaitHandles[hi],
			&WaitHandles[hi + 1],
			(ActiveWaitHandles - hi) * hnd_sz);
	WaitHandles[ActiveWaitHandles] = NULL;
}


#ifdef DEBUG
static void
free_io_completion_port_mem(
	void
	)
{
	olplus *	pci;

#if defined(_MSC_VER) && defined (_DEBUG)
	_CrtCheckMemory();
#endif
	LOCK_COMPL();
	while ((pci = compl_info_list) != NULL) {

#if 0	/* sockaddr with received-from address in recvbuf */
	/* is sometimes modified by system after we free it  */
	/* triggering heap corruption warning -- find a */
	/* better way to free it after I/O is surely done */
		/* this handles both xmit and recv buffs */
		if (pci->recv_buf != NULL) {
			DPRINTF(1, ("freeing xmit/recv buff %p\n", pci->recv_buf));
			free(pci->recv_buf);
		}
#endif

		FreeHeap(pci, "free_io_completion_port_mem");
		/* FreeHeap() removed this item from compl_info_list */
	}
	UNLOCK_COMPL();

#if defined(_MSC_VER) && defined (_DEBUG)
	_CrtCheckMemory();
#endif
}
#endif	/* DEBUG */


void
uninit_io_completion_port(
	void
	)
{
	if (hIoCompletionPort != NULL) {
		/*  Get each of the service threads to exit
		*/
		signal_io_completion_port_exit();
	}
}


static int
QueueSerialWait(
	struct refclockio *	rio,
	recvbuf_t *		buff,
	olplus *		lpo,
	BOOL			clear_timestamp
	)
{
	DWORD err;

	lpo->request_type = SERIAL_WAIT;
	lpo->recv_buf = buff;

	if (clear_timestamp)
		ZERO(buff->recv_time);

	buff->fd = _get_osfhandle(rio->fd);
	if (!WaitCommEvent((HANDLE)buff->fd,
			   (DWORD *)&buff->recv_buffer, &lpo->ol)) {
		err = GetLastError();
		if (ERROR_IO_PENDING != err) {
			/*
			 * ERROR_INVALID_PARAMETER occurs after rio->fd
			 * is closed.
			 */
			if (ERROR_INVALID_PARAMETER != err)
				msyslog(LOG_ERR, "Can't wait on Refclock: %m");
			freerecvbuf(buff);
			return 0;
		}
	}
	return 1;
}


static int 
OnSerialWaitComplete(
	struct refclockio *	rio,
	olplus *		lpo,
	DWORD			Bytes,
	int errstatus
	)
{
	recvbuf_t *buff;
	struct peer *pp;
	l_fp arrival_time;
	DWORD comm_mask;
	DWORD modem_status;
	static const l_fp zero_time = { 0 };
	BOOL rc;

	get_systime(&arrival_time);

	if (!rio->active)
		return FALSE;

	/*
	 * Get the recvbuf pointer from the overlapped buffer.
	 */
	buff = lpo->recv_buf;
	comm_mask = (*(DWORD *)&buff->recv_buffer);
#ifdef DEBUG
		if (errstatus || comm_mask &
		    ~(EV_RXFLAG | EV_RLSD | EV_RXCHAR)) {
			msyslog(LOG_ERR, "WaitCommEvent returned unexpected mask %x errstatus %d",
				comm_mask, errstatus);
			exit(-1);
		}
#endif
		if (EV_RLSD & comm_mask) { 
			modem_status = 0;
			GetCommModemStatus((HANDLE)buff->fd, &modem_status);
			if (modem_status & MS_RLSD_ON) {
				/*
				 * Use the timestamp from this PPS CD not
				 * the later end of line.
				 */
				buff->recv_time = arrival_time;
			}

			if (EV_RLSD == comm_mask) {
				/*
				 * if we didn't see an end of line yet
				 * issue another wait for it.
				 */
				QueueSerialWait(rio, buff, lpo, FALSE);
				return TRUE;
			}
		}

		if (EV_RXCHAR & comm_mask) {		/* raw discipline */
			QueueRawSerialRead(rio, buff, lpo);
			return TRUE;
		}

		/*
		 * We've detected the end of line of serial input.
		 * Use this timestamp unless we already have a CD PPS
		 * timestamp in buff->recv_time.
		 */
		if (memcmp(&buff->recv_time, &zero_time, sizeof buff->recv_time)) {
			/*
			 * We will first see a user PPS timestamp here on either
			 * the first or second line of text.  Log a one-time
			 * message while processing the second line.
			 */
			if (1 == rio->recvcount) {
				pp = (struct peer *)rio->srcclock;
				msyslog(LOG_NOTICE, "Using user-mode PPS timestamp for %s",
					refnumtoa(&pp->srcadr));
			}
		} else {
			buff->recv_time = arrival_time;
		}

		/*
		 * Now that we have a complete line waiting, read it.
		 * There is still a race here, but we're likely to win.
		 */

		lpo->request_type = SERIAL_READ;

		rc = ReadFile(
			(HANDLE)buff->fd,
			buff->recv_buffer,
			sizeof(buff->recv_buffer),
			NULL,
			&lpo->ol);

		if (!rc && ERROR_IO_PENDING != GetLastError()) {
			msyslog(LOG_ERR, "Can't read from Refclock: %m");
			freerecvbuf(buff);
			return 0;
		}

	return 1;
}


/* Return 1 on Successful Read */
static int 
OnSerialReadComplete(
	struct refclockio *	rio,
	olplus *		lpo,
	DWORD			Bytes,
	int			errstatus
	)
{
	recvbuf_t *		buff;
	l_fp			cr_time;

	if (!rio->active)
		return FALSE;

	/*
	 * Get the recvbuf pointer from the overlapped buffer.
	 */
	buff = lpo->recv_buf;

	/*
	 * ignore 0 bytes read due to timeout's and closure on fd
	 */
	if (!errstatus && Bytes) {
		buff->recv_length = (int) Bytes;
		buff->receiver = rio->clock_recv;
		buff->dstadr = NULL;
		buff->recv_peer = rio->srcclock;
		packets_received++;
		handler_pkts++;
		/*
		 * Eat the first line of input as it's possibly
		 * partial and if a PPS is present, it may not 
		 * have fired since the port was opened.
		 */
		if (rio->recvcount++) {
			cr_time = buff->recv_time;
			buff->fd = rio->fd;		/* was handle */
			add_full_recv_buffer(buff);
			/*
			 * Mimic Unix line discipline and assume CR/LF
			 * line termination.  On Unix the CR terminates
			 * the line containing the timecode, and
			 * immediately after the LF terminates an empty
			 * line.  So synthesize the empty LF-terminated
			 * line using the same CR timestamp.  Both CR
			 * and LF are stripped by refclock_gtlin().
			 */
			buff = get_free_recv_buffer_alloc();
			buff->recv_time = cr_time;
			buff->recv_length = 0;
			buff->fd = rio->fd;
			buff->receiver = rio->clock_recv;
			buff->dstadr = NULL;
			buff->recv_peer = rio->srcclock;
			add_full_recv_buffer(buff);
			/*
			 * Now signal we have something to process
			 */
			SetEvent(WaitableIoEventHandle);
			buff = get_free_recv_buffer_alloc();
		}
	}

	QueueSerialWait(rio, buff, lpo, TRUE);

	return 1;
}


/*
 * QueueRawSerialRead() returns TRUE if successful.
 */
static int
QueueRawSerialRead(
	struct refclockio *	rio,
	recvbuf_t *		buff,
	olplus *		lpo
	)
{
	BOOL		rc;

	buff->fd = _get_osfhandle(rio->fd);
	lpo->request_type = RAW_SERIAL_READ;
	lpo->recv_buf = buff;
	rc = ReadFile((HANDLE)buff->fd, buff->recv_buffer,
		      sizeof(buff->recv_buffer), NULL, &lpo->ol);
	if (!rc && ERROR_IO_PENDING != GetLastError()) {
		msyslog(LOG_ERR, "Can't raw read from Refclock: %m");
		freerecvbuf(buff);
		return FALSE;
	}

	return TRUE;
}


static int 
OnRawSerialReadComplete(
	struct refclockio *	rio,
	olplus *		lpo,
	DWORD			octets,
	int			errstatus
	)
{
	recvbuf_t *		rbufp;
	l_fp			arrival_time;

	get_systime(&arrival_time);

	if (!rio->active)
		return FALSE;

	rbufp = lpo->recv_buf;

	/* ignore 0 bytes read. */
	if (NO_ERROR == errstatus && octets > 0) {
		rbufp->recv_length = (int)octets;
		rbufp->dstadr = NULL;
		rbufp->recv_time = arrival_time;
		rbufp->receiver = rio->clock_recv;
		rbufp->recv_peer = rio->srcclock;
		rbufp->fd = rio->fd; /* was handle */
		packets_received++;
		handler_pkts++;
		add_full_recv_buffer(rbufp);
		/*
		 * Now signal we have something to process
		 */
		SetEvent(WaitableIoEventHandle);
		rbufp = get_free_recv_buffer_alloc();
	}

	QueueSerialWait(rio, rbufp, lpo, TRUE);

	return 1;
}


/*  Add a reference clock data structures I/O handles to
 *  the I/O completion port. Return 1 if any error.
 */  
int
io_completion_port_add_clock_io(
	struct refclockio *rio
	)
{
	olplus *	lpo;
	recvbuf_t *	buff;
	HANDLE		h;

	h = (HANDLE)_get_osfhandle(rio->fd);
	if (NULL == CreateIoCompletionPort(
			h, 
			hIoCompletionPort, 
			(ULONG_PTR)rio,
			0)) {
		msyslog(LOG_ERR, "Can't add COM port to i/o completion port: %m");
		return 1;
	}

	lpo = GetHeapAlloc("io_completion_port_add_clock_io");
	if (NULL == lpo) {
		msyslog(LOG_ERR, "Can't allocate heap for completion port: %m");
		return 1;
	}

	buff = get_free_recv_buffer_alloc();
	QueueSerialWait(rio, buff, lpo, TRUE);

	return 0;
}

/*
 * Queue a receiver on a socket. Returns 0 if no buffer can be queued 
 *
 *  Note: As per the winsock documentation, we use WSARecvFrom. Using
 *	  ReadFile() is less efficient.
 */
static unsigned long 
QueueSocketRecv(
	SOCKET s,
	recvbuf_t *buff,
	olplus *lpo
	)
{
	WSABUF wsabuf;
	DWORD Flags;
	DWORD Result;

	lpo->request_type = SOCK_RECV;
	lpo->recv_buf = buff;

	if (buff != NULL) {
		Flags = 0;
		buff->fd = s;
		buff->recv_srcadr_len = sizeof(buff->recv_srcadr);
		wsabuf.buf = (char *)buff->recv_buffer;
		wsabuf.len = sizeof(buff->recv_buffer);

		if (SOCKET_ERROR == WSARecvFrom(buff->fd, &wsabuf, 1, 
						NULL, &Flags, 
						&buff->recv_srcadr.sa, 
						&buff->recv_srcadr_len, 
						&lpo->ol, NULL)) {
			Result = GetLastError();
			switch (Result) {
				case NO_ERROR :
				case WSA_IO_PENDING :
					break ;

				case WSAENOTSOCK :
					msyslog(LOG_ERR, "Can't read from non-socket fd %d: %m", (int)buff->fd);
					/* return the buffer */
					freerecvbuf(buff);
					return 0;
					break;

				case WSAEFAULT :
					msyslog(LOG_ERR, "The buffers parameter is incorrect: %m");
					/* return the buffer */
					freerecvbuf(buff);
					return 0;
				break;

				default :
				  /* nop */ ;
			}
		}
	}
	else 
		return 0;
	return 1;
}


/* Returns 0 if any Error */
static int 
OnSocketRecv(ULONG_PTR i, olplus *lpo, DWORD Bytes, int errstatus)
{
	struct recvbuf *buff = NULL;
	recvbuf_t *newbuff;
	l_fp arrival_time;
	struct interface * inter = (struct interface *) i;
	
	get_systime(&arrival_time);

	NTP_REQUIRE(NULL != lpo);
	NTP_REQUIRE(NULL != lpo->recv_buf);

	/*
	 * Convert the overlapped pointer back to a recvbuf pointer.
	 */
	buff = lpo->recv_buf;

	/*
	 * If the socket is closed we get an Operation Aborted error
	 * Just clean up
	 */
	if (errstatus == WSA_OPERATION_ABORTED)
	{
		freerecvbuf(buff);
		lpo->recv_buf = NULL;
		FreeHeap(lpo, "OnSocketRecv: Socket Closed");
		return (1);
	}

	/*
	 * Get a new recv buffer for the replacement socket receive
	 */
	newbuff = get_free_recv_buffer_alloc();
	QueueSocketRecv(inter->fd, newbuff, lpo);

	DPRINTF(4, ("%sfd %d %s recv packet mode is %d\n", 
		    (MODE_BROADCAST == get_packet_mode(buff))
			? " **** Broadcast "
			: "",
		    (int)buff->fd, stoa(&buff->recv_srcadr),
		    get_packet_mode(buff)));

	/*
	 * If we keep it add some info to the structure
	 */
	if (Bytes && !inter->ignore_packets) {
		NTP_INSIST(buff->recv_srcadr_len <=
			   sizeof(buff->recv_srcadr));
		memcpy(&buff->recv_time, &arrival_time,
		       sizeof(buff->recv_time));	
		buff->recv_length = (int) Bytes;
		buff->receiver = &receive; 
		buff->dstadr = inter;
		packets_received++;
		handler_pkts++;
		inter->received++;
		add_full_recv_buffer(buff);

		DPRINTF(2, ("Received %d bytes fd %d in buffer %p from %s\n", 
			    buff->recv_length, (int)buff->fd, buff,
			    stoa(&buff->recv_srcadr)));

		/*
		 * Now signal we have something to process
		 */
		SetEvent(WaitableIoEventHandle);
	} else
		freerecvbuf(buff);

	return 1;
}


/*  Add a socket handle to the I/O completion port, and send 
 *  NTP_RECVS_PER_SOCKET recv requests to the kernel.
 */
extern int
io_completion_port_add_socket(SOCKET fd, struct interface *inter)
{
	olplus *lpo;
	recvbuf_t *buff;
	int n;

	if (fd != INVALID_SOCKET) {
		if (NULL == CreateIoCompletionPort((HANDLE)fd, 
		    hIoCompletionPort, (ULONG_PTR)inter, 0)) {
			msyslog(LOG_ERR, "Can't add socket to i/o completion port: %m");
			return 1;
		}
	}

	/*
	 * Windows 2000 bluescreens with bugcheck 0x76
	 * PROCESS_HAS_LOCKED_PAGES at ntpd process
	 * termination when using more than one pending
	 * receive per socket.  A runtime version test
	 * would allow using more on newer versions
	 * of Windows.
	 */

#define WINDOWS_RECVS_PER_SOCKET 1

	for (n = 0; n < WINDOWS_RECVS_PER_SOCKET; n++) {

		buff = get_free_recv_buffer_alloc();
		lpo = (olplus *) GetHeapAlloc("io_completion_port_add_socket");
		if (lpo == NULL)
		{
			msyslog(LOG_ERR, "Can't allocate heap for completion port: %m");
			return 1;
		}

		QueueSocketRecv(fd, buff, lpo);

	}
	return 0;
}


static int 
OnWriteComplete(ULONG_PTR i, olplus *lpo, DWORD Bytes, int errstatus)
{
	void *buff;

	UNUSED_ARG(i);
	UNUSED_ARG(Bytes);
	UNUSED_ARG(errstatus);

	buff = lpo->trans_buf;
	free(buff);
	lpo->trans_buf = NULL;
	FreeHeap(lpo, "OnWriteComplete");

	return 1;
}


/*
 * io_completion_port_sendto() -- sendto() replacement for Windows
 *
 * Returns len after successful send.
 * Returns -1 for any error, with the error code available via
 *	msyslog() %m, or GetLastError().
 */
int
io_completion_port_sendto(
	int		fd,
	void  *		pkt,
	size_t		len,
	sockaddr_u *	dest
	)
{
	static u_long time_next_ifscan_after_error;
	WSABUF wsabuf;
	DWORD octets_sent;
	DWORD Result;
	int errval;
	int AddrLen;

	wsabuf.buf = (void *)pkt;
	wsabuf.len = len;
	AddrLen = SOCKLEN(dest);
	octets_sent = 0;

	Result = WSASendTo(fd, &wsabuf, 1, &octets_sent, 0,
			   &dest->sa, AddrLen, NULL, NULL);

	if (SOCKET_ERROR == Result) {
		errval = GetLastError();
		if (ERROR_UNEXP_NET_ERR == errval) {
			/*
			 * We get this error when trying to send if the
			 * network interface is gone or has lost link.
			 * Rescan interfaces to catch on sooner, but no
			 * more often than once per minute.  Once ntpd
			 * is able to detect changes without polling
			 * this should be unneccessary
			 */
			if (time_next_ifscan_after_error < current_time) {
				time_next_ifscan_after_error = current_time + 60;
				timer_interfacetimeout(current_time);
			}
			DPRINTF(4, ("sendto unexpected network error, interface may be down\n"));
		} else {
			msyslog(LOG_ERR, "WSASendTo(%s) error %m",
				stoa(dest));
		}
		SetLastError(errval);
		return -1;
	}

	if (len != (int)octets_sent) {
		msyslog(LOG_ERR, "WSASendTo(%s) sent %u of %d octets",
			stoa(dest), octets_sent, len);
		SetLastError(ERROR_BAD_LENGTH);
		return -1;
	}

	DPRINTF(4, ("sendto %s %d octets\n", stoa(dest), len));

	return len;
}


/*
 * async_write, clone of write(), used by some reflock drivers
 */
int	
async_write(
	int fd,
	const void *data,
	unsigned int count
	)
{
	void *buff;
	olplus *lpo;
	DWORD BytesWritten;

	buff = emalloc(count);
	lpo = GetHeapAlloc("async_write");
	if (lpo == NULL) {
		free(buff);
		DPRINTF(1, ("async_write: out of memory\n"));
		errno = ENOMEM;
		return -1;
	}

	lpo->request_type = SERIAL_WRITE;
	lpo->trans_buf = buff;
	memcpy(buff, data, count);

	if (!WriteFile((HANDLE)_get_osfhandle(fd), buff, count,
		&BytesWritten, &lpo->ol)
		&& ERROR_IO_PENDING != GetLastError()) {

		msyslog(LOG_ERR, "async_write - error %m");
		free(buff);
		FreeHeap(lpo, "async_write");
		errno = EBADF;
		return -1;
	}

	return count;
}


/*
 * GetReceivedBuffers
 * Note that this is in effect the main loop for processing requests
 * both send and receive. This should be reimplemented
 */
int GetReceivedBuffers()
{
	DWORD	index;
	HANDLE	ready;
	int	have_packet;

	have_packet = FALSE;
	while (!have_packet) {
		index = WaitForMultipleObjects(ActiveWaitHandles,
					       WaitHandles, FALSE,
					       INFINITE);
		switch (index) {

		case WAIT_OBJECT_0 + 0: /* Io event */
			DPRINTF(4, ("IoEvent occurred\n"));
			have_packet = TRUE;
			break;

		case WAIT_OBJECT_0 + 1: /* exit request */
			exit(0);
			break;

		case WAIT_OBJECT_0 + 2: /* timer */
			timer();
			break;

		case WAIT_IO_COMPLETION: /* loop */
			break;

		case WAIT_TIMEOUT:
			msyslog(LOG_ERR, "ntpd: WaitForMultipleObjects INFINITE timed out.");
			exit(1);
			break;

		case WAIT_FAILED:
			msyslog(LOG_ERR, "ntpd: WaitForMultipleObjects Failed: Error: %m");
			exit(1);
			break;

		default:
			DEBUG_INSIST((index - WAIT_OBJECT_0) <
				     ActiveWaitHandles);
			ready = WaitHandles[index - WAIT_OBJECT_0];
			handle_blocking_resp_sem(ready);
			break;
				
		} /* switch */
	}

	return (full_recvbuffs());	/* get received buffers */
}

#else
  static int NonEmptyCompilationUnit;
#endif

