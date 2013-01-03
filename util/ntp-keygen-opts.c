/*  
 *  EDIT THIS FILE WITH CAUTION  (ntp-keygen-opts.c)
 *  
 *  It has been AutoGen-ed  January  3, 2013 at 01:10:51 PM by AutoGen 5.17.1pre11
 *  From the definitions    ntp-keygen-opts.def
 *  and the template file   options
 *
 * Generated from AutoOpts 37:0:12 templates.
 *
 *  AutoOpts is a copyrighted work.  This source file is not encumbered
 *  by AutoOpts licensing, but is provided under the licensing terms chosen
 *  by the ntp-keygen author or copyright holder.  AutoOpts is
 *  licensed under the terms of the LGPL.  The redistributable library
 *  (``libopts'') is licensed under the terms of either the LGPL or, at the
 *  users discretion, the BSD license.  See the AutoOpts and/or libopts sources
 *  for details.
 *
 * The ntp-keygen program is copyrighted and licensed
 * under the following terms:
 *
 *  Copyright (C) 1970-2013 The University of Delaware, all rights reserved.
 *  This is free software. It is licensed for use, modification and
 *  redistribution under the terms of the NTP License, copies of which
 *  can be seen at:
 *    <http://ntp.org/license>
 *    <http://opensource.org/licenses/ntp-license.php>
 *
 *  Permission to use, copy, modify, and distribute this software and its
 *  documentation for any purpose with or without fee is hereby granted,
 *  provided that the above copyright notice appears in all copies and that
 *  both the copyright notice and this permission notice appear in
 *  supporting documentation, and that the name The University of Delaware not be used in
 *  advertising or publicity pertaining to distribution of the software
 *  without specific, written prior permission. The University of Delaware makes no
 *  representations about the suitability this software for any purpose. It
 *  is provided "as is" without express or implied warranty.
 */

#ifndef __doxygen__
#define OPTION_CODE_COMPILE 1
#include "ntp-keygen-opts.h"
#include <sys/types.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef  __cplusplus
extern "C" {
#endif
extern FILE * option_usage_fp;

/* TRANSLATORS: choose the translation for option names wisely because you
                cannot ever change your mind. */
#define zCopyright      (ntp_keygen_opt_strs+0)
#define zLicenseDescrip (ntp_keygen_opt_strs+326)

/*
 *  global included definitions
 */
#include <stdlib.h>

#ifdef __windows
  extern int atoi(const char*);
#else
# include <stdlib.h>
#endif

#ifndef NULL
#  define NULL 0
#endif

/*
 *  ntp-keygen option static const strings
 */
static char const ntp_keygen_opt_strs[2358] =
/*     0 */ "ntp-keygen (ntp) 4.2.7p344\n"
            "Copyright (C) 1970-2013 The University of Delaware, all rights reserved.\n"
            "This is free software. It is licensed for use, modification and\n"
            "redistribution under the terms of the NTP License, copies of which\n"
            "can be seen at:\n"
            "  <http://ntp.org/license>\n"
            "  <http://opensource.org/licenses/ntp-license.php>\n\0"
/*   326 */ "Permission to use, copy, modify, and distribute this software and its\n"
            "documentation for any purpose with or without fee is hereby granted,\n"
            "provided that the above copyright notice appears in all copies and that\n"
            "both the copyright notice and this permission notice appear in supporting\n"
            "documentation, and that the name The University of Delaware not be used in\n"
            "advertising or publicity pertaining to distribution of the software without\n"
            "specific, written prior permission.  The University of Delaware makes no\n"
            "representations about the suitability this software for any purpose.  It is\n"
            "provided \"as is\" without express or implied warranty.\n\0"
/*   966 */ "identity modulus bits\0"
/*   988 */ "IMBITS\0"
/*   995 */ "imbits\0"
/*  1002 */ "certificate scheme\0"
/*  1021 */ "CERTIFICATE\0"
/*  1033 */ "certificate\0"
/*  1045 */ "privatekey cipher\0"
/*  1063 */ "CIPHER\0"
/*  1070 */ "cipher\0"
/*  1077 */ "Increase debug verbosity level\0"
/*  1108 */ "DEBUG_LEVEL\0"
/*  1120 */ "debug-level\0"
/*  1132 */ "Set the debug verbosity level\0"
/*  1162 */ "SET_DEBUG_LEVEL\0"
/*  1178 */ "set-debug-level\0"
/*  1194 */ "Write IFF or GQ identity keys\0"
/*  1224 */ "ID_KEY\0"
/*  1231 */ "id-key\0"
/*  1238 */ "Generate GQ parameters and keys\0"
/*  1270 */ "GQ_PARAMS\0"
/*  1280 */ "gq-params\0"
/*  1290 */ "generate RSA host key\0"
/*  1312 */ "HOST_KEY\0"
/*  1321 */ "host-key\0"
/*  1330 */ "generate IFF parameters\0"
/*  1354 */ "IFFKEY\0"
/*  1361 */ "iffkey\0"
/*  1368 */ "set Autokey group name\0"
/*  1391 */ "IDENT\0"
/*  1397 */ "ident\0"
/*  1403 */ "set certificate lifetime\0"
/*  1428 */ "LIFETIME\0"
/*  1437 */ "lifetime\0"
/*  1446 */ "generate MD5 keys\0"
/*  1464 */ "MD5KEY\0"
/*  1471 */ "md5key\0"
/*  1478 */ "modulus\0"
/*  1486 */ "MODULUS\0"
/*  1494 */ "generate PC private certificate\0"
/*  1526 */ "PVT_CERT\0"
/*  1535 */ "pvt-cert\0"
/*  1544 */ "output private password\0"
/*  1568 */ "PVT_PASSWD\0"
/*  1579 */ "pvt-passwd\0"
/*  1590 */ "input private password\0"
/*  1613 */ "GET_PVT_PASSWD\0"
/*  1628 */ "get-pvt-passwd\0"
/*  1643 */ "generate sign key (RSA or DSA)\0"
/*  1674 */ "SIGN_KEY\0"
/*  1683 */ "sign-key\0"
/*  1692 */ "set host and optionally group name\0"
/*  1727 */ "SUBJECT_NAME\0"
/*  1740 */ "subject-name\0"
/*  1753 */ "trusted certificate (TC scheme)\0"
/*  1785 */ "TRUSTED_CERT\0"
/*  1798 */ "trusted-cert\0"
/*  1811 */ "generate <num> MV parameters\0"
/*  1840 */ "MV_PARAMS\0"
/*  1850 */ "mv-params\0"
/*  1860 */ "update <num> MV keys\0"
/*  1881 */ "MV_KEYS\0"
/*  1889 */ "mv-keys\0"
/*  1897 */ "Display extended usage information and exit\0"
/*  1941 */ "help\0"
/*  1946 */ "Extended usage information passed thru pager\0"
/*  1991 */ "more-help\0"
/*  2001 */ "Output version information and exit\0"
/*  2037 */ "version\0"
/*  2045 */ "Save the option state to a config file\0"
/*  2084 */ "save-opts\0"
/*  2094 */ "Load options from a config file\0"
/*  2126 */ "LOAD_OPTS\0"
/*  2136 */ "no-load-opts\0"
/*  2149 */ "no\0"
/*  2152 */ "NTP_KEYGEN\0"
/*  2163 */ "ntp-keygen (ntp) - Create a NTP host key - Ver. 4.2.7p344\n"
            "USAGE:  %s [ -<flag> [<val>] | --<name>[{=| }<val>] ]...\n\0"
/*  2279 */ "$HOME\0"
/*  2285 */ ".\0"
/*  2287 */ ".ntprc\0"
/*  2294 */ "http://bugs.ntp.org, bugs@ntp.org\0"
/*  2328 */ "\n\n\0"
/*  2331 */ "ntp-keygen (ntp) 4.2.7p344";

/*
 *  imbits option description:
 */
#ifdef AUTOKEY
#define IMBITS_DESC      (ntp_keygen_opt_strs+966)
#define IMBITS_NAME      (ntp_keygen_opt_strs+988)
#define IMBITS_name      (ntp_keygen_opt_strs+995)
#define IMBITS_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

#else   /* disable imbits */
#define IMBITS_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define IMBITS_NAME      NULL
#define IMBITS_DESC      NULL
#define IMBITS_name      NULL
#endif  /* AUTOKEY */

/*
 *  certificate option description:
 */
#ifdef AUTOKEY
#define CERTIFICATE_DESC      (ntp_keygen_opt_strs+1002)
#define CERTIFICATE_NAME      (ntp_keygen_opt_strs+1021)
#define CERTIFICATE_name      (ntp_keygen_opt_strs+1033)
#define CERTIFICATE_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

#else   /* disable certificate */
#define CERTIFICATE_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define CERTIFICATE_NAME      NULL
#define CERTIFICATE_DESC      NULL
#define CERTIFICATE_name      NULL
#endif  /* AUTOKEY */

/*
 *  cipher option description:
 */
#ifdef AUTOKEY
#define CIPHER_DESC      (ntp_keygen_opt_strs+1045)
#define CIPHER_NAME      (ntp_keygen_opt_strs+1063)
#define CIPHER_name      (ntp_keygen_opt_strs+1070)
#define CIPHER_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

#else   /* disable cipher */
#define CIPHER_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define CIPHER_NAME      NULL
#define CIPHER_DESC      NULL
#define CIPHER_name      NULL
#endif  /* AUTOKEY */

/*
 *  debug-level option description:
 */
#define DEBUG_LEVEL_DESC      (ntp_keygen_opt_strs+1077)
#define DEBUG_LEVEL_NAME      (ntp_keygen_opt_strs+1108)
#define DEBUG_LEVEL_name      (ntp_keygen_opt_strs+1120)
#define DEBUG_LEVEL_FLAGS     (OPTST_DISABLED)

/*
 *  set-debug-level option description:
 */
#define SET_DEBUG_LEVEL_DESC      (ntp_keygen_opt_strs+1132)
#define SET_DEBUG_LEVEL_NAME      (ntp_keygen_opt_strs+1162)
#define SET_DEBUG_LEVEL_name      (ntp_keygen_opt_strs+1178)
#define SET_DEBUG_LEVEL_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

/*
 *  id-key option description:
 */
#ifdef AUTOKEY
#define ID_KEY_DESC      (ntp_keygen_opt_strs+1194)
#define ID_KEY_NAME      (ntp_keygen_opt_strs+1224)
#define ID_KEY_name      (ntp_keygen_opt_strs+1231)
#define ID_KEY_FLAGS     (OPTST_DISABLED)

#else   /* disable id-key */
#define ID_KEY_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define ID_KEY_NAME      NULL
#define ID_KEY_DESC      NULL
#define ID_KEY_name      NULL
#endif  /* AUTOKEY */

/*
 *  gq-params option description:
 */
#ifdef AUTOKEY
#define GQ_PARAMS_DESC      (ntp_keygen_opt_strs+1238)
#define GQ_PARAMS_NAME      (ntp_keygen_opt_strs+1270)
#define GQ_PARAMS_name      (ntp_keygen_opt_strs+1280)
#define GQ_PARAMS_FLAGS     (OPTST_DISABLED)

#else   /* disable gq-params */
#define GQ_PARAMS_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define GQ_PARAMS_NAME      NULL
#define GQ_PARAMS_DESC      NULL
#define GQ_PARAMS_name      NULL
#endif  /* AUTOKEY */

/*
 *  host-key option description:
 */
#ifdef AUTOKEY
#define HOST_KEY_DESC      (ntp_keygen_opt_strs+1290)
#define HOST_KEY_NAME      (ntp_keygen_opt_strs+1312)
#define HOST_KEY_name      (ntp_keygen_opt_strs+1321)
#define HOST_KEY_FLAGS     (OPTST_DISABLED)

#else   /* disable host-key */
#define HOST_KEY_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define HOST_KEY_NAME      NULL
#define HOST_KEY_DESC      NULL
#define HOST_KEY_name      NULL
#endif  /* AUTOKEY */

/*
 *  iffkey option description:
 */
#ifdef AUTOKEY
#define IFFKEY_DESC      (ntp_keygen_opt_strs+1330)
#define IFFKEY_NAME      (ntp_keygen_opt_strs+1354)
#define IFFKEY_name      (ntp_keygen_opt_strs+1361)
#define IFFKEY_FLAGS     (OPTST_DISABLED)

#else   /* disable iffkey */
#define IFFKEY_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define IFFKEY_NAME      NULL
#define IFFKEY_DESC      NULL
#define IFFKEY_name      NULL
#endif  /* AUTOKEY */

/*
 *  ident option description:
 */
#ifdef AUTOKEY
#define IDENT_DESC      (ntp_keygen_opt_strs+1368)
#define IDENT_NAME      (ntp_keygen_opt_strs+1391)
#define IDENT_name      (ntp_keygen_opt_strs+1397)
#define IDENT_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

#else   /* disable ident */
#define IDENT_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define IDENT_NAME      NULL
#define IDENT_DESC      NULL
#define IDENT_name      NULL
#endif  /* AUTOKEY */

/*
 *  lifetime option description:
 */
#ifdef AUTOKEY
#define LIFETIME_DESC      (ntp_keygen_opt_strs+1403)
#define LIFETIME_NAME      (ntp_keygen_opt_strs+1428)
#define LIFETIME_name      (ntp_keygen_opt_strs+1437)
#define LIFETIME_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

#else   /* disable lifetime */
#define LIFETIME_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define LIFETIME_NAME      NULL
#define LIFETIME_DESC      NULL
#define LIFETIME_name      NULL
#endif  /* AUTOKEY */

/*
 *  md5key option description:
 */
#define MD5KEY_DESC      (ntp_keygen_opt_strs+1446)
#define MD5KEY_NAME      (ntp_keygen_opt_strs+1464)
#define MD5KEY_name      (ntp_keygen_opt_strs+1471)
#define MD5KEY_FLAGS     (OPTST_DISABLED)

/*
 *  modulus option description:
 */
#ifdef AUTOKEY
#define MODULUS_DESC      (ntp_keygen_opt_strs+1478)
#define MODULUS_NAME      (ntp_keygen_opt_strs+1486)
#define MODULUS_name      (ntp_keygen_opt_strs+1478)
#define MODULUS_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

#else   /* disable modulus */
#define MODULUS_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define MODULUS_NAME      NULL
#define MODULUS_DESC      NULL
#define MODULUS_name      NULL
#endif  /* AUTOKEY */

/*
 *  pvt-cert option description:
 */
#ifdef AUTOKEY
#define PVT_CERT_DESC      (ntp_keygen_opt_strs+1494)
#define PVT_CERT_NAME      (ntp_keygen_opt_strs+1526)
#define PVT_CERT_name      (ntp_keygen_opt_strs+1535)
#define PVT_CERT_FLAGS     (OPTST_DISABLED)

#else   /* disable pvt-cert */
#define PVT_CERT_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define PVT_CERT_NAME      NULL
#define PVT_CERT_DESC      NULL
#define PVT_CERT_name      NULL
#endif  /* AUTOKEY */

/*
 *  pvt-passwd option description:
 */
#ifdef AUTOKEY
#define PVT_PASSWD_DESC      (ntp_keygen_opt_strs+1544)
#define PVT_PASSWD_NAME      (ntp_keygen_opt_strs+1568)
#define PVT_PASSWD_name      (ntp_keygen_opt_strs+1579)
#define PVT_PASSWD_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

#else   /* disable pvt-passwd */
#define PVT_PASSWD_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define PVT_PASSWD_NAME      NULL
#define PVT_PASSWD_DESC      NULL
#define PVT_PASSWD_name      NULL
#endif  /* AUTOKEY */

/*
 *  get-pvt-passwd option description:
 */
#ifdef AUTOKEY
#define GET_PVT_PASSWD_DESC      (ntp_keygen_opt_strs+1590)
#define GET_PVT_PASSWD_NAME      (ntp_keygen_opt_strs+1613)
#define GET_PVT_PASSWD_name      (ntp_keygen_opt_strs+1628)
#define GET_PVT_PASSWD_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

#else   /* disable get-pvt-passwd */
#define GET_PVT_PASSWD_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define GET_PVT_PASSWD_NAME      NULL
#define GET_PVT_PASSWD_DESC      NULL
#define GET_PVT_PASSWD_name      NULL
#endif  /* AUTOKEY */

/*
 *  sign-key option description:
 */
#ifdef AUTOKEY
#define SIGN_KEY_DESC      (ntp_keygen_opt_strs+1643)
#define SIGN_KEY_NAME      (ntp_keygen_opt_strs+1674)
#define SIGN_KEY_name      (ntp_keygen_opt_strs+1683)
#define SIGN_KEY_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

#else   /* disable sign-key */
#define SIGN_KEY_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define SIGN_KEY_NAME      NULL
#define SIGN_KEY_DESC      NULL
#define SIGN_KEY_name      NULL
#endif  /* AUTOKEY */

/*
 *  subject-name option description:
 */
#ifdef AUTOKEY
#define SUBJECT_NAME_DESC      (ntp_keygen_opt_strs+1692)
#define SUBJECT_NAME_NAME      (ntp_keygen_opt_strs+1727)
#define SUBJECT_NAME_name      (ntp_keygen_opt_strs+1740)
#define SUBJECT_NAME_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

#else   /* disable subject-name */
#define SUBJECT_NAME_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define SUBJECT_NAME_NAME      NULL
#define SUBJECT_NAME_DESC      NULL
#define SUBJECT_NAME_name      NULL
#endif  /* AUTOKEY */

/*
 *  trusted-cert option description:
 */
#ifdef AUTOKEY
#define TRUSTED_CERT_DESC      (ntp_keygen_opt_strs+1753)
#define TRUSTED_CERT_NAME      (ntp_keygen_opt_strs+1785)
#define TRUSTED_CERT_name      (ntp_keygen_opt_strs+1798)
#define TRUSTED_CERT_FLAGS     (OPTST_DISABLED)

#else   /* disable trusted-cert */
#define TRUSTED_CERT_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define TRUSTED_CERT_NAME      NULL
#define TRUSTED_CERT_DESC      NULL
#define TRUSTED_CERT_name      NULL
#endif  /* AUTOKEY */

/*
 *  mv-params option description:
 */
#ifdef AUTOKEY
#define MV_PARAMS_DESC      (ntp_keygen_opt_strs+1811)
#define MV_PARAMS_NAME      (ntp_keygen_opt_strs+1840)
#define MV_PARAMS_name      (ntp_keygen_opt_strs+1850)
#define MV_PARAMS_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

#else   /* disable mv-params */
#define MV_PARAMS_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define MV_PARAMS_NAME      NULL
#define MV_PARAMS_DESC      NULL
#define MV_PARAMS_name      NULL
#endif  /* AUTOKEY */

/*
 *  mv-keys option description:
 */
#ifdef AUTOKEY
#define MV_KEYS_DESC      (ntp_keygen_opt_strs+1860)
#define MV_KEYS_NAME      (ntp_keygen_opt_strs+1881)
#define MV_KEYS_name      (ntp_keygen_opt_strs+1889)
#define MV_KEYS_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

#else   /* disable mv-keys */
#define MV_KEYS_FLAGS     (OPTST_OMITTED | OPTST_NO_INIT)
#define MV_KEYS_NAME      NULL
#define MV_KEYS_DESC      NULL
#define MV_KEYS_name      NULL
#endif  /* AUTOKEY */

/*
 *  Help/More_Help/Version option descriptions:
 */
#define HELP_DESC       (ntp_keygen_opt_strs+1897)
#define HELP_name       (ntp_keygen_opt_strs+1941)
#ifdef HAVE_WORKING_FORK
#define MORE_HELP_DESC  (ntp_keygen_opt_strs+1946)
#define MORE_HELP_name  (ntp_keygen_opt_strs+1991)
#define MORE_HELP_FLAGS (OPTST_IMM | OPTST_NO_INIT)
#else
#define MORE_HELP_DESC  NULL
#define MORE_HELP_name  NULL
#define MORE_HELP_FLAGS (OPTST_OMITTED | OPTST_NO_INIT)
#endif
#ifdef NO_OPTIONAL_OPT_ARGS
#  define VER_FLAGS     (OPTST_IMM | OPTST_NO_INIT)
#else
#  define VER_FLAGS     (OPTST_SET_ARGTYPE(OPARG_TYPE_STRING) | \
                         OPTST_ARG_OPTIONAL | OPTST_IMM | OPTST_NO_INIT)
#endif
#define VER_DESC        (ntp_keygen_opt_strs+2001)
#define VER_name        (ntp_keygen_opt_strs+2037)
#define SAVE_OPTS_DESC  (ntp_keygen_opt_strs+2045)
#define SAVE_OPTS_name  (ntp_keygen_opt_strs+2084)
#define LOAD_OPTS_DESC     (ntp_keygen_opt_strs+2094)
#define LOAD_OPTS_NAME     (ntp_keygen_opt_strs+2126)
#define NO_LOAD_OPTS_name  (ntp_keygen_opt_strs+2136)
#define LOAD_OPTS_pfx      (ntp_keygen_opt_strs+2149)
#define LOAD_OPTS_name     (NO_LOAD_OPTS_name + 3)
/*
 *  Declare option callback procedures
 */
#ifdef AUTOKEY
  static tOptProc doOptImbits;
#else /* not AUTOKEY */
# define doOptImbits NULL
#endif /* def/not AUTOKEY */
#ifdef AUTOKEY
  static tOptProc doOptModulus;
#else /* not AUTOKEY */
# define doOptModulus NULL
#endif /* def/not AUTOKEY */
#if defined(TEST_NTP_KEYGEN_OPTS)
/*
 *  Under test, omit argument processing, or call optionStackArg,
 *  if multiple copies are allowed.
 */
static tOptProc
    doUsageOpt;

/*
 *  #define map the "normal" callout procs to the test ones...
 */
#define DEBUG_LEVEL_OPT_PROC optionStackArg


#else /* NOT defined TEST_NTP_KEYGEN_OPTS */
/*
 *  When not under test, there are different procs to use
 */
extern tOptProc
    ntpOptionPrintVersion, optionBooleanVal,      optionNestedVal,
    optionNumericVal,      optionPagedUsage,      optionResetOpt,
    optionStackArg,        optionTimeDate,        optionTimeVal,
    optionUnstackArg,      optionVendorOption,    optionVersionStderr;
static tOptProc
    doOptDebug_Level, doUsageOpt;

/*
 *  #define map the "normal" callout procs
 */
#define DEBUG_LEVEL_OPT_PROC doOptDebug_Level

#endif /* defined(TEST_NTP_KEYGEN_OPTS) */
#define VER_PROC        ntpOptionPrintVersion

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 *  Define the ntp-keygen Option Descriptions.
 * This is an array of OPTION_CT entries, one for each
 * option that the ntp-keygen program responds to.
 */
static tOptDesc optDesc[OPTION_CT] = {
  {  /* entry idx, value */ 0, VALUE_OPT_IMBITS,
     /* equiv idx, value */ 0, VALUE_OPT_IMBITS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ IMBITS_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --imbits */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doOptImbits,
     /* desc, NAME, name */ IMBITS_DESC, IMBITS_NAME, IMBITS_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 1, VALUE_OPT_CERTIFICATE,
     /* equiv idx, value */ 1, VALUE_OPT_CERTIFICATE,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ CERTIFICATE_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --certificate */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ CERTIFICATE_DESC, CERTIFICATE_NAME, CERTIFICATE_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 2, VALUE_OPT_CIPHER,
     /* equiv idx, value */ 2, VALUE_OPT_CIPHER,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ CIPHER_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --cipher */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ CIPHER_DESC, CIPHER_NAME, CIPHER_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 3, VALUE_OPT_DEBUG_LEVEL,
     /* equiv idx, value */ 3, VALUE_OPT_DEBUG_LEVEL,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, NOLIMIT, 0,
     /* opt state flags  */ DEBUG_LEVEL_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --debug-level */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ DEBUG_LEVEL_OPT_PROC,
     /* desc, NAME, name */ DEBUG_LEVEL_DESC, DEBUG_LEVEL_NAME, DEBUG_LEVEL_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 4, VALUE_OPT_SET_DEBUG_LEVEL,
     /* equiv idx, value */ 4, VALUE_OPT_SET_DEBUG_LEVEL,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, NOLIMIT, 0,
     /* opt state flags  */ SET_DEBUG_LEVEL_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --set-debug-level */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ optionNumericVal,
     /* desc, NAME, name */ SET_DEBUG_LEVEL_DESC, SET_DEBUG_LEVEL_NAME, SET_DEBUG_LEVEL_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 5, VALUE_OPT_ID_KEY,
     /* equiv idx, value */ 5, VALUE_OPT_ID_KEY,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ ID_KEY_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --id-key */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ ID_KEY_DESC, ID_KEY_NAME, ID_KEY_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 6, VALUE_OPT_GQ_PARAMS,
     /* equiv idx, value */ 6, VALUE_OPT_GQ_PARAMS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ GQ_PARAMS_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --gq-params */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ GQ_PARAMS_DESC, GQ_PARAMS_NAME, GQ_PARAMS_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 7, VALUE_OPT_HOST_KEY,
     /* equiv idx, value */ 7, VALUE_OPT_HOST_KEY,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ HOST_KEY_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --host-key */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ HOST_KEY_DESC, HOST_KEY_NAME, HOST_KEY_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 8, VALUE_OPT_IFFKEY,
     /* equiv idx, value */ 8, VALUE_OPT_IFFKEY,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ IFFKEY_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --iffkey */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ IFFKEY_DESC, IFFKEY_NAME, IFFKEY_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 9, VALUE_OPT_IDENT,
     /* equiv idx, value */ 9, VALUE_OPT_IDENT,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ IDENT_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --ident */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ IDENT_DESC, IDENT_NAME, IDENT_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 10, VALUE_OPT_LIFETIME,
     /* equiv idx, value */ 10, VALUE_OPT_LIFETIME,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ LIFETIME_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --lifetime */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ optionNumericVal,
     /* desc, NAME, name */ LIFETIME_DESC, LIFETIME_NAME, LIFETIME_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 11, VALUE_OPT_MD5KEY,
     /* equiv idx, value */ 11, VALUE_OPT_MD5KEY,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ MD5KEY_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --md5key */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ MD5KEY_DESC, MD5KEY_NAME, MD5KEY_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 12, VALUE_OPT_MODULUS,
     /* equiv idx, value */ 12, VALUE_OPT_MODULUS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ MODULUS_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --modulus */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doOptModulus,
     /* desc, NAME, name */ MODULUS_DESC, MODULUS_NAME, MODULUS_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 13, VALUE_OPT_PVT_CERT,
     /* equiv idx, value */ 13, VALUE_OPT_PVT_CERT,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ PVT_CERT_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --pvt-cert */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ PVT_CERT_DESC, PVT_CERT_NAME, PVT_CERT_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 14, VALUE_OPT_PVT_PASSWD,
     /* equiv idx, value */ 14, VALUE_OPT_PVT_PASSWD,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ PVT_PASSWD_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --pvt-passwd */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ PVT_PASSWD_DESC, PVT_PASSWD_NAME, PVT_PASSWD_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 15, VALUE_OPT_GET_PVT_PASSWD,
     /* equiv idx, value */ 15, VALUE_OPT_GET_PVT_PASSWD,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ GET_PVT_PASSWD_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --get-pvt-passwd */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ GET_PVT_PASSWD_DESC, GET_PVT_PASSWD_NAME, GET_PVT_PASSWD_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 16, VALUE_OPT_SIGN_KEY,
     /* equiv idx, value */ 16, VALUE_OPT_SIGN_KEY,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ SIGN_KEY_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --sign-key */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ SIGN_KEY_DESC, SIGN_KEY_NAME, SIGN_KEY_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 17, VALUE_OPT_SUBJECT_NAME,
     /* equiv idx, value */ 17, VALUE_OPT_SUBJECT_NAME,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ SUBJECT_NAME_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --subject-name */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ SUBJECT_NAME_DESC, SUBJECT_NAME_NAME, SUBJECT_NAME_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 18, VALUE_OPT_TRUSTED_CERT,
     /* equiv idx, value */ 18, VALUE_OPT_TRUSTED_CERT,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ TRUSTED_CERT_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --trusted-cert */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ TRUSTED_CERT_DESC, TRUSTED_CERT_NAME, TRUSTED_CERT_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 19, VALUE_OPT_MV_PARAMS,
     /* equiv idx, value */ 19, VALUE_OPT_MV_PARAMS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ MV_PARAMS_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --mv-params */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ optionNumericVal,
     /* desc, NAME, name */ MV_PARAMS_DESC, MV_PARAMS_NAME, MV_PARAMS_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 20, VALUE_OPT_MV_KEYS,
     /* equiv idx, value */ 20, VALUE_OPT_MV_KEYS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ MV_KEYS_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --mv-keys */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ optionNumericVal,
     /* desc, NAME, name */ MV_KEYS_DESC, MV_KEYS_NAME, MV_KEYS_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ INDEX_OPT_VERSION, VALUE_OPT_VERSION,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_OPT_VERSION,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ VER_FLAGS, 0,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ VER_PROC,
     /* desc, NAME, name */ VER_DESC, NULL, VER_name,
     /* disablement strs */ NULL, NULL },



  {  /* entry idx, value */ INDEX_OPT_HELP, VALUE_OPT_HELP,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_OPT_HELP,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ OPTST_IMM | OPTST_NO_INIT, 0,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doUsageOpt,
     /* desc, NAME, name */ HELP_DESC, NULL, HELP_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ INDEX_OPT_MORE_HELP, VALUE_OPT_MORE_HELP,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_OPT_MORE_HELP,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ MORE_HELP_FLAGS, 0,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL,  NULL,
     /* option proc      */ optionPagedUsage,
     /* desc, NAME, name */ MORE_HELP_DESC, NULL, MORE_HELP_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ INDEX_OPT_SAVE_OPTS, VALUE_OPT_SAVE_OPTS,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_OPT_SAVE_OPTS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ OPTST_SET_ARGTYPE(OPARG_TYPE_STRING)
                          | OPTST_ARG_OPTIONAL | OPTST_NO_INIT, 0,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL,  NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ SAVE_OPTS_DESC, NULL, SAVE_OPTS_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ INDEX_OPT_LOAD_OPTS, VALUE_OPT_LOAD_OPTS,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_OPT_LOAD_OPTS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, NOLIMIT, 0,
     /* opt state flags  */ OPTST_SET_ARGTYPE(OPARG_TYPE_STRING)
			  | OPTST_DISABLE_IMM, 0,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ optionLoadOpt,
     /* desc, NAME, name */ LOAD_OPTS_DESC, LOAD_OPTS_NAME, LOAD_OPTS_name,
     /* disablement strs */ NO_LOAD_OPTS_name, LOAD_OPTS_pfx }
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Define the ntp-keygen Option Environment
 */
#define zPROGNAME       (ntp_keygen_opt_strs+2152)
#define zUsageTitle     (ntp_keygen_opt_strs+2163)
#define zRcName         (ntp_keygen_opt_strs+2287)
static char const * const apzHomeList[3] = {
    ntp_keygen_opt_strs+2279,
    ntp_keygen_opt_strs+2285,
    NULL };
#define zBugsAddr       (ntp_keygen_opt_strs+2294)
#define zExplain        (ntp_keygen_opt_strs+2328)
#define zDetail         (NULL)
#define zFullVersion    (ntp_keygen_opt_strs+2331)
/* extracted from optcode.tlib near line 353 */

#if defined(ENABLE_NLS)
# define OPTPROC_BASE OPTPROC_TRANSLATE
  static tOptionXlateProc translate_option_strings;
#else
# define OPTPROC_BASE OPTPROC_NONE
# define translate_option_strings NULL
#endif /* ENABLE_NLS */


#define ntp_keygen_full_usage (NULL)

#define ntp_keygen_short_usage (NULL)

#endif /* not defined __doxygen__ */

/*
 *  Create the static procedure(s) declared above.
 */
/**
 * The callout function that invokes the optionUsage function.
 *
 * @param pOptions the AutoOpts option description structure
 * @param pOptDesc the descriptor for the "help" (usage) option.
 * @noreturn
 */
static void
doUsageOpt(tOptions * pOptions, tOptDesc * pOptDesc)
{
    optionUsage(&ntp_keygenOptions, NTP_KEYGEN_EXIT_SUCCESS);
    /* NOTREACHED */
    (void)pOptDesc;
    (void)pOptions;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Code to handle the imbits option, when AUTOKEY is #define-d.
 *
 * @param pOptions the ntp-keygen options data structure
 * @param pOptDesc the option descriptor for this option.
 */
#ifdef AUTOKEY
static void
doOptImbits(tOptions* pOptions, tOptDesc* pOptDesc)
{
    static struct {long rmin, rmax;} const rng[1] = {
        { 256, 2048 } };
    int  ix;

    if (pOptions <= OPTPROC_EMIT_LIMIT)
        goto emit_ranges;
    optionNumericVal(pOptions, pOptDesc);

    for (ix = 0; ix < 1; ix++) {
        if (pOptDesc->optArg.argInt < rng[ix].rmin)
            continue;  /* ranges need not be ordered. */
        if (pOptDesc->optArg.argInt == rng[ix].rmin)
            return;
        if (rng[ix].rmax == LONG_MIN)
            continue;
        if (pOptDesc->optArg.argInt <= rng[ix].rmax)
            return;
    }

    option_usage_fp = stderr;

 emit_ranges:
    optionShowRange(pOptions, pOptDesc, (void *)rng, 1);
}
#endif /* defined AUTOKEY */

#if ! defined(TEST_NTP_KEYGEN_OPTS)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Code to handle the debug-level option.
 *
 * @param pOptions the ntp-keygen options data structure
 * @param pOptDesc the option descriptor for this option.
 */
static void
doOptDebug_Level(tOptions* pOptions, tOptDesc* pOptDesc)
{
    /*
     * Be sure the flag-code[0] handles special values for the options pointer
     * viz. (poptions <= OPTPROC_EMIT_LIMIT) *and also* the special flag bit
     * ((poptdesc->fOptState & OPTST_RESET) != 0) telling the option to
     * reset its state.
     */
    /* extracted from debug-opt.def, line 15 */
OPT_VALUE_SET_DEBUG_LEVEL++;
    (void)pOptDesc;
    (void)pOptions;
}
#endif /* defined(TEST_NTP_KEYGEN_OPTS) */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Code to handle the modulus option, when AUTOKEY is #define-d.
 *
 * @param pOptions the ntp-keygen options data structure
 * @param pOptDesc the option descriptor for this option.
 */
#ifdef AUTOKEY
static void
doOptModulus(tOptions* pOptions, tOptDesc* pOptDesc)
{
    static struct {long rmin, rmax;} const rng[1] = {
        { 256, 2048 } };
    int  ix;

    if (pOptions <= OPTPROC_EMIT_LIMIT)
        goto emit_ranges;
    optionNumericVal(pOptions, pOptDesc);

    for (ix = 0; ix < 1; ix++) {
        if (pOptDesc->optArg.argInt < rng[ix].rmin)
            continue;  /* ranges need not be ordered. */
        if (pOptDesc->optArg.argInt == rng[ix].rmin)
            return;
        if (rng[ix].rmax == LONG_MIN)
            continue;
        if (pOptDesc->optArg.argInt <= rng[ix].rmax)
            return;
    }

    option_usage_fp = stderr;

 emit_ranges:
    optionShowRange(pOptions, pOptDesc, (void *)rng, 1);
}
#endif /* defined AUTOKEY */
/* extracted from optmain.tlib near line 46 */

#if defined(TEST_NTP_KEYGEN_OPTS) /* TEST MAIN PROCEDURE: */

extern void optionPutShell(tOptions*);

/**
 * Generated main procedure.  This will emit text that a Bourne shell can
 * process to handle its command line arguments.
 *
 * @param argc argument count
 * @param argv argument vector
 * @returns program exit code
 */
int
main(int argc, char ** argv)
{
    int res = NTP_KEYGEN_EXIT_SUCCESS;
    (void)optionProcess(&ntp_keygenOptions, argc, argv);
    optionPutShell(&ntp_keygenOptions);
    res = ferror(stdout);
    if (res != 0)
        fputs("output error writing to stdout\n", stderr);
    return res;
}
#endif  /* defined TEST_NTP_KEYGEN_OPTS */
/* extracted from optmain.tlib near line 1188 */

/**
 * The directory containing the data associated with ntp-keygen.
 */
#ifndef  PKGDATADIR
# define PKGDATADIR ""
#endif

/**
 * Information about the person or institution that packaged ntp-keygen
 * for the current distribution.
 */
#ifndef  WITH_PACKAGER
# define ntp_keygen_packager_info NULL
#else
static char const ntp_keygen_packager_info[] =
    "Packaged by " WITH_PACKAGER

# ifdef WITH_PACKAGER_VERSION
        " ("WITH_PACKAGER_VERSION")"
# endif

# ifdef WITH_PACKAGER_BUG_REPORTS
    "\nReport ntp_keygen bugs to " WITH_PACKAGER_BUG_REPORTS
# endif
    "\n";
#endif
#ifndef __doxygen__

#endif /* __doxygen__ */
/**
 * The option definitions for ntp-keygen.  The one structure that
 * binds them all.
 */
tOptions ntp_keygenOptions = {
    OPTIONS_STRUCT_VERSION,
    0, NULL,                    /* original argc + argv    */
    ( OPTPROC_BASE
    + OPTPROC_ERRSTOP
    + OPTPROC_SHORTOPT
    + OPTPROC_LONGOPT
    + OPTPROC_NO_REQ_OPT
    + OPTPROC_ENVIRON
    + OPTPROC_NO_ARGS
    + OPTPROC_MISUSE ),
    0, NULL,                    /* current option index, current option */
    NULL,         NULL,         zPROGNAME,
    zRcName,      zCopyright,   zLicenseDescrip,
    zFullVersion, apzHomeList,  zUsageTitle,
    zExplain,     zDetail,      optDesc,
    zBugsAddr,                  /* address to send bugs to */
    NULL, NULL,                 /* extensions/saved state  */
    optionUsage, /* usage procedure */
    translate_option_strings,   /* translation procedure */
    /*
     *  Indexes to special options
     */
    { INDEX_OPT_MORE_HELP, /* more-help option index */
      INDEX_OPT_SAVE_OPTS, /* save option index */
      NO_EQUIVALENT, /* '-#' option index */
      NO_EQUIVALENT /* index of default opt */
    },
    26 /* full option count */, 21 /* user option count */,
    ntp_keygen_full_usage, ntp_keygen_short_usage,
    NULL, NULL,
    PKGDATADIR, ntp_keygen_packager_info
};

#if ENABLE_NLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_DCGETTEXT
# include <gettext.h>
#endif
#include <autoopts/usage-txt.h>

static char * AO_gettext(char const * pz);
static void   coerce_it(void ** s);

/**
 * AutoGen specific wrapper function for gettext.  It relies on the macro _()
 * to convert from English to the target language, then strdup-duplicates the
 * result string.  It tries the "libopts" domain first, then whatever has been
 * set via the \a textdomain(3) call.
 *
 * @param[in] pz the input text used as a lookup key.
 * @returns the translated text (if there is one),
 *   or the original text (if not).
 */
static char *
AO_gettext(char const * pz)
{
    char * res;
    if (pz == NULL)
        return NULL;
#ifdef HAVE_DCGETTEXT
    /*
     * While processing the option_xlateable_txt data, try to use the
     * "libopts" domain.  Once we switch to the option descriptor data,
     * do *not* use that domain.
     */
    if (option_xlateable_txt.field_ct != 0) {
        res = dgettext("libopts", pz);
        if (res == pz)
            res = (char *)(void *)_(pz);
    } else
        res = (char *)(void *)_(pz);
#else
    res = (char *)(void *)_(pz);
#endif
    if (res == pz)
        return res;
    res = strdup(res);
    if (res == NULL) {
        fputs(_("No memory for duping translated strings\n"), stderr);
        exit(NTP_KEYGEN_EXIT_FAILURE);
    }
    return res;
}

/**
 * All the pointers we use are marked "* const", but they are stored in
 * writable memory.  Coerce the mutability and set the pointer.
 */
static void coerce_it(void ** s) { *s = AO_gettext(*s);
}

/**
 * Translate all the translatable strings in the ntp_keygenOptions
 * structure defined above.  This is done only once.
 */
static void
translate_option_strings(void)
{
    tOptions * const opts = &ntp_keygenOptions;

    /*
     *  Guard against re-translation.  It won't work.  The strings will have
     *  been changed by the first pass through this code.  One shot only.
     */
    if (option_xlateable_txt.field_ct != 0) {
        /*
         *  Do the translations.  The first pointer follows the field count
         *  field.  The field count field is the size of a pointer.
         */
        tOptDesc * od = opts->pOptDesc;
        char **    ppz = (char**)(void*)&(option_xlateable_txt);
        int        ix  = option_xlateable_txt.field_ct;

        do {
            ppz++; /* skip over field_ct */
            *ppz = AO_gettext(*ppz);
        } while (--ix > 0);
        /* prevent re-translation and disable "libopts" domain lookup */
        option_xlateable_txt.field_ct = 0;

        coerce_it((void*)&(opts->pzCopyright));
        coerce_it((void*)&(opts->pzCopyNotice));
        coerce_it((void*)&(opts->pzFullVersion));
        coerce_it((void*)&(opts->pzUsageTitle));
        coerce_it((void*)&(opts->pzExplain));
        coerce_it((void*)&(opts->pzDetail));
        coerce_it((void*)&(opts->pzPackager));

        for (ix = opts->optCt; ix > 0; ix--, od++)
            coerce_it((void*)&(od->pzText));
    }

    if ((opts->fOptSet & OPTPROC_NXLAT_OPT_CFG) == 0) {
        tOptDesc * od = opts->pOptDesc;
        int        ix;

        for (ix = opts->optCt; ix > 0; ix--, od++) {
            coerce_it((void*)&(od->pz_Name));
            coerce_it((void*)&(od->pz_DisableName));
            coerce_it((void*)&(od->pz_DisablePfx));
        }
        ntp_keygenOptions.fOptSet |= OPTPROC_NXLAT_OPT_CFG | OPTPROC_NXLAT_OPT;
    }
}

#endif /* ENABLE_NLS */

#ifdef  __cplusplus
}
#endif
/* ntp-keygen-opts.c ends here */
