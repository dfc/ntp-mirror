#include "libntptest.h"
#include "timestructs.h"

extern "C" {
#include <math.h>
#include "timevalops.h"
}

#include <string>
#include <sstream>

using namespace timeStruct;

class timevalTest : public libntptest {
protected:

	static u_int32 my_tick_to_tsf(u_int32 ticks);
	static u_int32 my_tsf_to_tick(u_int32 tsf);

	static const long MICROSECONDS;
	// that's it...
	struct lfpfracdata {
		long	usec;
		u_int32	frac;
	};
	static const lfpfracdata fdata[];
};

u_int32
timevalTest::my_tick_to_tsf(
	u_int32 ticks
	)
{
	// convert microseconds to l_fp fractional units, using double
	// precision float calculations or, if available, 64bit integer
	// arithmetic. This should give the precise fraction, rounded to
	// the nearest representation.
#if SIZEOF_LONG >= 8
	return (u_int32)((((u_long)ticks << 32) + 500000) / 1000000);
#else
	return (u_int32)floor((double)ticks * 4294.967296 + 0.5);
#endif
	// And before you ask: if ticks >= 1000000, the result is
	// truncated nonsense, so don't use it out-of-bounds.
}

u_int32
timevalTest::my_tsf_to_tick(
	u_int32 tsf
	)
{
	// Inverse operation: converts fraction to microseconds.
#if SIZEOF_LONG >= 8
	return (u_int32)(((u_long)tsf * 1000000 + 0x80000000) >> 32);
#else
	return (u_int32)floor((double)tsf / 4294.967296 + 0.5);
#endif
	// Beware: The result might be 10^6 due to rounding!
}


const long timevalTest::MICROSECONDS = 1000000;
const timevalTest::lfpfracdata timevalTest::fdata [] = {
	{      0, 0x00000000 }, {   7478, 0x01ea1405 },
	{  22077, 0x05a6d699 }, { 125000, 0x20000000 },
	{ 180326, 0x2e29d841 }, { 207979, 0x353e1c9b },
	{ 250000, 0x40000000 }, { 269509, 0x44fe8ab5 },
	{ 330441, 0x5497c808 }, { 333038, 0x5541fa76 },
	{ 375000, 0x60000000 }, { 394734, 0x650d4995 },
	{ 446327, 0x72427c7c }, { 500000, 0x80000000 },
	{ 517139, 0x846338b4 }, { 571953, 0x926b8306 },
	{ 587353, 0x965cc426 }, { 625000, 0xa0000000 },
	{ 692136, 0xb12fd32c }, { 750000, 0xc0000000 },
	{ 834068, 0xd5857aff }, { 848454, 0xd9344806 },
	{ 854222, 0xdaae4b02 }, { 861465, 0xdc88f862 },
	{ 875000, 0xe0000000 }, { 910661, 0xe921144d },
	{ 922162, 0xec12cf10 }, { 942190, 0xf1335d25 }
};


// and the global predicate instances we're using here

static AssertFpClose FpClose(0, 1);

static AssertTimevalClose TimevalClose(0, 1);

// ---------------------------------------------------------------------
// test support stuff
// ---------------------------------------------------------------------

TEST_F(timevalTest, Normalise) {
	for (long ns = -2000000000; ns <= 2000000000; ns += 10000000) {
		timeval_wrap x(0, ns);

		timeval_norm(x);
		ASSERT_TRUE(x.valid());
	}
}

TEST_F(timevalTest, SignNoFrac) {
	// sign test, no fraction
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 0);
		int	     E = (i > 0) - (i < 0);
		int	     r = timeval_test(a);

		ASSERT_EQ(E, r);
	}
}

TEST_F(timevalTest, SignWithFrac) {
	// sign test, with fraction
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 10);
		int	     E = (i >= 0) - (i < 0);
		int	     r = timeval_test(a);

		ASSERT_EQ(E, r);
	}
}

// test compare
TEST_F(timevalTest, CmpFracEQ) {
	// fractions are equal
	for (int i = -4; i <= 4; ++i)
		for (int j = -4; j <= 4; ++j) {
			timeval_wrap a(i, 200);
			timeval_wrap b(j, 200);
			int	     E = (i > j) - (i < j);
			int	     r = timeval_cmp(a, b);

			ASSERT_EQ(E, r);
		}
}

TEST_F(timevalTest, CmpFracGT) {
	// fraction a bigger fraction b
	for (int i = -4; i <= 4; ++i)
		for (int j = -4; j <= 4; ++j) {
			timeval_wrap a( i , 999800);
			timeval_wrap b( j , 200);
			int	     E = (i >= j) - (i < j);
			int	     r = timeval_cmp(a, b);

			ASSERT_EQ(E, r);
		}
}

TEST_F(timevalTest, CmpFracLT) {
	// fraction a less fraction b
	for (int i = -4; i <= 4; ++i)
		for (int j = -4; j <= 4; ++j) {
			timeval_wrap a(i, 200);
			timeval_wrap b(j, 999800);
			int	     E = (i > j) - (i <= j);
			int	     r = timeval_cmp(a, b);

			ASSERT_EQ(E, r);
		}
}

// Test addition (sum)
TEST_F(timevalTest, AddFullNorm) {
	for (int i = -4; i <= 4; ++i)
		for (int j = -4; j <= 4; ++j) {
			timeval_wrap a(i, 200);
			timeval_wrap b(j, 400);
			timeval_wrap E(i + j, 200 + 400);
			timeval_wrap c;

			timeval_add(c, a, b);
			ASSERT_EQ(E, c);
		}
}

TEST_F(timevalTest, AddFullOflow1) {
	for (int i = -4; i <= 4; ++i)
		for (int j = -4; j <= 4; ++j) {
			timeval_wrap a(i, 200);
			timeval_wrap b(j, 999900);
			timeval_wrap E(i + j + 1, 100);
			timeval_wrap c;
			timeval_add(c, a, b);
			ASSERT_EQ(E, c);
		}
}

TEST_F(timevalTest, AddUsecNorm) {
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 200);
		timeval_wrap E(i, 600);
		timeval_wrap c;

		timeval_addus(c, a, 400);
		ASSERT_EQ(E, c);
	}
}

TEST_F(timevalTest, AddUsecOflow1) {
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 200);
		timeval_wrap E(i + 1, 100);
		timeval_wrap c;

		timeval_addus(c, a, MICROSECONDS - 100);
		ASSERT_EQ(E, c);
	}
}

// test subtraction (difference)
TEST_F(timevalTest, SubFullNorm) {
	for (int i = -4; i <= 4; ++i)
		for (int j = -4; j <= 4; ++j) {
			timeval_wrap a(i, 600);
			timeval_wrap b(j, 400);
			timeval_wrap E(i - j, 600 - 400);
			timeval_wrap c;

			timeval_sub(c, a, b);
			ASSERT_EQ(E, c);
		}
}

TEST_F(timevalTest, SubFullOflow) {
	for (int i = -4; i <= 4; ++i)
		for (int j = -4; j <= 4; ++j) {
			timeval_wrap a(i, 100);
			timeval_wrap b(j, 999900);
			timeval_wrap E(i - j - 1, 200);
			timeval_wrap c;

			timeval_sub(c, a, b);
			ASSERT_EQ(E, c);
		}
}

TEST_F(timevalTest, SubUsecNorm) {
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 600);
		timeval_wrap E(i, 200);
		timeval_wrap c;

		timeval_subus(c, a, 600 - 200);
		ASSERT_EQ(E, c);
	}
}

TEST_F(timevalTest, SubUsecOflow) {
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 100);
		timeval_wrap E(i - 1, 200);
		timeval_wrap c;

		timeval_subus(c, a, MICROSECONDS - 100);
		ASSERT_EQ(E, c);
	}
}

// test negation
TEST_F(timevalTest, Neg) {
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 100);
		timeval_wrap b;
		timeval_wrap c;

		timeval_neg(b, a);
		timeval_add(c, a, b);
		ASSERT_EQ(0, timeval_test(c));
	}
}

// test abs value
TEST_F(timevalTest, AbsNoFrac) {
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 0);
		timeval_wrap b;
		int	     c;

		c = timeval_abs(b, a);
		ASSERT_EQ((i < 0), c);
		ASSERT_EQ((i != 0), timeval_test(b));
	}
}

TEST_F(timevalTest, AbsWithFrac) {
	for (int i = -4; i <= 4; ++i) {
		timeval_wrap a(i, 100);
		timeval_wrap b;
		int	     c;

		c = timeval_abs(b, a);
		ASSERT_EQ((i < 0), c);
		ASSERT_EQ(1, timeval_test(b));
	}
}

// conversion to l_fp
TEST_F(timevalTest, ToLFPbittest) {
	for (u_int32 i = 0; i < 1000000; i++) {
		timeval_wrap a(1, i);
		l_fp_wrap    E(1, my_tick_to_tsf(i));
		l_fp_wrap    r;

		timeval_reltolfp(r, a);
		ASSERT_PRED_FORMAT2(FpClose, E, r);
	}
}

TEST_F(timevalTest, ToLFPrelPos) {
	for (int i = 0; i < COUNTOF(fdata); i++) {
		timeval_wrap a(1, fdata[i].usec);
		l_fp_wrap    E(1, fdata[i].frac);
		l_fp_wrap    r;

		timeval_reltolfp(r, a);
		ASSERT_PRED_FORMAT2(FpClose, E, r);
	}
}

TEST_F(timevalTest, ToLFPrelNeg) {
	for (int i = 0; i < COUNTOF(fdata); i++) {
		timeval_wrap a(-1, fdata[i].usec);
		l_fp_wrap    E(~0, fdata[i].frac);
		l_fp_wrap    r;

		timeval_reltolfp(r, a);
		ASSERT_PRED_FORMAT2(FpClose, E, r);
	}
}

TEST_F(timevalTest, ToLFPabs) {
	for (int i = 0; i < COUNTOF(fdata); i++) {
		timeval_wrap a(1, fdata[i].usec);
		l_fp_wrap    E(1 + JAN_1970, fdata[i].frac);
		l_fp_wrap    r;

		timeval_abstolfp(r, a);
		ASSERT_PRED_FORMAT2(FpClose, E, r);
	}
}

// conversion from l_fp
TEST_F(timevalTest, FromLFPbittest) {
	// Not *exactly* a bittest, because 2**32 tests would take a
	// really long time even on very fast machines! So we do test
	// every 1000 fractional units.
	for (u_int32 tsf = 0; tsf < ~u_int32(1000); tsf += 1000) {
		timeval_wrap E(1, my_tsf_to_tick(tsf));
		l_fp_wrap    a(1, tsf);
		timeval_wrap r;

		timeval_relfromlfp(r, a);

		// The conversion might be off by one microsecond when
		// comparing to calculated value; the table-driven
		// conversion does not use all bits from the FP fraction
		// and this truncation will affect the rounding.
		ASSERT_PRED_FORMAT2(TimevalClose, E, r);
	}
}

// usec -> frac -> usec roundtrip
TEST_F(timevalTest, LFProundtrip) {
	for (u_int32 i = 0; i < 1000000; i++) {
		timeval_wrap E(1, i);
		l_fp_wrap    a;
		timeval_wrap r;

		timeval_reltolfp(a, E);
		timeval_relfromlfp(r, a);
		ASSERT_EQ(E, r);
	}
}

TEST_F(timevalTest, ToString) {
	static const struct {
		time_t	     sec;
		long	     usec;
		const char * repr;
	} data [] = {
		{ 0, 0,	 "0.000000" },
		{ 2, 0,	 "2.000000" },
		{-2, 0, "-2.000000" },
		{ 0, 1,	 "0.000001" },
		{ 0,-1,	"-0.000001" },
		{ 1,-1,	 "0.999999" },
		{-1, 1, "-0.999999" },
		{-1,-1, "-1.000001" },
	};
	for (int i = 0; i < COUNTOF(data); ++i) {
		timeval_wrap a(data[i].sec, data[i].usec);
		std::string  E(data[i].repr);
		std::string  r(timeval_tostr(a));

		ASSERT_EQ(E, r);
	}
}

// -*- EOF -*-
