/*
 *  EDIT THIS FILE WITH CAUTION  (ntpdc-opts.c)
 *
 *  It has been AutoGen-ed  February 21, 2013 at 10:46:41 PM by AutoGen 5.17.2pre17
 *  From the definitions    ntpdc-opts.def
 *  and the template file   options
 *
 * Generated from AutoOpts 38:0:13 templates.
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
#include "ntpdc-opts.h"
#include <sys/types.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef  __cplusplus
extern "C" {
#endif
extern FILE * option_usage_fp;
#define zCopyright      (ntpdc_opt_strs+0)
#define zLicenseDescrip (ntpdc_opt_strs+315)

/*
 *  global included definitions
 */
#ifdef __windows
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
static char const ntpdc_opt_strs[1861] =
/*     0 */ "ntpdc 4.2.7p357\n"
            "Copyright (C) 1970-2013 The University of Delaware, all rights reserved.\n"
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
            "advertising or publicity pertaining to distribution of the software without\n"
            "specific, written prior permission.  The University of Delaware makes no\n"
            "representations about the suitability this software for any purpose.  It is\n"
            "provided \"as is\" without express or implied warranty.\n\0"
/*   955 */ "Force IPv4 DNS name resolution\0"
/*   986 */ "IPV4\0"
/*   991 */ "ipv4\0"
/*   996 */ "Force IPv6 DNS name resolution\0"
/*  1027 */ "IPV6\0"
/*  1032 */ "ipv6\0"
/*  1037 */ "run a command and exit\0"
/*  1060 */ "COMMAND\0"
/*  1068 */ "command\0"
/*  1076 */ "Increase debug verbosity level\0"
/*  1107 */ "DEBUG_LEVEL\0"
/*  1119 */ "debug-level\0"
/*  1131 */ "Set the debug verbosity level\0"
/*  1161 */ "SET_DEBUG_LEVEL\0"
/*  1177 */ "set-debug-level\0"
/*  1193 */ "Force ntpq to operate in interactive mode\0"
/*  1235 */ "INTERACTIVE\0"
/*  1247 */ "interactive\0"
/*  1259 */ "Print a list of the peers\0"
/*  1285 */ "LISTPEERS\0"
/*  1295 */ "listpeers\0"
/*  1305 */ "numeric host addresses\0"
/*  1328 */ "NUMERIC\0"
/*  1336 */ "numeric\0"
/*  1344 */ "PEERS\0"
/*  1350 */ "peers\0"
/*  1356 */ "Show a list of the peers\0"
/*  1381 */ "SHOWPEERS\0"
/*  1391 */ "showpeers\0"
/*  1401 */ "display extended usage information and exit\0"
/*  1445 */ "help\0"
/*  1450 */ "extended usage information passed thru pager\0"
/*  1495 */ "more-help\0"
/*  1505 */ "output version information and exit\0"
/*  1541 */ "version\0"
/*  1549 */ "save the option state to a config file\0"
/*  1588 */ "save-opts\0"
/*  1598 */ "load options from a config file\0"
/*  1630 */ "LOAD_OPTS\0"
/*  1640 */ "no-load-opts\0"
/*  1653 */ "no\0"
/*  1656 */ "NTPDC\0"
/*  1662 */ "ntpdc - vendor-specific NTPD control program - Ver. 4.2.7p357\n"
            "Usage:  %s [ -<flag> [<val>] | --<name>[{=| }<val>] ]... [ host ...]\n\0"
/*  1794 */ "$HOME\0"
/*  1800 */ ".\0"
/*  1802 */ ".ntprc\0"
/*  1809 */ "http://bugs.ntp.org, bugs@ntp.org\0"
/*  1843 */ "\n\0"
/*  1845 */ "ntpdc 4.2.7p357";

/*
 *  ipv4 option description with
 *  "Must also have options" and "Incompatible options":
 */
#define IPV4_DESC      (ntpdc_opt_strs+955)
#define IPV4_NAME      (ntpdc_opt_strs+986)
#define IPV4_name      (ntpdc_opt_strs+991)
static int const aIpv4CantList[] = {
    INDEX_OPT_IPV6, NO_EQUIVALENT };
#define IPV4_FLAGS     (OPTST_DISABLED)

/*
 *  ipv6 option description with
 *  "Must also have options" and "Incompatible options":
 */
#define IPV6_DESC      (ntpdc_opt_strs+996)
#define IPV6_NAME      (ntpdc_opt_strs+1027)
#define IPV6_name      (ntpdc_opt_strs+1032)
static int const aIpv6CantList[] = {
    INDEX_OPT_IPV4, NO_EQUIVALENT };
#define IPV6_FLAGS     (OPTST_DISABLED)

/*
 *  command option description:
 */
#define COMMAND_DESC      (ntpdc_opt_strs+1037)
#define COMMAND_NAME      (ntpdc_opt_strs+1060)
#define COMMAND_name      (ntpdc_opt_strs+1068)
#define COMMAND_FLAGS     (OPTST_DISABLED | OPTST_STACKED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  debug-level option description:
 */
#define DEBUG_LEVEL_DESC      (ntpdc_opt_strs+1076)
#define DEBUG_LEVEL_NAME      (ntpdc_opt_strs+1107)
#define DEBUG_LEVEL_name      (ntpdc_opt_strs+1119)
#define DEBUG_LEVEL_FLAGS     (OPTST_DISABLED)

/*
 *  set-debug-level option description:
 */
#define SET_DEBUG_LEVEL_DESC      (ntpdc_opt_strs+1131)
#define SET_DEBUG_LEVEL_NAME      (ntpdc_opt_strs+1161)
#define SET_DEBUG_LEVEL_name      (ntpdc_opt_strs+1177)
#define SET_DEBUG_LEVEL_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

/*
 *  interactive option description with
 *  "Must also have options" and "Incompatible options":
 */
#define INTERACTIVE_DESC      (ntpdc_opt_strs+1193)
#define INTERACTIVE_NAME      (ntpdc_opt_strs+1235)
#define INTERACTIVE_name      (ntpdc_opt_strs+1247)
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
#define LISTPEERS_DESC      (ntpdc_opt_strs+1259)
#define LISTPEERS_NAME      (ntpdc_opt_strs+1285)
#define LISTPEERS_name      (ntpdc_opt_strs+1295)
static int const aListpeersCantList[] = {
    INDEX_OPT_COMMAND, NO_EQUIVALENT };
#define LISTPEERS_FLAGS     (OPTST_DISABLED)

/*
 *  numeric option description:
 */
#define NUMERIC_DESC      (ntpdc_opt_strs+1305)
#define NUMERIC_NAME      (ntpdc_opt_strs+1328)
#define NUMERIC_name      (ntpdc_opt_strs+1336)
#define NUMERIC_FLAGS     (OPTST_DISABLED)

/*
 *  peers option description with
 *  "Must also have options" and "Incompatible options":
 */
#define PEERS_DESC      (ntpdc_opt_strs+1259)
#define PEERS_NAME      (ntpdc_opt_strs+1344)
#define PEERS_name      (ntpdc_opt_strs+1350)
static int const aPeersCantList[] = {
    INDEX_OPT_COMMAND, NO_EQUIVALENT };
#define PEERS_FLAGS     (OPTST_DISABLED)

/*
 *  showpeers option description with
 *  "Must also have options" and "Incompatible options":
 */
#define SHOWPEERS_DESC      (ntpdc_opt_strs+1356)
#define SHOWPEERS_NAME      (ntpdc_opt_strs+1381)
#define SHOWPEERS_name      (ntpdc_opt_strs+1391)
static int const aShowpeersCantList[] = {
    INDEX_OPT_COMMAND, NO_EQUIVALENT };
#define SHOWPEERS_FLAGS     (OPTST_DISABLED)

/*
 *  Help/More_Help/Version option descriptions:
 */
#define HELP_DESC       (ntpdc_opt_strs+1401)
#define HELP_name       (ntpdc_opt_strs+1445)
#ifdef HAVE_WORKING_FORK
#define MORE_HELP_DESC  (ntpdc_opt_strs+1450)
#define MORE_HELP_name  (ntpdc_opt_strs+1495)
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
#define VER_DESC        (ntpdc_opt_strs+1505)
#define VER_name        (ntpdc_opt_strs+1541)
#define SAVE_OPTS_DESC  (ntpdc_opt_strs+1549)
#define SAVE_OPTS_name  (ntpdc_opt_strs+1588)
#define LOAD_OPTS_DESC     (ntpdc_opt_strs+1598)
#define LOAD_OPTS_NAME     (ntpdc_opt_strs+1630)
#define NO_LOAD_OPTS_name  (ntpdc_opt_strs+1640)
#define LOAD_OPTS_pfx      (ntpdc_opt_strs+1653)
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
#define DEBUG_LEVEL_OPT_PROC optionStackArg


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
    doOptDebug_Level, doUsageOpt;

/*
 *  #define map the "normal" callout procs
 */
#define DEBUG_LEVEL_OPT_PROC doOptDebug_Level

#endif /* defined(TEST_NTPDC_OPTS) */
#define VER_PROC        ntpOptionPrintVersion

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 *  Define the ntpdc Option Descriptions.
 * This is an array of OPTION_CT entries, one for each
 * option that the ntpdc program responds to.
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
     /* opt state flags  */ VER_FLAGS, AOUSE_VERSION,
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
     /* opt state flags  */ OPTST_IMM | OPTST_NO_INIT, AOUSE_HELP,
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
     /* opt state flags  */ MORE_HELP_FLAGS, AOUSE_MORE_HELP,
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
                          | OPTST_ARG_OPTIONAL | OPTST_NO_INIT, AOUSE_SAVE_OPTS,
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
			  | OPTST_DISABLE_IMM, AOUSE_LOAD_OPTS,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ optionLoadOpt,
     /* desc, NAME, name */ LOAD_OPTS_DESC, LOAD_OPTS_NAME, LOAD_OPTS_name,
     /* disablement strs */ NO_LOAD_OPTS_name, LOAD_OPTS_pfx }
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Define the ntpdc Option Environment
 */
#define zPROGNAME       (ntpdc_opt_strs+1656)
#define zUsageTitle     (ntpdc_opt_strs+1662)
#define zRcName         (ntpdc_opt_strs+1802)
static char const * const apzHomeList[3] = {
    ntpdc_opt_strs+1794,
    ntpdc_opt_strs+1800,
    NULL };
#define zBugsAddr       (ntpdc_opt_strs+1809)
#define zExplain        (ntpdc_opt_strs+1843)
#define zDetail         (NULL)
#define zFullVersion    (ntpdc_opt_strs+1845)
/* extracted from optcode.tlib near line 364 */

#if defined(ENABLE_NLS)
# define OPTPROC_BASE OPTPROC_TRANSLATE
  static tOptionXlateProc translate_option_strings;
#else
# define OPTPROC_BASE OPTPROC_NONE
# define translate_option_strings NULL
#endif /* ENABLE_NLS */

#define ntpdc_full_usage (NULL)
#define ntpdc_short_usage (NULL)

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
    optionUsage(&ntpdcOptions, NTPDC_EXIT_SUCCESS);
    /* NOTREACHED */
    (void)pOptDesc;
    (void)pOptions;
}

#if ! defined(TEST_NTPDC_OPTS)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Code to handle the debug-level option.
 *
 * @param pOptions the ntpdc options data structure
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
#endif /* defined(TEST_NTPDC_OPTS) */
/* extracted from optmain.tlib near line 46 */

#if defined(TEST_NTPDC_OPTS) /* TEST MAIN PROCEDURE: */

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
    int res = NTPDC_EXIT_SUCCESS;
    (void)optionProcess(&ntpdcOptions, argc, argv);
    optionPutShell(&ntpdcOptions);
    res = ferror(stdout);
    if (res != 0)
        fputs("output error writing to stdout\n", stderr);
    return res;
}
#endif  /* defined TEST_NTPDC_OPTS */
/* extracted from optmain.tlib near line 1254 */

/**
 * The directory containing the data associated with ntpdc.
 */
#ifndef  PKGDATADIR
# define PKGDATADIR ""
#endif

/**
 * Information about the person or institution that packaged ntpdc
 * for the current distribution.
 */
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
#ifndef __doxygen__

#endif /* __doxygen__ */
/**
 * The option definitions for ntpdc.  The one structure that
 * binds them all.
 */
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
/*
 * This code is designed to translate translatable option text for the
 * ntpdc program.  These translations happen upon entry
 * to optionProcess().
 */
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
        exit(NTPDC_EXIT_FAILURE);
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
 * Translate all the translatable strings in the ntpdcOptions
 * structure defined above.  This is done only once.
 */
static void
translate_option_strings(void)
{
    tOptions * const opts = &ntpdcOptions;

    /*
     *  Guard against re-translation.  It won't work.  The strings will have
     *  been changed by the first pass through this code.  One shot only.
     */
    if (option_xlateable_txt.field_ct != 0) {
        /*
         *  Do the translations.  The first pointer follows the field count
         *  field.  The field count field is the size of a pointer.
         */
        char ** ppz = (char**)(void*)&(option_xlateable_txt);
        int     ix  = option_xlateable_txt.field_ct;

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
        {
            tOptDesc * od = opts->pOptDesc;
            for (ix = opts->optCt; ix > 0; ix--, od++)
                coerce_it((void*)&(od->pzText));
        }
    }
}
#endif /* ENABLE_NLS */

#ifdef DO_NOT_COMPILE_THIS_CODE_IT_IS_FOR_GETTEXT
static void bogus_function(void) {
  /* TRANSLATORS:

     The following dummy function was crated solely so that xgettext can extract
     the correct strings.  These strings are actually referenced by a field name
     in the ntpdcOptions structure noted in the comments below.  The
     literal text is defined in ntpdc_opt_strs.
   
     NOTE: the strings below are segmented with respect to the source string
     ntpdc_opt_strs.  The strings above are handed off for translation
     at run time a paragraph at a time.  Consequently, they are presented here
     for translation a paragraph at a time.
   
     ALSO: often the description for an option will reference another option
     by name.  These are set off with apostrophe quotes (I hope).  Do not
     translate option names.
   */
  /* referenced via ntpdcOptions.pzCopyright */
  puts(_("ntpdc 4.2.7p357\n\
Copyright (C) 1970-2013 The University of Delaware, all rights reserved.\n\
This is free software. It is licensed for use, modification and\n\
redistribution under the terms of the NTP License, copies of which\n\
can be seen at:\n"));
  puts(_("  <http://ntp.org/license>\n\
  <http://opensource.org/licenses/ntp-license.php>\n"));

  /* referenced via ntpdcOptions.pzCopyNotice */
  puts(_("Permission to use, copy, modify, and distribute this software and its\n\
documentation for any purpose with or without fee is hereby granted,\n\
provided that the above copyright notice appears in all copies and that\n\
both the copyright notice and this permission notice appear in supporting\n\
documentation, and that the name The University of Delaware not be used in\n\
advertising or publicity pertaining to distribution of the software without\n\
specific, written prior permission.  The University of Delaware makes no\n\
representations about the suitability this software for any purpose.  It is\n\
provided \"as is\" without express or implied warranty.\n"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("Force IPv4 DNS name resolution"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("Force IPv6 DNS name resolution"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("run a command and exit"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("Increase debug verbosity level"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("Set the debug verbosity level"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("Force ntpq to operate in interactive mode"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("Print a list of the peers"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("numeric host addresses"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("Print a list of the peers"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("Show a list of the peers"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("display extended usage information and exit"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("extended usage information passed thru pager"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("output version information and exit"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("save the option state to a config file"));

  /* referenced via ntpdcOptions.pOptDesc->pzText */
  puts(_("load options from a config file"));

  /* referenced via ntpdcOptions.pzUsageTitle */
  puts(_("ntpdc - vendor-specific NTPD control program - Ver. 4.2.7p357\n\
Usage:  %s [ -<flag> [<val>] | --<name>[{=| }<val>] ]... [ host ...]\n"));

  /* referenced via ntpdcOptions.pzExplain */
  puts(_("\n"));

  /* referenced via ntpdcOptions.pzFullVersion */
  puts(_("ntpdc 4.2.7p357"));

  /* referenced via ntpdcOptions.pzFullUsage */
  puts(_("<<<NOT-FOUND>>>"));

  /* referenced via ntpdcOptions.pzShortUsage */
  puts(_("<<<NOT-FOUND>>>"));

  /* LIBOPTS-MESSAGES: */
#line 60 "../autoopts.c"
  puts(_("allocation of %d bytes failed\n"));
#line 86 "../autoopts.c"
  puts(_("allocation of %d bytes failed\n"));
#line 53 "../init.c"
  puts(_("AutoOpts function called without option descriptor\n"));
#line 89 "../init.c"
  puts(_("\tThis exceeds the compiled library version:  "));
#line 87 "../init.c"
  puts(_("Automated Options Processing Error!\n"
       "\t%s called AutoOpts function with structure version %d:%d:%d.\n"));
#line 73 "../autoopts.c"
  puts(_("realloc of %d bytes at 0x%p failed\n"));
#line 91 "../init.c"
  puts(_("\tThis is less than the minimum library version:  "));
#line 121 "../version.c"
  puts(_("Automated Options version %s\n"
       "\tCopyright (C) 1999-2013 by Bruce Korb - all rights reserved\n"));
#line 310 "../usage.c"
  puts(_("(AutoOpts bug):  %s.\n"));
#line 90 "../reset.c"
  puts(_("optionResetOpt() called, but reset-option not configured"));
#line 329 "../usage.c"
  puts(_("could not locate the 'help' option"));
#line 351 "../autoopts.c"
  puts(_("optionProcess() was called with invalid data"));
#line 770 "../usage.c"
  puts(_("invalid argument type specified"));
#line 589 "../find.c"
  puts(_("defaulted to option with optional arg"));
#line 76 "../alias.c"
  puts(_("aliasing option is out of range."));
#line 229 "../enum.c"
  puts(_("%s error:  the keyword '%s' is ambiguous for %s\n"));
#line 108 "../find.c"
  puts(_("  The following options match:\n"));
#line 290 "../find.c"
  puts(_("%s: ambiguous option name: %s (matches %d options)\n"));
#line 161 "../check.c"
  puts(_("%s: Command line arguments required\n"));
#line 43 "../alias.c"
  puts(_("%d %s%s options allowed\n"));
#line 81 "../makeshell.c"
  puts(_("%s error %d (%s) calling %s for '%s'\n"));
#line 293 "../makeshell.c"
  puts(_("interprocess pipe"));
#line 168 "../version.c"
  puts(_("error: version option argument '%c' invalid.  Use:\n"
       "\t'v' - version only\n"
       "\t'c' - version and copyright\n"
       "\t'n' - version and full copyright notice\n"));
#line 58 "../check.c"
  puts(_("%s error:  the '%s' and '%s' options conflict\n"));
#line 214 "../find.c"
  puts(_("%s: The '%s' option has been disabled."));
#line 421 "../find.c"
  puts(_("%s: The '%s' option has been disabled."));
#line 38 "../alias.c"
  puts(_("-equivalence"));
#line 460 "../find.c"
  puts(_("%s: illegal option -- %c\n"));
#line 110 "../reset.c"
  puts(_("%s: illegal option -- %c\n"));
#line 268 "../find.c"
  puts(_("%s: illegal option -- %s\n"));
#line 746 "../find.c"
  puts(_("%s: illegal option -- %s\n"));
#line 118 "../reset.c"
  puts(_("%s: illegal option -- %s\n"));
#line 332 "../find.c"
  puts(_("%s: unknown vendor extension option -- %s\n"));
#line 154 "../enum.c"
  puts(_("  or an integer from %d through %d\n"));
#line 164 "../enum.c"
  puts(_("  or an integer from %d through %d\n"));
#line 769 "../usage.c"
  puts(_("%s error:  invalid option descriptor for %s\n"));
#line 1097 "../usage.c"
  puts(_("%s error:  invalid option descriptor for %s\n"));
#line 379 "../find.c"
  puts(_("%s: invalid option name: %s\n"));
#line 518 "../find.c"
  puts(_("%s: The '%s' option requires an argument.\n"));
#line 171 "../autoopts.c"
  puts(_("(AutoOpts bug):  Equivalenced option '%s' was equivalenced to both\n"
       "\t'%s' and '%s'"));
#line 94 "../check.c"
  puts(_("%s error:  The %s option is required\n"));
#line 623 "../find.c"
  puts(_("%s: The '%s' option cannot have an argument.\n"));
#line 151 "../check.c"
  puts(_("%s: Command line arguments are not allowed.\n"));
#line 531 "../save.c"
  puts(_("error %d (%s) creating %s\n"));
#line 229 "../enum.c"
  puts(_("%s error:  '%s' does not match any %s keywords.\n"));
#line 93 "../reset.c"
  puts(_("%s error: The '%s' option requires an argument.\n"));
#line 184 "../save.c"
  puts(_("error %d (%s) stat-ing %s\n"));
#line 238 "../save.c"
  puts(_("error %d (%s) stat-ing %s\n"));
#line 143 "../restore.c"
  puts(_("%s error: no saved option state\n"));
#line 246 "../autoopts.c"
  puts(_("'%s' is not a command line option.\n"));
#line 114 "../time.c"
  puts(_("%s error:  '%s' is not a recognizable date/time.\n"));
#line 132 "../save.c"
  puts(_("'%s' not defined\n"));
#line 53 "../time.c"
  puts(_("%s error:  '%s' is not a recognizable time duration.\n"));
#line 92 "../check.c"
  puts(_("%s error:  The %s option must appear %d times\n"));
#line 157 "../numeric.c"
  puts(_("%s error:  '%s' is not a recognizable number.\n"));
#line 195 "../enum.c"
  puts(_("%s error:  %s exceeds %s keyword count\n"));
#line 366 "../usage.c"
  puts(_("Try '%s %s' for more information.\n"));
#line 45 "../alias.c"
  puts(_("one %s%s option allowed\n"));
#line 195 "../makeshell.c"
  puts(_("standard output"));
#line 930 "../makeshell.c"
  puts(_("standard output"));
#line 304 "../usage.c"
  puts(_("standard output"));
#line 451 "../usage.c"
  puts(_("standard output"));
#line 647 "../usage.c"
  puts(_("standard output"));
#line 175 "../version.c"
  puts(_("standard output"));
#line 304 "../usage.c"
  puts(_("standard error"));
#line 451 "../usage.c"
  puts(_("standard error"));
#line 647 "../usage.c"
  puts(_("standard error"));
#line 175 "../version.c"
  puts(_("standard error"));
#line 195 "../makeshell.c"
  puts(_("write"));
#line 930 "../makeshell.c"
  puts(_("write"));
#line 303 "../usage.c"
  puts(_("write"));
#line 450 "../usage.c"
  puts(_("write"));
#line 646 "../usage.c"
  puts(_("write"));
#line 174 "../version.c"
  puts(_("write"));
#line 60 "../numeric.c"
  puts(_("%s error:  %s option value %ld is out of range.\n"));
#line 44 "../check.c"
  puts(_("%s error:  %s option requires the %s option\n"));
#line 131 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
#line 183 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
#line 237 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
#line 256 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
#line 530 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
  /* END-LIBOPTS-MESSAGES */

  /* USAGE-TEXT: */
#line 895 "../usage.c"
  puts(_("\t\t\t\t- an alternate for '%s'\n"));
#line 1164 "../usage.c"
  puts(_("Version, usage and configuration options:"));
#line 946 "../usage.c"
  puts(_("\t\t\t\t- default option for unnamed options\n"));
#line 859 "../usage.c"
  puts(_("\t\t\t\t- disabled as '--%s'\n"));
#line 1133 "../usage.c"
  puts(_(" --- %-14s %s\n"));
#line 1131 "../usage.c"
  puts(_("This option has been disabled"));
#line 886 "../usage.c"
  puts(_("\t\t\t\t- enabled by default\n"));
#line 40 "../alias.c"
  puts(_("%s error:  only "));
#line 1208 "../usage.c"
  puts(_(" - examining environment variables named %s_*\n"));
#line 168 "../file.c"
  puts(_("\t\t\t\t- file must not pre-exist\n"));
#line 172 "../file.c"
  puts(_("\t\t\t\t- file must pre-exist\n"));
#line 416 "../usage.c"
  puts(_("Options are specified by doubled hyphens and their name or by a single\n"
       "hyphen and the flag character.\n"));
#line 908 "../makeshell.c"
  puts(_("\n"
       "= = = = = = = =\n\n"
       "This incarnation of genshell will produce\n"
       "a shell script to parse the options for %s:\n\n"));
#line 161 "../enum.c"
  puts(_("  or an integer mask with any of the lower %d bits set\n"));
#line 919 "../usage.c"
  puts(_("\t\t\t\t- is a set membership option\n"));
#line 940 "../usage.c"
  puts(_("\t\t\t\t- must appear between %d and %d times\n"));
#line 418 "../usage.c"
  puts(_("Options are specified by single or double hyphens and their name.\n"));
#line 926 "../usage.c"
  puts(_("\t\t\t\t- may appear multiple times\n"));
#line 913 "../usage.c"
  puts(_("\t\t\t\t- may not be preset\n"));
#line 1323 "../usage.c"
  puts(_("   Arg Option-Name    Description\n"));
#line 1259 "../usage.c"
  puts(_("  Flg Arg Option-Name    Description\n"));
#line 1317 "../usage.c"
  puts(_("  Flg Arg Option-Name    Description\n"));
#line 1318 "../usage.c"
  puts(_(" %3s %s"));
#line 1324 "../usage.c"
  puts(_(" %3s %s"));
#line 423 "../usage.c"
  puts(_("The '-#<number>' option may omit the hash char\n"));
#line 419 "../usage.c"
  puts(_("All arguments are named options.\n"));
#line 993 "../usage.c"
  puts(_(" - reading file %s"));
#line 445 "../usage.c"
  puts(_("\n"
       "please send bug reports to:  <%s>\n"));
#line 100 "../version.c"
  puts(_("\n"
       "please send bug reports to:  <%s>\n"));
#line 129 "../version.c"
  puts(_("\n"
       "please send bug reports to:  <%s>\n"));
#line 925 "../usage.c"
  puts(_("\t\t\t\t- may NOT appear - preset only\n"));
#line 965 "../usage.c"
  puts(_("\n"
       "The following option preset mechanisms are supported:\n"));
#line 1206 "../usage.c"
  puts(_("\n"
       "The following option preset mechanisms are supported:\n"));
#line 704 "../usage.c"
  puts(_("prohibits these options:\n"));
#line 699 "../usage.c"
  puts(_("prohibits the option '%s'\n"));
#line 81 "../numeric.c"
  puts(_("%s%ld to %ld"));
#line 79 "../numeric.c"
  puts(_("%sgreater than or equal to %ld"));
#line 75 "../numeric.c"
  puts(_("%s%ld exactly"));
#line 68 "../numeric.c"
  puts(_("%sit must lie in one of the ranges:\n"));
#line 68 "../numeric.c"
  puts(_("%sit must be in the range:\n"));
#line 88 "../numeric.c"
  puts(_(", or\n"));
#line 66 "../numeric.c"
  puts(_("%sis scalable with a suffix: k/K/m/M/g/G/t/T\n"));
#line 77 "../numeric.c"
  puts(_("%sless than or equal to %ld"));
#line 426 "../usage.c"
  puts(_("Operands and options may be intermixed.  They will be reordered.\n"));
#line 674 "../usage.c"
  puts(_("requires the option '%s'\n"));
#line 677 "../usage.c"
  puts(_("requires these options:\n"));
#line 1335 "../usage.c"
  puts(_("   Arg Option-Name   Req?  Description\n"));
#line 1329 "../usage.c"
  puts(_("  Flg Arg Option-Name   Req?  Description\n"));
#line 162 "../enum.c"
  puts(_("or you may use a numeric representation.  Preceding these with a '!'\n"
       "will clear the bits, specifying 'none' will clear all bits, and 'all'\n"
       "will set them all.  Multiple entries may be passed as an option\n"
       "argument list.\n"));
#line 932 "../usage.c"
  puts(_("\t\t\t\t- may appear up to %d times\n"));
#line 72 "../enum.c"
  puts(_("The valid \"%s\" option keywords are:\n"));
#line 1168 "../usage.c"
  puts(_("The next option supports vendor supported extra options:"));
#line 795 "../usage.c"
  puts(_("These additional options are:"));
  /* END-USAGE-TEXT */
}
#endif /* uncompilable code */
#ifdef  __cplusplus
}
#endif
/* ntpdc-opts.c ends here */
