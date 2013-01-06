
/**
 * \file putshell.c
 *
 *  This module will interpret the options set in the tOptions
 *  structure and print them to standard out in a fashion that
 *  will allow them to be interpreted by the Bourne or Korn shells.
 *
 *  This file is part of AutoOpts, a companion to AutoGen.
 *  AutoOpts is free software.
 *  AutoOpts is Copyright (C) 1992-2013 by Bruce Korb - all rights reserved
 *
 *  AutoOpts is available under any one of two licenses.  The license
 *  in use must be one of these two and the choice is under the control
 *  of the user of the license.
 *
 *   The GNU Lesser General Public License, version 3 or later
 *      See the files "COPYING.lgplv3" and "COPYING.gplv3"
 *
 *   The Modified Berkeley Software Distribution License
 *      See the file "COPYING.mbsd"
 *
 *  These files have the following md5sums:
 *
 *  43b91e8ca915626ed3818ffb1b71248b pkg/libopts/COPYING.gplv3
 *  06a1a2e4760c90ea5e1dad8dfaac4d39 pkg/libopts/COPYING.lgplv3
 *  66a5cedaf62c4b2637025f049f9b826f pkg/libopts/COPYING.mbsd
 */

/* = = = START-STATIC-FORWARD = = = */
static void
print_quot_str(char const * pzStr);

static void
print_enumeration(tOptions * pOpts, tOptDesc * pOD);

static void
print_membership(tOptions * pOpts, tOptDesc * pOD);

static void
print_stacked_arg(tOptions * pOpts, tOptDesc * pOD);

static void
print_reordering(tOptions * pOpts);
/* = = = END-STATIC-FORWARD = = = */

/*
 *  Make sure embedded single quotes come out okay.  The initial quote has
 *  been emitted and the closing quote will be upon return.
 */
static void
print_quot_str(char const * pzStr)
{
    /*
     *  Handle empty strings to make the rest of the logic simpler.
     */
    if ((pzStr == NULL) || (*pzStr == NUL)) {
        fputs(EMPTY_ARG, stdout);
        return;
    }

    /*
     *  Emit any single quotes/apostrophes at the start of the string and
     *  bail if that is all we need to do.
     */
    while (*pzStr == '\'') {
        fputs(QUOT_APOS, stdout);
        pzStr++;
    }
    if (*pzStr == NUL)
        return;

    /*
     *  Start the single quote string
     */
    fputc('\'', stdout);
    for (;;) {
        char const * pz = strchr(pzStr, '\'');
        if (pz == NULL)
            break;

        /*
         *  Emit the string up to the single quote (apostrophe) we just found.
         */
        (void)fwrite(pzStr, (size_t)(pz - pzStr), (size_t)1, stdout);
        fputc('\'', stdout);
        pzStr = pz;

        /*
         *  Emit an escaped apostrophe for every one we find.
         *  If that ends the string, do not re-open the single quotes.
         */
        while (*++pzStr == '\'')   fputs("\\'", stdout);
        if (*pzStr == NUL)
            return;

        fputc('\'', stdout);
    }

    /*
     *  If we broke out of the loop, we must still emit the remaining text
     *  and then close the single quote string.
     */
    fputs(pzStr, stdout);
    fputc('\'', stdout);
}

static void
print_enumeration(tOptions * pOpts, tOptDesc * pOD)
{
    uintptr_t e_val = pOD->optArg.argEnum;
    printf(OPT_VAL_FMT, pOpts->pzPROGNAME, pOD->pz_NAME);

    /*
     *  Convert value to string, print that and restore numeric value.
     */
    (*(pOD->pOptProc))(OPTPROC_RETURN_VALNAME, pOD);
    printf(QUOT_ARG_FMT, pOD->optArg.argString);
    if (pOD->fOptState & OPTST_ALLOC_ARG)
        AGFREE(pOD->optArg.argString);
    pOD->optArg.argEnum = e_val;

    printf(OPT_END_FMT, pOpts->pzPROGNAME, pOD->pz_NAME);
}

static void
print_membership(tOptions * pOpts, tOptDesc * pOD)
{
    char const * pz;
    uintptr_t val = 1;
    printf(zOptNumFmt, pOpts->pzPROGNAME, pOD->pz_NAME,
           (int)(uintptr_t)(pOD->optCookie));
    pOD->optCookie = (void*)(uintptr_t)~0UL;
    (*(pOD->pOptProc))(OPTPROC_RETURN_VALNAME, pOD);

    /*
     *  We are building the typeset list.  The list returned starts with
     *  'none + ' for use by option saving stuff.  We must ignore that.
     */
    pz = pOD->optArg.argString + 7;
    while (*pz != NUL) {
        printf("typeset -x -i %s_", pOD->pz_NAME);
        pz = SPN_PLUS_N_SPACE_CHARS(pz);

        for (;;) {
            int ch = *(pz++);
            if (IS_LOWER_CASE_CHAR(ch))   fputc(toupper(ch), stdout);
            else if (IS_UPPER_CASE_CHAR(ch))   fputc(ch, stdout);
            else if (IS_PLUS_N_SPACE_CHAR(ch)) goto name_done;
            else if (ch == NUL)        { pz--; goto name_done; }
            else fputc('_', stdout);
        } name_done:;
        printf(SHOW_VAL_FMT, (unsigned long)val);
        val <<= 1;
    }

    AGFREE(pOD->optArg.argString);
    pOD->optArg.argString = NULL;
    pOD->fOptState &= ~OPTST_ALLOC_ARG;
}

static void
print_stacked_arg(tOptions * pOpts, tOptDesc * pOD)
{
    tArgList*    pAL = (tArgList*)pOD->optCookie;
    tCC**        ppz = pAL->apzArgs;
    int          ct  = pAL->useCt;

    printf(zOptCookieCt, pOpts->pzPROGNAME, pOD->pz_NAME, ct);

    while (--ct >= 0) {
        printf(ARG_BY_NUM_FMT, pOpts->pzPROGNAME, pOD->pz_NAME,
               pAL->useCt - ct);
        print_quot_str(*(ppz++));
        printf(EXPORT_ARG_FMT, pOpts->pzPROGNAME, pOD->pz_NAME,
               pAL->useCt - ct);
    }
}

static void
print_reordering(tOptions * pOpts)
{
    unsigned int optIx;

    fputs(set_dash, stdout);

    for (optIx = pOpts->curOptIdx;
         optIx < pOpts->origArgCt;
         optIx++) {

        char* pzArg = pOpts->origArgVect[ optIx ];

        if (strchr(pzArg, '\'') == NULL)
            printf(arg_fmt, pzArg);

        else {
            fputs(" '", stdout);
            for (;;) {
                char ch = *(pzArg++);
                switch (ch) {
                case '\'':  fputs(apostrophy, stdout); break;
                case NUL:   goto arg_done;
                default:    fputc(ch, stdout); break;
                }
            } arg_done:;
            fputc('\'', stdout);
        }
    }
    fputs(init_optct, stdout);
}

/*=export_func  optionPutShell
 * what:  write a portable shell script to parse options
 * private:
 * arg:   tOptions*, pOpts, the program options descriptor
 * doc:   This routine will emit portable shell script text for parsing
 *        the options described in the option definitions.
=*/
void
optionPutShell(tOptions* pOpts)
{
    int  optIx = 0;

    printf(zOptCtFmt, pOpts->curOptIdx-1);

    do  {
        tOptDesc* pOD = pOpts->pOptDesc + optIx;

        if ((pOD->fOptState & OPTST_NO_OUTPUT_MASK) != 0)
            continue;

        /*
         *  Equivalence classes are hard to deal with.  Where the
         *  option data wind up kind of squishes around.  For the purposes
         *  of emitting shell state, they are not recommended, but we'll
         *  do something.  I guess we'll emit the equivalenced-to option
         *  at the point in time when the base option is found.
         */
        if (pOD->optEquivIndex != NO_EQUIVALENT)
            continue; /* equivalence to a different option */

        /*
         *  Equivalenced to a different option.  Process the current option
         *  as the equivalenced-to option.  Keep the persistent state bits,
         *  but copy over the set-state bits.
         */
        if (pOD->optActualIndex != optIx) {
            tOptDesc* p   = pOpts->pOptDesc + pOD->optActualIndex;
            p->optArg     = pOD->optArg;
            p->fOptState &= OPTST_PERSISTENT_MASK;
            p->fOptState |= pOD->fOptState & ~OPTST_PERSISTENT_MASK;
            printf(zEquivMode, pOpts->pzPROGNAME, pOD->pz_NAME, p->pz_NAME);
            pOD = p;
        }

        /*
         *  If the argument type is a set membership bitmask, then we always
         *  emit the thing.  We do this because it will always have some sort
         *  of bitmask value and we need to emit the bit values.
         */
        if (OPTST_GET_ARGTYPE(pOD->fOptState) == OPARG_TYPE_MEMBERSHIP) {
            print_membership(pOpts, pOD);
            continue;
        }

        /*
         *  IF the option was either specified or it wakes up enabled,
         *  then we will emit information.  Otherwise, skip it.
         *  The idea is that if someone defines an option to initialize
         *  enabled, we should tell our shell script that it is enabled.
         */
        if (UNUSED_OPT(pOD) && DISABLED_OPT(pOD))
            continue;

        /*
         *  Handle stacked arguments
         */
        if (  (pOD->fOptState & OPTST_STACKED)
           && (pOD->optCookie != NULL) )  {
            print_stacked_arg(pOpts, pOD);
            continue;
        }

        /*
         *  If the argument has been disabled,
         *  Then set its value to the disablement string
         */
        if ((pOD->fOptState & OPTST_DISABLED) != 0) {
            printf(zOptDisabl, pOpts->pzPROGNAME, pOD->pz_NAME,
                   (pOD->pz_DisablePfx != NULL)
                   ? pOD->pz_DisablePfx : "false");
            continue;
        }

        /*
         *  If the argument type is numeric, the last arg pointer
         *  is really the VALUE of the string that was pointed to.
         */
        if (OPTST_GET_ARGTYPE(pOD->fOptState) == OPARG_TYPE_NUMERIC) {
            printf(zOptNumFmt, pOpts->pzPROGNAME, pOD->pz_NAME,
                   (int)pOD->optArg.argInt);
            continue;
        }

        /*
         *  If the argument type is an enumeration, then it is much
         *  like a text value, except we call the callback function
         *  to emit the value corresponding to the "optArg" number.
         */
        if (OPTST_GET_ARGTYPE(pOD->fOptState) == OPARG_TYPE_ENUMERATION) {
            print_enumeration(pOpts, pOD);
            continue;
        }

        /*
         *  If the argument type is numeric, the last arg pointer
         *  is really the VALUE of the string that was pointed to.
         */
        if (OPTST_GET_ARGTYPE(pOD->fOptState) == OPARG_TYPE_BOOLEAN) {
            printf(zFullOptFmt, pOpts->pzPROGNAME, pOD->pz_NAME,
                   (pOD->optArg.argBool == 0) ? "false" : "true");
            continue;
        }

        /*
         *  IF the option has an empty value,
         *  THEN we set the argument to the occurrence count.
         */
        if (  (pOD->optArg.argString == NULL)
           || (pOD->optArg.argString[0] == NUL) ) {

            printf(zOptNumFmt, pOpts->pzPROGNAME, pOD->pz_NAME,
                   pOD->optOccCt);
            continue;
        }

        /*
         *  This option has a text value
         */
        printf(OPT_VAL_FMT, pOpts->pzPROGNAME, pOD->pz_NAME);
        print_quot_str(pOD->optArg.argString);
        printf(OPT_END_FMT, pOpts->pzPROGNAME, pOD->pz_NAME);

    } while (++optIx < pOpts->presetOptCt );

    if (  ((pOpts->fOptSet & OPTPROC_REORDER) != 0)
       && (pOpts->curOptIdx < pOpts->origArgCt))
        print_reordering(pOpts);

    fflush(stdout);
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of autoopts/putshell.c */
