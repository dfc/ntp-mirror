/*
 * ntp_calendar.c - calendar and helper functions
 *
 * Written by Juergen Perlinger (perlinger@ntp.org) for the NTP project.
 * The contents of 'html/copyright.html' apply.
 */
#include <config.h>
#include <sys/types.h>

#include "ntp_types.h"
#include "ntp_calendar.h"
#include "ntp_stdlib.h"
#include "ntp_fp.h"
#include "ntp_unixtime.h"

/*
 *---------------------------------------------------------------------
 * basic calendar stuff
 * --------------------------------------------------------------------
 */

/* month table for a year starting with March,1st */
static const u_short shift_month_table[13] = {
	0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337, 366
};

/* month tables for years starting with January,1st; regular & leap */
static const u_short real_month_table[2][13] = {
	/* -*- table for regular years -*- */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
	/* -*- table for leap years -*- */
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

/*
 * Some notes on the terminology:
 *
 * We use the proleptic Gregorian calendar, which is the Gregorian
 * calendar extended in both directions ad infinitum. This totally
 * disregards the fact that this calendar was invented in 1582, and
 * was adopted at various dates over the world; sometimes even after
 * the start of the NTP epoch.
 *
 * Normally date parts are given as current cycles, while time parts
 * are given as elapsed cycles:
 *
 * 1970-01-01/03:04:05 means 'IN the 1970st. year, IN the first month,
 * ON the first day, with 3hrs, 4minutes and 5 seconds elapsed.
 *
 * The basic calculations for this calendar implementation deal with
 * ELAPSED date units, which is the number of full years, full months
 * and full days before a date: 1970-01-01 would be (1969, 0, 0) in
 * that notation.
 *
 * To ease the numeric computations, month and day values outside the
 * normal range are acceptable: 2001-03-00 will be treated as the day
 * before 2001-03-01, 2000-13-32 will give the same result as
 * 2001-02-01 and so on.
 *
 * 'rd' or 'RD' is used as an abbreviation for the latin 'rata die'
 * (day number).  This is the number of days elapsed since 0000-12-31
 * in the proleptic Gregorian calendar. The begin of the Christian Era
 * (0001-01-01) is RD(1).
 *
 * 
 * Some notes on the implementation:
 *
 * Calendar algorithms thrive on the division operation, which is one of
 * the slowest numerical operations in any CPU. What saves us here from
 * abysmal performance is the fact that all divisions are divisions by
 * constant numbers, and most compilers can do this by a multiplication
 * operation.  But this might not work when using the div/ldiv/lldiv
 * function family, because many compilers are not able to do inline
 * expansion of the code with following optimisation for the
 * constant-divider case.
 *
 * Also div/ldiv/lldiv are defined in terms of int/long/longlong, which
 * are inherently target dependent. Nothing that could not be cured with
 * autoconf, but still a mess...
 *
 * Furhermore, we need floor division while C demands truncation to
 * zero, so additional steps are required to make sure the algorithms
 * work.
 *
 * For all this, all divisions by constant are coded manually, even when
 * there is a joined div/mod operation: The optimiser should sort that
 * out, if possible.
 *
 * Furthermore, the functions do not check for overflow conditions. This
 * is a sacrifice made for execution speed; since a 32-bit day counter
 * covers +/- 5,879,610 years, this should not pose a problem here.
 */


/*
 * ==================================================================
 *
 * General algorithmic stuff
 *
 * ==================================================================
 */

/*
 *---------------------------------------------------------------------
 * Do a periodic extension of 'value' around 'pivot' with a period of
 * 'cycle'.
 *
 * The result 'res' is a number that holds to the following properties:
 *
 *   1)	 res MOD cycle == value MOD cycle
 *   2)	 pivot <= res < pivot + cycle
 *	 (replace </<= with >/>= for negative cycles)
 *
 * where 'MOD' denotes the modulo operator for FLOOR DIVISION, which
 * is not the same as the '%' operator in C: C requires division to be
 * a truncated division, where remainder and dividend have the same
 * sign if the remainder is not zero, whereas floor division requires
 * divider and modulus to have the same sign for a non-zero modulus.
 *
 * This function has some useful applications:
 *
 * + let Y be a calendar year and V a truncated 2-digit year: then
 *	periodic_extend(Y-50, V, 100)
 *   is the closest expansion of the truncated year with respect to
 *   the full year, that is a 4-digit year with a difference of less
 *   than 50 years to the year Y. ("century unfolding")
 *
 * + let T be a UN*X time stamp and V be seconds-of-day: then
 *	perodic_extend(T-43200, V, 86400)
 *   is a time stamp that has the same seconds-of-day as the input
 *   value, with an absolute difference to T of <= 12hrs.  ("day
 *   unfolding")
 *
 * + Wherever you have a truncated periodic value and a non-truncated
 *   base value and you want to match them somehow...
 *
 * Basically, the function delivers 'pivot + (value - pivot) % cycle',
 * but the implementation takes some pains to avoid internal signed
 * integer overflows in the '(value - pivot) % cycle' part and adheres
 * to the floor division convention.
 *
 * If 64bit scalars where available on all intended platforms, writing a
 * version that uses 64 bit ops would be easy; writing a general
 * division routine for 64bit ops on a platform that can only do
 * 32/16bit divisions and is still performant is a bit more
 * difficult. Since most usecases can be coded in a way that does only
 * require the 32-bit version a 64bit version is NOT provided here.
 * ---------------------------------------------------------------------
 */
int32
ntpcal_periodic_extend(
	int32 pivot,
	int32 value,
	int32 cycle)
{
	u_int32 diff;
	char	cpl = 0; /* modulo complement flag	   */
	char	neg = 0; /* sign change flag	   */

	/* make the cycle positive and adjust the flags */ 
	if (cycle < 0) {
		cycle = - cycle;
		neg ^= 1;
		cpl ^= 1;
	}
	/* guard against div by zero or one */
	if (cycle > 1) {
		/*
		 * Get absolute difference as unsigned quantity and
		 * the complement flag. This is done by always
		 * subtracting the smaller value from the bigger
		 * one. This implementation works only on a two's
		 * complement machine!
		 */
		if (value >= pivot) {
			diff = (u_int32)value - (u_int32)pivot;
		} else {
			diff = (u_int32)pivot - (u_int32)value;
			cpl ^= 1;
		}
		diff %= (u_int32)cycle;
		if (diff) {
			if (cpl)
				diff = cycle - diff;
			if (neg)
				diff = ~diff + 1;
			pivot += diff;
		}
	}
	return pivot;
}

/*
 *-------------------------------------------------------------------
 * Convert a timestamp in NTP scale to a 64bit seconds value in the UN*X
 * scale with proper epoch unfolding around a given pivot or the current
 * system time. This function happily accepts negative pivot values as
 * timestamps befor 1970-01-01, so be aware of possible trouble on
 * platforms with 32bit 'time_t'!
 *
 * This is also a periodic extension, but since the cycle is 2^32 and
 * the shift is 2^31, we can do some *very* fast math without explicit
 * divisions.
 *-------------------------------------------------------------------
 */
vint64
ntpcal_ntp_to_time(
	u_int32	      ntp  ,
	const time_t *pivot)
{
	vint64 res;

#ifdef HAVE_INT64

	res.q_s = pivot ? *pivot : time(NULL);
	
	res.Q_s -= 0x80000000u;		/* unshift of half range */
	ntp	-= (u_int32)JAN_1970;	/* warp into UN*X domain */
	ntp	-= res.D_s.lo;		/* cycle difference	 */
	res.Q_s += (u_int64)ntp;		/* get expanded time	 */

#else /* no 64bit scalars */
	
	time_t tmp = pivot ? *pivot : time(NULL);

	/*
	 * shifting negative signed quantities is compiler-dependent, so
	 * we better avoid it and do it all manually. And shifting more
	 * than the rgisterwidth is undefined. Also a don't do!
	 */
#   if SIZEOF_TIME_T > 4
	if (tmp < 0) {
		tmp = -tmp;
		res.D_s.lo = (u_int32)tmp;
		res.D_s.hi = (u_int32)(tmp >> 32);
		M_NEG(res.D_s.hi, res.D_s.lo);
	} else {
		res.D_s.lo = (u_int32)tmp;
		res.D_s.hi = (u_int32)(tmp >> 32);
	}
#   else
	res.D_s.lo = (u_int32)(int32)tmp;
	res.D_s.hi = (tmp < 0) ? ~(u_int32)0 : 0;
#   endif /* sizeof time_t <= 4 */
	M_SUB(res.D_s.hi, res.D_s.lo, 0, 0x80000000u);
	ntp -= (u_int32)JAN_1970;	/* warp into UN*X domain */
	ntp -= res.D_s.lo;		/* cycle difference	 */
	M_ADD(res.D_s.hi, res.D_s.lo, 0, ntp);
#endif /* no 64bit scalars */

	return res;
}

/*
 *-------------------------------------------------------------------
 * Convert a timestamp in NTP scale to a 64bit seconds value in the NTP
 * scale with proper epoch unfolding around a given pivot or the current
 * system time.
 *
 * Note: The pivot must be given in the UN*X time domain!
 *
 * This is also a periodic extension, but since the cycle is 2^32 and
 * the shift is 2^31, we can do some *very* fast math without explicit
 * divisions.
 *-------------------------------------------------------------------
 */
vint64
ntpcal_ntp_to_ntp(
	u_int32	      ntp  ,
	const time_t *pivot)
{
	vint64 res;

#ifdef HAVE_INT64

	res.q_s = pivot ? *pivot : time(NULL);

	res.Q_s -= 0x80000000u;		/* unshift of half range */
	res.Q_s += (u_int32)JAN_1970;	/* warp into NTP domain	 */
	ntp	-= res.D_s.lo;		/* cycle difference	 */
	res.Q_s += (u_int64)ntp;		/* get expanded time	 */

#else /* no 64bit scalars */
	
	time_t tmp = pivot ? *pivot : time(NULL);

	/*
	 * shifting negative signed quantities is compiler-dependent, so
	 * we better avoid it and do it all manually. And shifting more
	 * than the rgisterwidth is undefined. Also a don't do!
	 */
#   if SIZEOF_TIME_T > 4
	if (tmp < 0) {
		tmp = -tmp;
		res.D_s.lo = (u_int32)tmp;
		res.D_s.hi = (u_int32)(tmp >> 32);
		M_NEG(res.D_s.hi, res.D_s.lo);
	} else {
		res.D_s.lo = (u_int32)tmp;
		res.D_s.hi = (u_int32)(tmp >> 32);
	}
#   else
	res.D_s.lo = (u_int32)(int32)tmp;
	res.D_s.hi = (tmp < 0) ? ~(u_int32)0 : 0;
#   endif
	M_SUB(res.D_s.hi, res.D_s.lo, 0, 0x80000000u);
	M_ADD(res.D_s.hi, res.D_s.lo, 0, (u_int32)JAN_1970);/*into NTP */
	ntp -= res.D_s.lo;		/* cycle difference	 */
	M_ADD(res.D_s.hi, res.D_s.lo, 0, ntp);

#endif /* no 64bit scalars */

	return res;
}


/*
 * ==================================================================
 *
 * Splitting values to composite entities
 *
 * ==================================================================
 */

/*
 *------------------------------------------------------------------- 
 * Split a 64bit seconds value into elapsed days in 'res.hi' and
 * elapsed seconds since midnight in 'res.lo' using explicit floor
 * division. This function happily accepts negative time values as
 * timestamps before the respective epoch start.
 * -------------------------------------------------------------------
 */
ntpcal_split
ntpcal_daysplit(
	const vint64 *ts)
{
	ntpcal_split res;

#ifdef HAVE_INT64

	/* manual floor division by SECSPERDAY */
	res.hi = (int32)(ts->q_s / SECSPERDAY);
	res.lo = (int32)(ts->q_s % SECSPERDAY);
	if (res.lo < 0) {
		res.hi -= 1;
		res.lo += SECSPERDAY;
	}

#else

	/*
	 * since we do not have 64bit ops, we have to this by hand.
	 * Luckily SECSPERDAY is 86400 is 675*128, so we do the division
	 * using chained 32/16 bit divisions and shifts.
	 */
	vint64	op;
	u_int32	q, r, a;
	int	isneg;

	memcpy(&op, ts, sizeof(op));
	/* fix sign */
	if ((isneg = M_ISNEG(op.D_s.hi, op.D_s.lo)) != 0)
		M_NEG(op.D_s.hi, op.D_s.lo);
		
	/* save remainder of DIV 128, shift for divide */
	r  = op.D_s.lo & 127; /* save remainder bits */
	op.D_s.lo = (op.D_s.lo >> 7) | (op.D_s.hi << 25);
	op.D_s.hi = (op.D_s.hi >> 7);

	/* now do a mnual division, trying to remove as many ops as
	 * possible -- division is always slow! An since we do not have
	 * the advantage of a specic 64/32 bit or even a specific 32/16
	 * bit division op but must use the general 32/32bit division
	 * even if we *know* the divider fits into unsigned 16 bits the
	 * exra code pathes should pay off.
	 */
	if ((a = op.D_s.hi) > 675u)
		a = a % 675u;
	if (a) {
		a = (a << 16) | op.W_s.lh;
		q = a / 675u;
		a = a % 675u;

		a = (a << 16) | op.W_s.ll;
		q = (q << 16) | (a / 675u);
	} else {
		a = op.D_s.lo;
		q = a / 675u;
	}
	a = a % 675u;

	/* assemble remainder */
	r |= a << 7;

	/* fix sign of result */
	if (isneg) {
		if (r) {
			r = SECSPERDAY - r;
			q = ~q;
		} else
			q = ~q + 1;
	}
	
	res.hi = q;
	res.lo = r;

#endif	
	return res;
}

/*
 *------------------------------------------------------------------- 
 * Split a 32bit seconds value into h/m/s and excessive days.  This
 * function happily accepts negative time values as timestamps before
 * midnight.
 * -------------------------------------------------------------------
 */
static int32
priv_timesplit(
	int   split[3],
	int32 ts	    )
{
	int32 days = 0;

	/* make sure we have a positive offset into a day */
	if (ts < 0 || ts >= SECSPERDAY) {
		days = ts / SECSPERDAY;
		ts   = ts % SECSPERDAY;
		if (ts < 0) {
			days -= 1;
			ts   += SECSPERDAY;
		}
	}

	/* get secs, mins, hours */
	split[2] = (u_char)(ts % SECSPERMIN);
	ts /= SECSPERMIN;
	split[1] = (u_char)(ts % MINSPERHR);
	split[0] = (u_char)(ts / MINSPERHR);

	return days;
}

/*
 * ---------------------------------------------------------------------
 * Given the number of elapsed days in the calendar era, split this
 * number into the number of elapsed years in 'res.hi' and the number
 * of elapsed days of that year in 'res.lo'.
 *
 * if 'isleapyear' is not NULL, it will receive an integer that is 0 for
 * regular years and a non-zero value for leap years.
 *---------------------------------------------------------------------
 */
ntpcal_split
ntpcal_split_eradays(
	int32 days	,
	int  *isleapyear)
{
	ntpcal_split res;
	int32	     n400, n100, n004, n001, yday; /* calendar year cycles */
	
	/*
	 * Split off calendar cycles, using floor division in the first
	 * step. After that first step, simple division does it because
	 * all operands are positive; alas, we have to be aware of the
	 * possibe cycle overflows for 100 years and 1 year, caused by
	 * the additional leap day.
	 */
	n400 = days / GREGORIAN_CYCLE_DAYS;
	yday = days % GREGORIAN_CYCLE_DAYS;
	if (yday < 0) {
		n400 -= 1;
		yday += GREGORIAN_CYCLE_DAYS;
	}
	n100 = yday / GREGORIAN_NORMAL_CENTURY_DAYS;
	yday = yday % GREGORIAN_NORMAL_CENTURY_DAYS;
	n004 = yday / GREGORIAN_NORMAL_LEAP_CYCLE_DAYS;
	yday = yday % GREGORIAN_NORMAL_LEAP_CYCLE_DAYS;
	n001 = yday / DAYSPERYEAR;
	yday = yday % DAYSPERYEAR;
	
	/*
	 * check for leap cycle overflows and calculate the leap flag
	 * if needed
	 */
	if ((n001 | n100) > 3) {
		/* hit last day of leap year */
		n001 -= 1;
		yday += DAYSPERYEAR;
		if (isleapyear)
			*isleapyear = 1;
	} else if (isleapyear)
		*isleapyear = (n001 == 3) && ((n004 != 24) || (n100 == 3));
	
	/* now merge the cycles to elapsed years, using horner scheme */
	res.hi = ((4*n400 + n100)*25 + n004)*4 + n001;
	res.lo = yday;
	
	return res;
}

/*
 *---------------------------------------------------------------------
 * Given a number of elapsed days in a year and a leap year indicator,
 * split the number of elapsed days into the number of elapsed months in
 * 'res.hi' and the number of elapsed days of that month in 'res.lo'.
 *
 * This function will fail and return {-1,-1} if the number of elapsed
 * days is not in the valid range!
 *---------------------------------------------------------------------
 */
ntpcal_split
ntpcal_split_yeardays(
	int32 eyd	,
	int   isleapyear)
{
	ntpcal_split   res;
	const u_short *lt;	/* month length table	*/

	/* check leap year flag and select proper table */
	lt = real_month_table[(isleapyear != 0)];
	if (0 <= eyd && eyd < lt[12]) {
		/* get zero-based month by approximation & correction step */
		res.hi = eyd >> 5;	   /* approx month; might be 1 too low */
		if (lt[res.hi + 1] <= eyd) /* fixup approximative month value  */
			res.hi += 1;
		res.lo = eyd - lt[res.hi];
	} else
		res.lo = res.hi = -1;

	return res;
}

/*
 *---------------------------------------------------------------------
 * Convert a RD into the date part of a 'struct calendar'.
 *---------------------------------------------------------------------
 */
int
ntpcal_rd_to_date(
	struct calendar *jd,
	int32		 rd)
{
	ntpcal_split split;
	int	     leaps = 0;
	int	     retv  = 0;

	/* get day-of-week first */
	jd->weekday = rd % 7;
	if (jd->weekday >= 7)	/* unsigned! */
		jd->weekday += 7;

	split = ntpcal_split_eradays(rd - 1, &leaps);
	retv  = leaps;
	/* get year and day-of-year */
	jd->year = (u_short)split.hi + 1;
	if (jd->year != split.hi + 1) {
		jd->year = 0;
		retv	 = -1;	/* bletch. overflow trouble. */
	}
	jd->yearday = (u_short)split.lo + 1;

	/* convert to month and mday */
	split = ntpcal_split_yeardays(split.lo, leaps);
	jd->month    = (u_char)split.hi + 1;
	jd->monthday = (u_char)split.lo + 1;

	return retv ? retv : leaps;
}

/*
 *---------------------------------------------------------------------
 * Convert a RD into the date part of a 'struct tm'.
 *---------------------------------------------------------------------
 */
int
ntpcal_rd_to_tm(
	struct tm  *utm,
	int32	    rd )
{
	ntpcal_split split;
	int	     leaps = 0;

	/* get day-of-week first */
	utm->tm_wday = rd % 7;
	if (utm->tm_wday < 0)
		utm->tm_wday += 7;

	/* get year and day-of-year */
	split = ntpcal_split_eradays(rd - 1, &leaps);
	utm->tm_year = split.hi - 1899;
	utm->tm_yday = split.lo;	/* 0-based */

	/* convert to month and mday */
	split = ntpcal_split_yeardays(split.lo, leaps);
	utm->tm_mon  = split.hi;	/* 0-based */
	utm->tm_mday = split.lo + 1;	/* 1-based */

	return leaps;
}

/*
 *---------------------------------------------------------------------
 * Take a value of seconds since midnight and split it into hhmmss in a
 * 'struct calendar'.
 *---------------------------------------------------------------------
 */
int32
ntpcal_daysec_to_date(
	struct calendar *jd,
	int32		sec)
{
	int32 days;
	int   ts[3];
	
	days = priv_timesplit(ts, sec);
	jd->hour   = (u_char)ts[0];
	jd->minute = (u_char)ts[1];
	jd->second = (u_char)ts[2];

	return days;
}

/*
 *---------------------------------------------------------------------
 * Take a value of seconds since midnight and split it into hhmmss in a
 * 'struct tm'.
 *---------------------------------------------------------------------
 */
int32
ntpcal_daysec_to_tm(
	struct tm *utm,
	int32	   sec)
{
	int32 days;
	int   ts[3];
	
	days = priv_timesplit(ts, sec);
	utm->tm_hour = ts[0];
	utm->tm_min  = ts[1];
	utm->tm_sec  = ts[2];

	return days;
}

/*
 *---------------------------------------------------------------------
 * take a split representation for day/second-of-day and day offset
 * and convert it to a 'struct calendar'. The seconds will be normalised
 * into the range of a day, and the day will be adjusted accordingly.
 *
 * returns >0 if the result is in a leap year, 0 if in a regular
 * year and <0 if the result did not fit into the calendar struct.
 *---------------------------------------------------------------------
 */
int
ntpcal_daysplit_to_date(
	struct calendar	   *jd ,
	const ntpcal_split *ds ,
	int32		    dof)
{
	dof += ntpcal_daysec_to_date(jd, ds->lo);
	return ntpcal_rd_to_date(jd, ds->hi + dof);
}

/*
 *---------------------------------------------------------------------
 * take a split representation for day/second-of-day and day offset
 * and convert it to a 'struct tm'. The seconds will be normalised
 * into the range of a day, and the day will be adjusted accordingly.
 *
 * returns 1 if the result is in a leap year and zero if in a regular
 * year.
 *---------------------------------------------------------------------
 */
int
ntpcal_daysplit_to_tm(
	struct tm	   *utm,
	const ntpcal_split *ds ,
	int32		    dof)
{
	dof += ntpcal_daysec_to_tm(utm, ds->lo);
	return ntpcal_rd_to_tm(utm, ds->hi + dof);
}

/*
 *---------------------------------------------------------------------
 * Take a UN*X time and convert to a calendar structure.
 *---------------------------------------------------------------------
 */
int
ntpcal_time_to_date(
	struct calendar	*jd,
	const vint64	*ts)
{
	ntpcal_split ds;

	ds = ntpcal_daysplit(ts);
	ds.hi += ntpcal_daysec_to_date(jd, ds.lo);
	ds.hi += DAY_UNIX_STARTS;
	return ntpcal_rd_to_date(jd, ds.hi);
}


/*
 * ==================================================================
 *
 * merging composite entities
 *
 * ==================================================================
 */

/*
 *---------------------------------------------------------------------
 * Merge a number of days and a number of seconds into seconds,
 * expressed in 64 bits to avoid overflow.
 *---------------------------------------------------------------------
 */
vint64
ntpcal_dayjoin(
	int32 days,
	int32 secs)
{
	vint64 res;

#ifdef HAVE_INT64

	res.q_s	 = days;
	res.q_s *= SECSPERDAY;
	res.q_s += secs;

#else

	u_int32 p1, p2;
	int	isneg;

	/* res = days *86400 + secs, using manual 16/32 bit
	 * multiplications and shifts.
	 */
	if ((isneg = (days < 0)) != 0)
		days = -days;

	/* assemble days * 675 */
	res.D_s.lo = (days & 0xFFFF) * 675u;
	res.D_s.hi = 0;
	p1 = (days >> 16) * 675u;
	p2 = p1 >> 16;
	p1 = p1 << 16;
	M_ADD(res.D_s.hi, res.D_s.lo, p2, p1);

	/* mul by 128, using shift */
	res.D_s.hi = (res.D_s.hi << 7) | (res.D_s.lo >> 25);
	res.D_s.lo = (res.D_s.lo << 7);

	/* fix sign */
	if (isneg)
		M_NEG(res.D_s.hi, res.D_s.lo);
	
	/* properly add seconds */
	p1 = secs;
	p2 = (secs < 0) ? ~(u_int32)0 : 0;
	M_ADD(res.D_s.hi, res.D_s.lo, p2, p1);

#endif	

	return res;
}

/*
 *---------------------------------------------------------------------
 * Convert elapsed years in Era into elapsed days in Era.
 *
 * To accomodate for negative values of years, floor division would be
 * required for all division operations. This can be eased by first
 * splitting the years into full 400-year cycles and years in the
 * cycle. Only this operation must be coded as a full floor division; as
 * the years in the cycle is a non-negative number, all other divisions
 * can be regular truncated divisions.
 *---------------------------------------------------------------------
 */
int32
ntpcal_days_in_years(
	int32 years)
{
	int32 cycle; /* full gregorian cycle */

	/* split off full calendar cycles, using floor division */
	cycle = years / 400;
	years = years % 400;
	if (years < 0) {
		cycle -= 1;
		years += 400;
	}

	/*
	 * Calculate days in cycle. years now is a non-negative number,
	 * holding the number of years in the 400-year cycle.
	 */
	return cycle * GREGORIAN_CYCLE_DAYS
	     + years * DAYSPERYEAR	/* days inregular years	*/
	     + years / 4		/* 4 year leap rule	*/
	     - years / 100;		/* 100 year leap rule	*/
	/* the 400-year rule does not apply due to full-cycle split-off */
}

/*
 *---------------------------------------------------------------------
 * Convert a number of elapsed month in a year into elapsed days in year.
 *
 * The month will be normalized, and 'res.hi' will contain the
 * excessive years that must be considered when converting the years,
 * while 'res.lo' will contain the number of elapsed days since start
 * of the year.
 *
 * This code uses the shifted-month-approach to convert month to days,
 * because then there is no need to have explicit leap year
 * information.	 The slight disadvantage is that for most month values
 * the result is a negative value, and the year excess is one; the
 * conversion is then simply based on the start of the following year.
 *---------------------------------------------------------------------
 */
ntpcal_split
ntpcal_days_in_months(
	int32 m)
{
	ntpcal_split res;

	/* normalize month into range */
	res.hi = 0;
	res.lo = m;
	if (res.lo < 0 || res.lo >= 12) {
		res.hi = res.lo / 12;
		res.lo = res.lo % 12;
		if (res.lo < 0) {
			res.hi -= 1;
			res.lo += 12;
		}
	}

	/* add 10 month for year starting with march */
	if (res.lo < 2)
		res.lo += 10;
	else {
		res.hi += 1;
		res.lo -= 2;
	}

	/* get cummulated days in year with unshift */
	res.lo = shift_month_table[res.lo] - 306;

	return res;
}

/*
 *---------------------------------------------------------------------
 * Convert ELAPSED years/months/days of gregorian calendar to elapsed
 * days in Gregorian epoch.
 *
 * If you want to convert years and days-of-year, just give a month of
 * zero.
 *---------------------------------------------------------------------
 */
int32
ntpcal_edate_to_eradays(
	int32 years  ,
	int32 months ,
	int32 mdays  )
{
	ntpcal_split tmp;
	int32	     res;

	if (months) {
		tmp = ntpcal_days_in_months(months);
		res = ntpcal_days_in_years(years + tmp.hi) + tmp.lo;
	} else
		res = ntpcal_days_in_years(years);
	res += mdays;

	return res;
}

/*
 *---------------------------------------------------------------------
 * Convert ELAPSED years/months/days of gregorian calendar to elapsed
 * days in year.
 *
 * Note: This will give the true difference to the start of the given year,
 * even if months & days are off-scale.
 *---------------------------------------------------------------------
 */
int32
ntpcal_edate_to_yeardays(
	int32 years ,
	int32 months,
	int32 mdays )
{
	ntpcal_split tmp;

	if (0 <= months && months < 12) {
		years += 1;
		mdays += real_month_table[is_leapyear(years)][months];
	} else {
		tmp = ntpcal_days_in_months(months);
		mdays += tmp.lo
		       + ntpcal_days_in_years(years + tmp.hi)
		       - ntpcal_days_in_years(years);
	}
	return mdays;
}

/*
 *---------------------------------------------------------------------
 * Convert elapsed days and the hour/minute/second information into
 * total seconds.
 *
 * If 'isvalid' is not NULL, do a range check on the time specification
 * and tell if the time input is in the normal range, permitting for a
 * single leapsecond.
 *---------------------------------------------------------------------
 */
int32
ntpcal_etime_to_seconds(
	int32 hours  ,
	int32 minutes,
	int32 seconds)
{
	int32 res;

	res = (hours * MINSPERHR + minutes) * SECSPERMIN + seconds;

	return res;
}

/*
 *---------------------------------------------------------------------
 * Convert the date part of a 'struct tm' (that is, year, month,
 * day-of-month) into the RD of that day.
 *---------------------------------------------------------------------
 */
int32
ntpcal_tm_to_rd(
	const struct tm *utm)
{
	return ntpcal_edate_to_eradays(utm->tm_year + 1899,
				       utm->tm_mon,
				       utm->tm_mday - 1) + 1;
}

/*
 *---------------------------------------------------------------------
 * Convert the date part of a 'struct calendar' (that is, year, month,
 * day-of-month) into the RD of that day.
 *---------------------------------------------------------------------
 */
int32
ntpcal_date_to_rd(
	const struct calendar *jd)
{
	return ntpcal_edate_to_eradays((int32)jd->year - 1,
				       (int32)jd->month - 1,
				       (int32)jd->monthday - 1) + 1;
}

/*
 *---------------------------------------------------------------------
 * convert a year number to rata die of year start
 *---------------------------------------------------------------------
 */
int32
ntpcal_year_to_ystart(
	int32 year)
{
	return ntpcal_days_in_years(year - 1) + 1;
}

/*
 *---------------------------------------------------------------------
 * For a given RD, get the RD of the associated year start,
 * that is, the RD of the last January,1st on or before that day.
 *---------------------------------------------------------------------
 */
int32
ntpcal_rd_to_ystart(
	int32 rd)
{
	/*
	 * Rather simple exercise: split the day number into elapsed
	 * years and elapsed days, then remove the elapsed days from the
	 * input value. Nice'n sweet...
	 */
	return rd - ntpcal_split_eradays(rd - 1, NULL).lo;
}

/*
 *---------------------------------------------------------------------
 * For a given RD, get the RD of the associated month start.
 *---------------------------------------------------------------------
 */
int32
ntpcal_rd_to_mstart(
	int32 rd)
{
	ntpcal_split split;
	int	     leaps;

	split = ntpcal_split_eradays( rd - 1 , &leaps);
	split = ntpcal_split_yeardays(split.lo,	 leaps);
	return rd - split.lo;
}

/*
 *---------------------------------------------------------------------
 * take a 'struct calendar' and get the seconds-of-day from it.
 *---------------------------------------------------------------------
 */
int32
ntpcal_date_to_daysec(
	const struct calendar *jd)
{
	return ntpcal_etime_to_seconds(jd->hour, jd->minute, jd->second);
}

/*
 *---------------------------------------------------------------------
 * take a 'struct tm' and get the seconds-of-day from it.
 *---------------------------------------------------------------------
 */
int32
ntpcal_tm_to_daysec(
	const struct tm *utm)
{
	return ntpcal_etime_to_seconds(utm->tm_hour, utm->tm_min, utm->tm_sec);
}

/*
 * ==================================================================
 *
 * extended and uncecked variants of caljulian/caltontp
 *
 * ==================================================================
 */
int
ntpcal_ntp_to_date(
	struct calendar *jd ,
	u_int32		 ntp,
	const time_t	*piv)
{
	vint64	     vl;
	ntpcal_split ds;
	
	/*
	 * Unfold ntp time around current time into NTP domain. Split
	 * into days and seconds, shift days into CE domain and
	 * process the parts.
	 */
	vl = ntpcal_ntp_to_ntp(ntp, piv);
	ds = ntpcal_daysplit(&vl);
	ds.hi += ntpcal_daysec_to_date(jd, ds.lo);
	return ntpcal_rd_to_date(jd, ds.hi + DAY_NTP_STARTS);
}


u_int32
ntpcal_date_to_ntp(
	const struct calendar *jd)
{
	/*
	 * Convert date to NTP. Ignore yearday, use d/m/y only.
	 */
	return ntpcal_dayjoin(ntpcal_date_to_rd(jd) - DAY_NTP_STARTS,
			      ntpcal_date_to_daysec(jd)).d_s.lo;
}

/*
 * ==================================================================
 *
 * day-of-week calculations
 *
 * ==================================================================
 */
/*
 * Given a RataDie and a day-of-week, calculate a RDN that is reater-than,
 * greater-or equal, closest, less-or-equal or less-than the given RDN
 * and denotes the given day-of-week
 */
int32
ntpcal_weekday_gt(
	int32 rdn,
	int32 dow)
{
	return ntpcal_periodic_extend(rdn+1, dow, 7);
}

int32
ntpcal_weekday_ge(
	int32 rdn,
	int32 dow)
{
	return ntpcal_periodic_extend(rdn, dow, 7);
}

int32
ntpcal_weekday_close(
	int32 rdn,
	int32 dow)
{
	return ntpcal_periodic_extend(rdn-3, dow, 7);
}

int32
ntpcal_weekday_le(
	int32 rdn,
	int32 dow)
{
	return ntpcal_periodic_extend(rdn, dow, -7);
}

int32
ntpcal_weekday_lt(
	int32 rdn,
	int32 dow)
{
	return ntpcal_periodic_extend(rdn-1, dow, -7);
}

/*
 * ==================================================================
 *
 * ISO week-calendar conversions
 *
 * The ISO8601 calendar defines a calendar of years, weeks and weekdays.
 * It is related to the Gregorian calendar, and a ISO year starts at the
 * Monday closest to Jan,1st of the corresponding Gregorian year.  A ISO
 * calendar year has always 52 or 53 weeks, and like the Grogrian
 * calendar the ISO8601 calendar repeats itself every 400 years, or
 * 146097 days, or 20871 weeks.
 *
 * While it is possible to write ISO calendar functions based on the
 * Gregorian calendar functions, the following implementation takes a
 * different approach, based directly on years and weeks.
 *
 * Analysis of the tabulated data shows that it is not possible to
 * interpolate from years to weeks over a full 400 year range; cyclic
 * shifts over 400 years do not provide a solution here. But it *is*
 * possible to interpolate over every single century of the 400-year
 * cycle. (The centennial leap year rule seems to be the culprit here.)
 *
 * It can be shown that a conversion from years to weeks can be done
 * using a linear transformation of the form
 *
 *   w = floor( y * a + b )
 *
 * where the slope a must hold to
 *
 *  52.1780821918 <= a < 52.1791044776
 *
 * and b must be chosen according to the selected slope and the number
 * of the century in a 400-year period.
 *
 * The inverse calculation can also be done in this way. Careful scaling
 * provides an unlimited set of integer coefficients a,k,b that enable
 * us to write the calulation in the form
 *
 *   w = (y * a	 + b ) / k
 *   y = (w * a' + b') / k'
 *
 * In this implementation the values of k and k' are chosen to be
 * smallest possible powers of two, so the division can be implemented
 * as shifts if the optimiser chooses to do so.
 *
 * ==================================================================
 */

/*
 * Given a number of elapsed (ISO-)years since the begin of the
 * christian era, return the number of elapsed weeks corresponding to
 * the number of years.
 */
int32
isocal_weeks_in_years(
	int32 years)
{
	/*
	 * use: w = (y * 53431 + b[c]) / 1024 as interpolation
	 */
	static const int32 bctab[4] = { 449, 157, 889, 597 };
	
	int32 cycle; /* full gregorian cycle */
	int32 cents; /* full centuries	   */
	int32 weeks; /* accumulated weeks	   */

	/* split off full calendar cycles, using floor division */
	cycle = years / 400;
	years = years % 400;
	if (years < 0) {
		cycle -= 1;
		years += 400;
	}

	/* split off full centuries */
	cents = years / 100;
	years = years % 100;

	/*
	 * calculate elapsed weeks, taking into account that the
	 * first, third and fourth century have 5218 weeks but the
	 * second century falls short by one week.
	 */
	weeks = (years * 53431 + bctab[cents]) / 1024;

	return cycle * GREGORIAN_CYCLE_WEEKS
	     + cents * 5218 - (cents > 1)
	     + weeks;
}

/*
 * Given a number of elapsed weeks since the begin of the christian
 * era, split this number into the number of elapsed years in res.hi
 * and the excessive number of weeks in res.lo. (That is, res.lo is
 * the number of elapsed weeks in the remaining partial year.)
 */
ntpcal_split
isocal_split_eraweeks(
	int32 weeks)
{
	/*
	 * use: y = (w * 157 + b[c]) / 8192 as interpolation
	 */
	
	static const int32 bctab[4] = { 85, 131, 17, 62 };
	
	ntpcal_split res;
	int32	     cents;

	/*
	 * split off 400-year cycles, using the fact that a 400-year
	 * cycle has 146097 days, which is exactly 20871 weeks.
	 */
	res.hi = weeks / GREGORIAN_CYCLE_WEEKS;
	res.lo = weeks % GREGORIAN_CYCLE_WEEKS;
	if (res.lo < 0) {
		res.hi -= 1;
		res.lo += GREGORIAN_CYCLE_WEEKS;
	}
	res.hi *= 400;

	/*
	 * split off centuries, taking into account that the first,
	 * third and fourth century have 5218 weeks but that the
	 * second century falls short by one week.
	 */
	res.lo += (res.lo >= 10435);
	cents	= res.lo / 5218;
	res.lo %= 5218;		/* res.lo is weeks in century now */
	
	/* convert elapsed weeks in century to elapsed years and weeks */
	res.lo	= res.lo * 157 + bctab[cents];
	res.hi += cents * 100 + res.lo / 8192;
	res.lo	= (res.lo % 8192) / 157;	
	
	return res;
}

/*
 * Given a second in the NTP time scale and a pivot, expand the NTP
 * time stamp around the pivot and convert into an ISO calendar time
 * stamp.
 */
int
isocal_ntp_to_date(
	struct isodate *id ,
	u_int32		ntp,
	const time_t   *piv)
{
	vint64	     vl;
	ntpcal_split ds;
	int	     ts[3];
	
	/*
	 * Unfold ntp time around current time into NTP domain. Split
	 * into days and seconds, shift days into CE domain and
	 * process the parts.
	 */
	vl = ntpcal_ntp_to_ntp(ntp, piv);
	ds = ntpcal_daysplit(&vl);

	/* split time part */
	ds.hi += priv_timesplit(ts, ds.lo);
	id->hour   = (u_char)ts[0];
	id->minute = (u_char)ts[1];
	id->second = (u_char)ts[2];

	/* split date part */
	ds.lo = ds.hi + DAY_NTP_STARTS - 1;	/* elapsed era days  */
	ds.hi = ds.lo / 7;			/* elapsed era weeks */
	ds.lo = ds.lo % 7;			/* elapsed week days */
	if (ds.lo < 0) {			/* floor division!   */
		ds.hi -= 1;
		ds.lo += 7;
	}
	id->weekday = (u_char)ds.lo + 1;	/* weekday result    */

	ds = isocal_split_eraweeks(ds.hi);	/* elapsed years&week*/
	id->year = (u_short)ds.hi + 1;		/* shift to current  */
	id->week = (u_char)ds.lo + 1;

	return (ds.hi >= 0 && ds.hi < 0xFFFFU);
}

/*
 * Convert a ISO date spec into a second in the NTP time scale,
 * properly truncated to 32 bit.
 */
u_int32
isocal_date_to_ntp(
	const struct isodate *id)
{
	int32 weeks, days, secs;

	weeks = isocal_weeks_in_years((int32)id->year - 1)
	      + (int32)id->week - 1;
	days = weeks * 7 + (int32)id->weekday;
	/* days is RDN of ISO date now */
	secs = ntpcal_etime_to_seconds(id->hour, id->minute, id->second);
	return ntpcal_dayjoin(days - DAY_NTP_STARTS, secs).d_s.lo;
}

/* -*-EOF-*- */
