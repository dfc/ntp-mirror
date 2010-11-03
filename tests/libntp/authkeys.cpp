/* This file contains test for both libntp/authkeys.c and libntp/authusekey.c */

#include "libntptest.h"

extern "C" {
#include "ntp.h"
#include "ntp_stdlib.h"
};

// This declaration does not exist in ntp_stdlib.h
extern u_short cache_flags;

class authkeysTest : public libntptest {
protected:
	static const int KEYTYPE = KEY_TYPE_MD5;

	virtual void SetUp() {
		init_auth();

		/*
		 * init_auth() does not initialize global variables like authnumkeys,
		 * so let's reset them to zero here.
		 */
		authnumkeys = 0;

		/*
		 * Especially, empty the key cache!
		 */
		cache_keyid = 0;
		cache_type = 0;
		cache_flags = 0;
		cache_key = NULL;
		cache_keylen = 0;
	}

	void AddTrustedKey(keyid_t keyno) {
		/*
		 * We need to add a MD5-key in addition to setting the
		 * trust, because authhavekey() requires type != 0.
		 */
		MD5auth_setkey(keyno, KEYTYPE, NULL, 0);

		authtrust(keyno, 1);
	}

	void AddUntrustedKey(keyid_t keyno) {
		authtrust(keyno, 0);
	}
};

TEST_F(authkeysTest, AddTrustedKeys) {
	const keyid_t KEYNO1 = 5;
	const keyid_t KEYNO2 = 8;

	AddTrustedKey(KEYNO1);
	AddTrustedKey(KEYNO2);

	EXPECT_TRUE(authistrusted(KEYNO1));
	EXPECT_TRUE(authistrusted(KEYNO2));
}

TEST_F(authkeysTest, AddUntrustedKey) {
	const keyid_t KEYNO = 3;
   
	AddUntrustedKey(KEYNO);

	EXPECT_FALSE(authistrusted(KEYNO));
}

TEST_F(authkeysTest, HaveKeyCorrect) {
	const keyid_t KEYNO = 3;

	AddTrustedKey(KEYNO);

	EXPECT_TRUE(auth_havekey(KEYNO));
	EXPECT_TRUE(authhavekey(KEYNO));
}

TEST_F(authkeysTest, HaveKeyIncorrect) {
	const keyid_t KEYNO = 2;

	EXPECT_FALSE(auth_havekey(KEYNO));
	EXPECT_FALSE(authhavekey(KEYNO));
}

TEST_F(authkeysTest, AddWithAuthUseKey) {
	const keyid_t KEYNO = 5;
	const char* KEY = "52a";

	EXPECT_TRUE(authusekey(KEYNO, KEYTYPE, (u_char*)KEY));	
}

TEST_F(authkeysTest, EmptyKey) {
	const keyid_t KEYNO = 3;
	const char* KEY = "";


	EXPECT_FALSE(authusekey(KEYNO, KEYTYPE, (u_char*)KEY));
}
