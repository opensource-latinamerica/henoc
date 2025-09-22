AC_PREREQ([2.69])
AC_INIT([henoc], [0.1], [https://github.com/opensource-latinamerica/henoc/issues])
AC_CONFIG_SRCDIR([README.md])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign silent-rules tar-ustar no-dist-gzip dist-xz])

# Programs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_MAKE_SET

# Options
AC_ARG_WITH([qmake],
  [AS_HELP_STRING([--with-qmake=FILE], [Path to Qt5 qmake executable])],
  [QMAKE="$withval"],
  [QMAKE=""])

AC_PATH_PROGS([FALLBACK_QMAKE], [qmake-qt5 qmake], [], [$PATH])
AS_IF([test -z "$QMAKE"], [QMAKE="$FALLBACK_QMAKE"]) 
AS_IF([test -z "$QMAKE"], [AC_MSG_ERROR([qmake (Qt5) not found; install Qt5 development tools])])

# Verify qmake is Qt5
AC_MSG_CHECKING([Qt qmake version])
QM_VER=`$QMAKE -v 2>/dev/null | tr -d '\r'`
AS_CASE([$QM_VER],
  [*Qt\ version\ 5*], [AC_MSG_RESULT([$QM_VER])],
  [AC_MSG_ERROR([qmake does not point to Qt5: $QM_VER])])

# Optional ODE install prefix
AC_ARG_WITH([ode-prefix],
  [AS_HELP_STRING([--with-ode-prefix=DIR], [Install prefix for ODE build under src/HenocUniverse/ode (default: in-tree ode_install)])],
  [ODE_PREFIX="$withval"],
  [ODE_PREFIX="`pwd`/src/HenocUniverse/ode_install"]) 

AC_SUBST([QMAKE])
AC_SUBST([ODE_PREFIX])

# Headers (non-fatal)
AC_CHECK_HEADERS([GL/gl.h], [], [AC_MSG_WARN([OpenGL header GL/gl.h not found])])
AC_CHECK_HEADERS([GL/glu.h], [], [AC_MSG_WARN([OpenGL header GL/glu.h not found])])

# Optional asset building
AC_ARG_ENABLE([assets],
  [AS_HELP_STRING([--enable-assets], [Build PNG assets from branding/*.svg during make])],
  [enable_assets="$enableval"],
  [enable_assets=no])

AM_CONDITIONAL([ASSETS], [test "x$enable_assets" = "xyes"])

ASSET_TOOL=""
ASSET_SIZES="16 32 64 128 256"
if test "x$enable_assets" = "xyes"; then
  AC_PATH_PROG([INKSCAPE], [inkscape], [])
  AC_PATH_PROG([RSVG_CONVERT], [rsvg-convert], [])
  AC_PATH_PROG([IM_CONVERT], [convert], [])
  if test -n "$INKSCAPE"; then
    ASSET_TOOL=inkscape
    ASSET_CONVERT='$(INKSCAPE) "$<" --export-type=png --export-filename="$@" -w "$$size" -h "$$size" >/dev/null 2>&1'
  elif test -n "$RSVG_CONVERT"; then
    ASSET_TOOL=rsvg
    ASSET_CONVERT='$(RSVG_CONVERT) -w "$$size" -h "$$size" "$<" -o "$@"'
  elif test -n "$IM_CONVERT"; then
    ASSET_TOOL=im
    ASSET_CONVERT='$(IM_CONVERT) -background none -resize "$$size"x"$$size" "$<" "$@"'
  else
    AC_MSG_ERROR([--enable-assets requires inkscape or rsvg-convert or convert (ImageMagick)])
  fi
fi

AC_SUBST([ASSET_SIZES])
AC_SUBST([INKSCAPE])
AC_SUBST([RSVG_CONVERT])
AC_SUBST([IM_CONVERT])
AC_SUBST([ASSET_TOOL])
AC_SUBST([ASSET_CONVERT])
AM_CONDITIONAL([ASSET_TOOL_INKSCAPE], [test "x$ASSET_TOOL" = "xinkscape"]) 
AM_CONDITIONAL([ASSET_TOOL_RSVG],     [test "x$ASSET_TOOL" = "xrsvg"]) 
AM_CONDITIONAL([ASSET_TOOL_IM],       [test "x$ASSET_TOOL" = "xim"]) 

AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT
