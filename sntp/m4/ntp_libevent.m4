dnl NTP_LIBEVENT_CHECK_NOBUILD([MINVERSION [, DIR]])	     -*- Autoconf -*-
dnl
dnl Look for libevent, which must be at least MINVERSION.
dnl DIR is the location of our "bundled" copy of libevent.
dnl If NOBUILD is provided as the 3rd argument, do all of the above,
dnl but DO NOT invoke DIR/configure if we are going to use our bundled
dnl version.  This may be the case for nested packages.
dnl
dnl provide --enable-local-libevent .
dnl
dnl Examples:
dnl
dnl    NTP_LIBEVENT_CHECK_NOBUILD([2.0.9], [sntp/libevent])
dnl    NTP_LIBEVENT_CHECK
dnl
AC_DEFUN([NTP_LIBEVENT_CHECK_NOBUILD], [

ntp_pkgconfig_min_version='0.15.0'
ntp_libevent_min_version=m4_default([$1], [2.0.9])
ntp_libevent_tearoff=m4_default([$2], [libevent])

AC_SUBST([CPPFLAGS_LIBEVENT])
AC_SUBST([LDADD_LIBEVENT])

AC_PATH_TOOL([PKG_CONFIG], [pkg-config])
# Make sure pkg-config is recent enough
case "$PKG_CONFIG" in
 /*)
    AC_MSG_CHECKING([if pkg-config is at least version $ntp_pkgconfig_min_version])
    if $PKG_CONFIG --atleast-pkgconfig-version $ntp_pkgconfig_min_version; then
	AC_MSG_RESULT([yes])
    else
	AC_MSG_RESULT([no])
	PKG_CONFIG=""
    fi
    ;;
esac

AC_ARG_ENABLE(
    [local-libevent],
    [AC_HELP_STRING(
	[--enable-local-libevent],
	[Force using the supplied libevent tearoff code]
    )],
    [ntp_use_local_libevent=$enableval],
    [ntp_use_local_libevent=${ntp_use_local_libevent-detect}]
)

case "$ntp_use_local_libevent" in
 yes)
    ;;
 no)
    ;;
 *) # If we have (a good enough) pkg-config, see if it can find libevent
    case "$PKG_CONFIG" in
     /*)
	AC_MSG_CHECKING([if libevent 2.0.9 or later is installed])
	if $PKG_CONFIG --atleast-version=$ntp_libevent_min_version libevent
	then
	    ntp_use_local_libevent=no
	    AC_MSG_NOTICE([Using the installed libevent])
	    CPPFLAGS_LIBEVENT=`$PKG_CONFIG --cflags-only-I libevent`
	    LDADD_LIBEVENT=`$PKG_CONFIG --libs-only-L libevent`
	    case "$LIBISC_PTHREADS_NOTHREADS" in
	     pthreads)
		LDADD_LIBEVENT="$LDADD_LIBEVENT -levent_pthreads"
	    esac
	    LDADD_LIBEVENT="$LDADD_LIBEVENT -levent_core"
	    AC_MSG_RESULT([yes])
	else
	    ntp_use_local_libevent=yes
	    AC_MSG_RESULT([no])
	fi
	;;
     *)
	ntp_use_local_libevent=yes
	;;
    esac
    ;;
esac

case "$ntp_use_local_libevent" in
 yes)
    AC_MSG_NOTICE([Using libevent tearoff])
    CPPFLAGS_LIBEVENT="-I\$(top_builddir)/$ntp_libevent_tearoff/include -I\$(top_srcdir)/$ntp_libevent_tearoff/include"
    case "$LIBISC_PTHREADS_NOTHREADS" in
     pthreads)
	LDADD_LIBEVENT="\$(top_builddir)/$ntp_libevent_tearoff/libevent_pthreads.la \$(top_builddir)/$ntp_libevent_tearoff/libevent_core.la"
	;;
     *)
	LDADD_LIBEVENT="\$(top_builddir)/$ntp_libevent_tearoff/libevent_core.la"
    esac
esac

AM_CONDITIONAL([BUILD_LIBEVENT], [test "x$ntp_use_local_libevent" = "xyes"])

]) dnl NTP_LIBEVENT_CHECK_NOBUILD

dnl NTP_LIBEVENT_CHECK([MINVERSION [, DIR]])	     -*- Autoconf -*-
AC_DEFUN([NTP_LIBEVENT_CHECK], [

AC_SUBST([NTP_FORCE_LIBEVENT_DIST])
NTP_LIBEVENT_CHECK_NOBUILD([$1], [$2])

case "$ntp_libevent_tearoff" in
 libevent)
    ;;
 *)
    AC_MSG_ERROR([ntp_libevent.m4 dir must be libevent, not $ntp_libevent_tearoff])
    ;;
esac

case "$ntp_use_local_libevent" in
 yes)
    dnl ac_configure_args is undocumented but widely abused.
    ac_configure_args="--disable-shared $ac_configure_args"
    ac_configure_args="--disable-libevent-regress $ac_configure_args"
    ac_configure_args="--disable-libevent-install $ac_configure_args"
    AC_CONFIG_SUBDIRS([libevent])
    ;;
 *)
    NTP_FORCE_LIBEVENT_DIST=libevent
    ;;
esac

]) dnl NTP_LIBEVENT_CHECK

