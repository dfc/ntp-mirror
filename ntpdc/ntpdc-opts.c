/*  
 *  EDIT THIS FILE WITH CAUTION  (ntpdc-opts.c)
 *  
 *  It has been AutoGen-ed  April 14, 2012 at 04:42:17 AM by AutoGen 5.14
 *  From the definitions    ntpdc-opts.def
 *  and the template file   options
 *
 * Generated from AutoOpts 36:1:11 templates.
 *
 *  AutoOpts is a copyrighted work.  This source file is not encumbered
 *  by AutoOpts licensing, but is provided under the licensing terms chosen
 *  by the ntpdc author or copyright holder.  AutoOpts is
 *  licensed under the terms of the LGPL.  The redistributable library
 *  (``libopts'') is licensed under the terms of either the LGPL or, at the
 *  users discretion, the BSD license.  See the AutoOpts and/or libopts sources
 *  for details.
 *
 * The ntpdc program is copyrighted and licensed
 * under the following terms:
 *
 *  Copyright (C) 1970-2012 The University of Delaware, all rights reserved.
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

#include <sys/types.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTION_CODE_COMPILE 1
#include "ntpdc-opts.h"
#ifdef  __cplusplus
extern "C" {
#endif
extern FILE * option_usage_fp;

/* TRANSLATORS: choose the translation for option names wisely because you
                cannot ever change your mind. */
#define zCopyright      (ntpdc_opt_strs+0)
#define zLicenseDescrip (ntpdc_opt_strs+315)

extern tUsageProc optionUsage;
/*
 *  global included definitions
 */#ifdef __windows
  extern int atoi(const char*);
#else
# include <stdlib.h>
#endif

#ifndef NULL
#  define NULL 0
#endif

/*
 *  ntpdc option static const strings
 */
static char const ntpdc_opt_strs[1860] =
/*     0 */ "ntpdc 4.2.7p272\n"
            "Copyright (C) 1970-2012 The University of Delaware, all rights reserved.\n"
            "This is free software. It is licensed for use, modification and\n"
            "redistribution under the terms of the NTP License, copies of which\n"
            "can be seen at:\n"
            "  <http://ntp.org/license>\n"
            "  <http://opensource.org/licenses/ntp-license.php>\n\0"
/*   315 */ "Permission to use, copy, modify, and distribute this software and its\n"
            "documentation for any purpose with or without fee is hereby granted,\n"
            "provided that the above copyright notice appears in all copies and that\n"
            "both the copyright notice and this permission notice appear in supporting\n"
            "documentation, and that the name The University of Delaware not be used in\n"
            "advertising or publicity pertaining to distribution of the software\n"
            "without specific, written prior permission. The University of Delaware\n"
            "makes no representations about the suitability this software for any\n"
            "purpose. It is provided \"as is\" without express or implied warranty.\n\0"
/*   953 */ "Force IPv4 DNS name resolution\0"
/*   984 */ "IPV4\0"
/*   989 */ "ipv4\0"
/*   994 */ "Force IPv6 DNS name resolution\0"
/*  1025 */ "IPV6\0"
/*  1030 */ "ipv6\0"
/*  1035 */ "run a command and exit\0"
/*  1058 */ "COMMAND\0"
/*  1066 */ "command\0"
/*  1074 */ "Increase debug verbosity level\0"
/*  1105 */ "DEBUG_LEVEL\0"
/*  1117 */ "debug-level\0"
/*  1129 */ "Set the debug verbosity level\0"
/*  1159 */ "SET_DEBUG_LEVEL\0"
/*  1175 */ "set-debug-level\0"
/*  1191 */ "Force ntpq to operate in interactive mode\0"
/*  1233 */ "INTERACTIVE\0"
/*  1245 */ "interactive\0"
/*  1257 */ "Print a list of the peers\0"
/*  1283 */ "LISTPEERS\0"
/*  1293 */ "listpeers\0"
/*  1303 */ "numeric host addresses\0"
/*  1326 */ "NUMERIC\0"
/*  1334 */ "numeric\0"
/*  1342 */ "PEERS\0"
/*  1348 */ "peers\0"
/*  1354 */ "Show a list of the peers\0"
/*  1379 */ "SHOWPEERS\0"
/*  1389 */ "showpeers\0"
/*  1399 */ "Display extended usage information and exit\0"
/*  1443 */ "help\0"
/*  1448 */ "Extended usage information passed thru pager\0"
/*  1493 */ "more-help\0"
/*  1503 */ "Output version information and exit\0"
/*  1539 */ "version\0"
/*  1547 */ "Save the option state to a config file\0"
/*  1586 */ "save-opts\0"
/*  1596 */ "Load options from a config file\0"
/*  1628 */ "LOAD_OPTS\0"
/*  1638 */ "no-load-opts\0"
/*  1651 */ "no\0"
/*  1654 */ "NTPDC\0"
/*  1660 */ "ntpdc - vendor-specific NTPD control program - Ver. 4.2.7p272\n"
            "USAGE:  %s [ -<flag> [<val>] | --<name>[{=| }<val>] ]... [ host ...]\n\0"
/*  1792 */ ".ntprc\0"
/*  1799 */ "$HOME\0"
/*  1805 */ ".\0"
/*  1807 */ "http://bugs.ntp.org, bugs@ntp.org\0"
/*  1841 */ "\n\n\0"
/*  1844 */ "ntpdc 4.2.7p272";

/*
 *  ipv4 option description with
 *  "Must also have options" and "Incompatible options":
 */
#define IPV4_DESC      (ntpdc_opt_strs+953)
#define IPV4_NAME      (ntpdc_opt_strs+984)
#define IPV4_name      (ntpdc_opt_strs+989)
static int const aIpv4CantList[] = {
    INDEX_OPT_IPV6, NO_EQUIVALENT };
#define IPV4_FLAGS     (OPTST_DISABLED)

/*
 *  ipv6 option description with
 *  "Must also have options" and "Incompatible options":
 */
#define IPV6_DESC      (ntpdc_opt_strs+994)
#define IPV6_NAME      (ntpdc_opt_strs+1025)
#define IPV6_name      (ntpdc_opt_strs+1030)
static int const aIpv6CantList[] = {
    INDEX_OPT_IPV4, NO_EQUIVALENT };
#define IPV6_FLAGS     (OPTST_DISABLED)

/*
 *  command option description:
 */
#define COMMAND_DESC      (ntpdc_opt_strs+1035)
#define COMMAND_NAME      (ntpdc_opt_strs+1058)
#define COMMAND_name      (ntpdc_opt_strs+1066)
#define COMMAND_FLAGS     (OPTST_DISABLED | OPTST_STACKED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  debug-level option description:
 */
#define DEBUG_LEVEL_DESC      (ntpdc_opt_strs+1074)
#define DEBUG_LEVEL_NAME      (ntpdc_opt_strs+1105)
#define DEBUG_LEVEL_name      (ntpdc_opt_strs+1117)
#define DEBUG_LEVEL_FLAGS     (OPTST_DISABLED)

/*
 *  set-debug-level option description:
 */
#define SET_DEBUG_LEVEL_DESC      (ntpdc_opt_strs+1129)
#define SET_DEBUG_LEVEL_NAME      (ntpdc_opt_strs+1159)
#define SET_DEBUG_LEVEL_name      (ntpdc_opt_strs+1175)
#define SET_DEBUG_LEVEL_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  interactive option description with
 *  "Must also have options" and "Incompatible options":
 */
#define INTERACTIVE_DESC      (ntpdc_opt_strs+1191)
#define INTERACTIVE_NAME      (ntpdc_opt_strs+1233)
#define INTERACTIVE_name      (ntpdc_opt_strs+1245)
static int const aInteractiveCantList[] = {
    INDEX_OPT_COMMAND,
    INDEX_OPT_LISTPEERS,
    INDEX_OPT_PEERS,
    INDEX_OPT_SHOWPEERS, NO_EQUIVALENT };
#define INTERACTIVE_FLAGS     (OPTST_DISABLED)

/*
 *  listpeers option description with
 *  "Must also have options" and "Incompatible options":
 */
#define LISTPEERS_DESC      (ntpdc_opt_strs+1257)
#define LISTPEERS_NAME      (ntpdc_opt_strs+1283)
#define LISTPEERS_name      (ntpdc_opt_strs+1293)
static int const aListpeersCantList[] = {
    INDEX_OPT_COMMAND, NO_EQUIVALENT };
#define LISTPEERS_FLAGS     (OPTST_DISABLED)

/*
 *  numeric option description:
 */
#define NUMERIC_DESC      (ntpdc_opt_strs+1303)
#define NUMERIC_NAME      (ntpdc_opt_strs+1326)
#define NUMERIC_name      (ntpdc_opt_strs+1334)
#define NUMERIC_FLAGS     (OPTST_DISABLED)

/*
 *  peers option description with
 *  "Must also have options" and "Incompatible options":
 */
#define PEERS_DESC      (ntpdc_opt_strs+1257)
#define PEERS_NAME      (ntpdc_opt_strs+1342)
#define PEERS_name      (ntpdc_opt_strs+1348)
static int const aPeersCantList[] = {
    INDEX_OPT_COMMAND, NO_EQUIVALENT };
#define PEERS_FLAGS     (OPTST_DISABLED)

/*
 *  showpeers option description with
 *  "Must also have options" and "Incompatible options":
 */
#define SHOWPEERS_DESC      (ntpdc_opt_strs+1354)
#define SHOWPEERS_NAME      (ntpdc_opt_strs+1379)
#define SHOWPEERS_name      (ntpdc_opt_strs+1389)
static int const aShowpeersCantList[] = {
    INDEX_OPT_COMMAND, NO_EQUIVALENT };
#define SHOWPEERS_FLAGS     (OPTST_DISABLED)

/*
 *  Help/More_Help/Version option descriptions:
 */
#define HELP_DESC       (ntpdc_opt_strs+1399)
#define HELP_name       (ntpdc_opt_strs+1443)
#ifdef HAVE_WORKING_FORK
#define MORE_HELP_DESC  (ntpdc_opt_strs+1448)
#define MORE_HELP_name  (ntpdc_opt_strs+1493)
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
#define VER_DESC        (ntpdc_opt_strs+1503)
#define VER_name        (ntpdc_opt_strs+1539)
#define SAVE_OPTS_DESC  (ntpdc_opt_strs+1547)
#define SAVE_OPTS_name  (ntpdc_opt_strs+1586)
#define LOAD_OPTS_DESC     (ntpdc_opt_strs+1596)
#define LOAD_OPTS_NAME     (ntpdc_opt_strs+1628)
#define NO_LOAD_OPTS_name  (ntpdc_opt_strs+1638)
#define LOAD_OPTS_pfx      (ntpdc_opt_strs+1651)
#define LOAD_OPTS_name     (NO_LOAD_OPTS_name + 3)
/*
 *  Declare option callback procedures
 */
#if defined(TEST_NTPDC_OPTS)
/*
 *  Under test, omit argument processing, or call optionStackArg,
 *  if multiple copies are allowed.
 */
static tOptProc
    doUsageOpt;

/*
 *  #define map the "normal" callout procs to the test ones...
 */
#define SET_DEBUG_LEVEL_OPT_PROC optionStackArg


#else /* NOT defined TEST_NTPDC_OPTS */
/*
 *  When not under test, there are different procs to use
 */
extern tOptProc
    ntpOptionPrintVersion, optionBooleanVal,      optionNestedVal,
    optionNumericVal,      optionPagedUsage,      optionResetOpt,
    optionStackArg,        optionTimeDate,        optionTimeVal,
    optionUnstackArg,      optionVendorOption,    optionVersionStderr;
static tOptProc
    doOptSet_Debug_Level, doUsageOpt;

/*
 *  #define map the "normal" callout procs
 */
#define SET_DEBUG_LEVEL_OPT_PROC doOptSet_Debug_Level

#define SET_DEBUG_LEVEL_OPT_PROC doOptSet_Debug_Level
#endif /* defined(TEST_NTPDC_OPTS) */
#define VER_PROC        ntpOptionPrintVersion

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Define the Ntpdc Option Descriptions.
 */
static tOptDesc optDesc[OPTION_CT] = {
  {  /* entry idx, value */ 0, VALUE_OPT_IPV4,
     /* equiv idx, value */ 0, VALUE_OPT_IPV4,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ IPV4_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --ipv4 */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, aIpv4CantList,
     /* option proc      */ NULL,
     /* desc, NAME, name */ IPV4_DESC, IPV4_NAME, IPV4_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 1, VALUE_OPT_IPV6,
     /* equiv idx, value */ 1, VALUE_OPT_IPV6,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ IPV6_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --ipv6 */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, aIpv6CantList,
     /* option proc      */ NULL,
     /* desc, NAME, name */ IPV6_DESC, IPV6_NAME, IPV6_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 2, VALUE_OPT_COMMAND,
     /* equiv idx, value */ 2, VALUE_OPT_COMMAND,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, NOLIMIT, 0,
     /* opt state flags  */ COMMAND_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --command */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ optionStackArg,
     /* desc, NAME, name */ COMMAND_DESC, COMMAND_NAME, COMMAND_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 3, VALUE_OPT_DEBUG_LEVEL,
     /* equiv idx, value */ 3, VALUE_OPT_DEBUG_LEVEL,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, NOLIMIT, 0,
     /* opt state flags  */ DEBUG_LEVEL_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --debug-level */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
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
     /* option proc      */ SET_DEBUG_LEVEL_OPT_PROC,
     /* desc, NAME, name */ SET_DEBUG_LEVEL_DESC, SET_DEBUG_LEVEL_NAME, SET_DEBUG_LEVEL_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 5, VALUE_OPT_INTERACTIVE,
     /* equiv idx, value */ 5, VALUE_OPT_INTERACTIVE,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ INTERACTIVE_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --interactive */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, aInteractiveCantList,
     /* option proc      */ NULL,
     /* desc, NAME, name */ INTERACTIVE_DESC, INTERACTIVE_NAME, INTERACTIVE_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 6, VALUE_OPT_LISTPEERS,
     /* equiv idx, value */ 6, VALUE_OPT_LISTPEERS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ LISTPEERS_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --listpeers */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, aListpeersCantList,
     /* option proc      */ NULL,
     /* desc, NAME, name */ LISTPEERS_DESC, LISTPEERS_NAME, LISTPEERS_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 7, VALUE_OPT_NUMERIC,
     /* equiv idx, value */ 7, VALUE_OPT_NUMERIC,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ NUMERIC_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --numeric */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ NUMERIC_DESC, NUMERIC_NAME, NUMERIC_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 8, VALUE_OPT_PEERS,
     /* equiv idx, value */ 8, VALUE_OPT_PEERS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ PEERS_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --peers */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, aPeersCantList,
     /* option proc      */ NULL,
     /* desc, NAME, name */ PEERS_DESC, PEERS_NAME, PEERS_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 9, VALUE_OPT_SHOWPEERS,
     /* equiv idx, value */ 9, VALUE_OPT_SHOWPEERS,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ SHOWPEERS_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --showpeers */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, aShowpeersCantList,
     /* option proc      */ NULL,
     /* desc, NAME, name */ SHOWPEERS_DESC, SHOWPEERS_NAME, SHOWPEERS_name,
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
 *  Define the Ntpdc Option Environment
 */
#define zPROGNAME       (ntpdc_opt_strs+1654)
#define zUsageTitle     (ntpdc_opt_strs+1660)
#define zRcName         (ntpdc_opt_strs+1792)
static char const * const apzHomeList[3] = {
    ntpdc_opt_strs+1799,
    ntpdc_opt_strs+1805,
    NULL };
#define zBugsAddr       (ntpdc_opt_strs+1807)
#define zExplain        (ntpdc_opt_strs+1841)
#define zDetail         (NULL)
#define zFullVersion    (ntpdc_opt_strs+1844)
/* extracted from optcode.tlib near line 315 */

#if defined(ENABLE_NLS)
# define OPTPROC_BASE OPTPROC_TRANSLATE
  static tOptionXlateProc translate_option_strings;
#else
# define OPTPROC_BASE OPTPROC_NONE
# define translate_option_strings NULL
#endif /* ENABLE_NLS */


#define ntpdc_full_usage (NULL)

#define ntpdc_short_usage (NULL)

/*
 *  Create the static procedure(s) declared above.
 */
static void
doUsageOpt(tOptions * pOptions, tOptDesc * pOptDesc)
{
    (void)pOptions;
    USAGE(NTPDC_EXIT_SUCCESS);
}

#if ! defined(TEST_NTPDC_OPTS)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   For the set-debug-level option.
 */
static void
doOptSet_Debug_Level(tOptions* pOptions, tOptDesc* pOptDesc)
{
    /* extracted from debug-opt.def, line 26 */
DESC(DEBUG_LEVEL).optOccCt = atoi( pOptDesc->pzLastArg );
}
#endif /* defined(TEST_NTPDC_OPTS) */
/* extracted from optmain.tlib near line 128 */

#if defined(TEST_NTPDC_OPTS) /* TEST MAIN PROCEDURE: */

extern void optionPutShell(tOptions*);

int
main(int argc, char ** argv)
{
    int res = NTPDC_EXIT_SUCCESS;
    (void)optionProcess(&ntpdcOptions, argc, argv);
    optionPutShell(&ntpdcOptions);
    res = ferror(stdout);
    if (res != 0)
        fputs("output error writing to stdout\n", stderr);
    return res;
}
#endif  /* defined TEST_NTPDC_OPTS */
/* extracted from optmain.tlib near line 1148 */

#ifndef  PKGDATADIR
# define PKGDATADIR ""
#endif

#ifndef  WITH_PACKAGER
# define ntpdc_packager_info NULL
#else
static char const ntpdc_packager_info[] =
    "Packaged by " WITH_PACKAGER

# ifdef WITH_PACKAGER_VERSION
        " ("WITH_PACKAGER_VERSION")"
# endif

# ifdef WITH_PACKAGER_BUG_REPORTS
    "\nReport ntpdc bugs to " WITH_PACKAGER_BUG_REPORTS
# endif
    "\n";
#endif

tOptions ntpdcOptions = {
    OPTIONS_STRUCT_VERSION,
    0, NULL,                    /* original argc + argv    */
    ( OPTPROC_BASE
    + OPTPROC_ERRSTOP
    + OPTPROC_SHORTOPT
    + OPTPROC_LONGOPT
    + OPTPROC_NO_REQ_OPT
    + OPTPROC_ENVIRON
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
    15 /* full option count */, 10 /* user option count */,
    ntpdc_full_usage, ntpdc_short_usage,
    NULL, NULL,
    PKGDATADIR, ntpdc_packager_info
};

#if ENABLE_NLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <autoopts/usage-txt.h>

static char* AO_gettext(char const* pz);
static void  coerce_it(void** s);

static char*
AO_gettext(char const* pz)
{
    char* pzRes;
    if (pz == NULL)
        return NULL;
    pzRes = _(pz);
    if (pzRes == pz)
        return pzRes;
    pzRes = strdup(pzRes);
    if (pzRes == NULL) {
        fputs(_("No memory for duping translated strings\n"), stderr);
        exit(NTPDC_EXIT_FAILURE);
    }
    return pzRes;
}

static void coerce_it(void** s) { *s = AO_gettext(*s);
}

/*
 *  This invokes the translation code (e.g. gettext(3)).
 */
static void
translate_option_strings(void)
{
    tOptions * const pOpt = &ntpdcOptions;

    /*
     *  Guard against re-translation.  It won't work.  The strings will have
     *  been changed by the first pass through this code.  One shot only.
     */
    if (option_usage_text.field_ct != 0) {
        /*
         *  Do the translations.  The first pointer follows the field count
         *  field.  The field count field is the size of a pointer.
         */
        tOptDesc * pOD = pOpt->pOptDesc;
        char **    ppz = (char**)(void*)&(option_usage_text);
        int        ix  = option_usage_text.field_ct;

        do {
            ppz++;
            *ppz = AO_gettext(*ppz);
        } while (--ix > 0);

        coerce_it((void*)&(pOpt->pzCopyright));
        coerce_it((void*)&(pOpt->pzCopyNotice));
        coerce_it((void*)&(pOpt->pzFullVersion));
        coerce_it((void*)&(pOpt->pzUsageTitle));
        coerce_it((void*)&(pOpt->pzExplain));
        coerce_it((void*)&(pOpt->pzDetail));
        coerce_it((void*)&(pOpt->pzPackager));
        option_usage_text.field_ct = 0;

        for (ix = pOpt->optCt; ix > 0; ix--, pOD++)
            coerce_it((void*)&(pOD->pzText));
    }

    if ((pOpt->fOptSet & OPTPROC_NXLAT_OPT_CFG) == 0) {
        tOptDesc * pOD = pOpt->pOptDesc;
        int        ix;

        for (ix = pOpt->optCt; ix > 0; ix--, pOD++) {
            coerce_it((void*)&(pOD->pz_Name));
            coerce_it((void*)&(pOD->pz_DisableName));
            coerce_it((void*)&(pOD->pz_DisablePfx));
        }
        /* prevent re-translation */
        ntpdcOptions.fOptSet |= OPTPROC_NXLAT_OPT_CFG | OPTPROC_NXLAT_OPT;
    }
}

#endif /* ENABLE_NLS */

#ifdef  __cplusplus
}
#endif
/* ntpdc-opts.c ends here */
