#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
PACKAGE=osd-lyrics
PACKAGE_NAME="OSD Lyrics"
MAJOR_VER=0
MINOR_VER=3
BUILD=20100604
AC_SUBST(BUILD)

AC_INIT([osd-lyrics], [0.3.20100604], [http://code.google.com/p/osd-lyrics/issues/list])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/ol_main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_RANLIB
AC_CONFIG_FILES([ po/Makefile.in
	Makefile
	lib/Makefile
	dailydeb.sh
	src/Makefile
	src/tests/Makefile
	data/Makefile
])

#define paths
OL_ICONDIR="$datadir/$PACKAGE/icons"
AC_SUBST(OL_ICONDIR)
OL_GUIDIR="$datadir/$PACKAGE"
AC_SUBST(OL_GUIDIR)

# gprof support
AC_ARG_WITH(gprof-support,
[  --with-gprof-support=DIR   Include gmon.out file renaming support 
  --without-gprof-support    Don't do gmon.out file renaming (default)],
[ case "$withval" in
  yes)
    AC_MSG_RESULT(yes)
    AC_DEFINE([WITH_GPROF_SUPPORT], [], [Support gprof profiling])
    CFLAGS="-pg $CFLAGS"
    LDFLAGS="-pg $LDFLAGS"
    AC_MSG_WARN(--with-gprof-support called without argument - using default)
    gmondir="/tmp"
    AC_SUBST(gmondir)
    ;;
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    AC_DEFINE([WITH_GPROF_SUPPORT], [], [Support gprof profiling])
    CFLAGS="-pg $CFLAGS"
    LDFLAGS="-pg $LDFLAGS"
    gmondir="$withval"
    AC_SUBST(gmondir)
    ;;
  esac 
])

dnl app indicators in Ubuntu
APPINDICATOR_REQUIRED=0.0.7

AC_ARG_ENABLE(appindicator,
                        AS_HELP_STRING([--enable-appindicator[=@<:@no/auto/yes@:>@]],[Build support for application indicators ]),
                        [enable_appindicator=$enableval],
                        [enable_appindicator="auto"])

dnl MPD support
AC_ARG_ENABLE([mpd],
  [AS_HELP_STRING([--disable-mpd], [Disables MPD support. You won't need libmpd if you use this option])],
  [],
  [enable_mpd=yes])

AC_ARG_ENABLE([xmms2],
  [AS_HELP_STRING([--disable-xmms2], [Disables XMMS2 support. You won't need libxmmsclient if you use this option])],
  [],
  [enable_xmms2=yes])

AC_ARG_ENABLE([amarok1],
  [AS_HELP_STRING([--enable-amarok1], [Enables Amarok 1.4 support])],
  [],
  [enable_amarok1=no])

AS_IF([test "x$enable_amarok1" != xno],
  [AC_DEFINE([ENABLE_AMAROK1], [1], [Define if compile to support Amarok 1.4])])

# Checks for libraries.
PKG_CHECK_MODULES([GTK2], [gtk+-2.0 >= 2.12.0, gthread-2.0, gmodule-export-2.0])
PKG_CHECK_MODULES([X11], [x11])   dnl For XGrabKey
PKG_CHECK_MODULES([DBUS_GLIB], [dbus-glib-1])
PKG_CHECK_MODULES([CURL], [libcurl])
PKG_CHECK_MODULES([NOTIFY], [libnotify])
PKG_CHECK_MODULES([SQLITE], [sqlite3 >= 3.3.0])
MPD_CFLAGS=
MPD_LIBS=
AS_IF([test "x$enable_mpd" != "xno"],
  [PKG_CHECK_MODULES([MPD], [libmpd],
    [AC_DEFINE([ENABLE_MPD], [1], [Define if compile to support MPD])
     #BUILD_MPD_SUPPORT=1
    ],
    [enable_mpd=no
     #BUILD_MPD_SUPPORT=0
     AC_MSG_WARN(
       [libmpd is not available, use --disable-mpd to disable MPD support])
    ])
  ],
  [BUILD_MPD_SUPPORT=0
   AC_SUBST(MPD_CFLAGS)
   AC_SUBST(MPD_LIBS)
  ]
)
XMMS2_CFLAGS=
XMMS2_LIBS=
AS_IF([test "x$enable_xmms2" != "xno"],
  [PKG_CHECK_MODULES([XMMS2], [xmms2-client],
    [AC_DEFINE([ENABLE_XMMS2], [1], [Define if compile to support XMMS2])
    ],
    [enable_xmms2=no
     AC_MSG_WARN(
       [xmms2-client is not available, use --disable-xmms2 to disable XMMS2 support])
    ])
  ],
  [BUILD_XMMS2_SUPPORT=0
   AC_SUBST(XMMS2_CFLAGS)
   AC_SUBST(XMMS2_LIBS)
  ]
)
#AM_CONDITIONAL(BUILD_MPD_SUPPORT, [test "x$enable_mpd" != "xno"])
#AC_SUBST(BUILD_MPD_SUPPORT)
if test x$enable_appindicator = xauto ; then
        PKG_CHECK_EXISTS([appindicator-0.1 >= $APPINDICATOR_REQUIRED],
                         enable_appindicator="yes",
                         enable_appindicator="no")
fi

if test x$enable_appindicator = xyes ; then
        PKG_CHECK_EXISTS([appindicator-0.1 >= $APPINDICATOR_REQUIRED],,
                         AC_MSG_ERROR([appindicator-0.1 is not installed]))
        PKG_CHECK_MODULES(APP_INDICATOR,
                        appindicator-0.1 >= $APPINDICATOR_REQUIRED)
        AC_SUBST(APP_INDICATOR_CFLAGS)
        AC_SUBST(APP_INDICATOR_LIBS)
        AC_DEFINE(HAVE_APP_INDICATOR, 1, [Have AppIndicator])
fi
AM_CONDITIONAL(HAVE_APP_INDICATOR, test x"$enable_appindicator" = xyes)

# i18n support from GNU gettext.
GETTEXT_PACKAGE=osd-lyrics
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package])
IT_PROG_INTLTOOL([0.35.0])
AM_GLIB_GNU_GETTEXT

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
