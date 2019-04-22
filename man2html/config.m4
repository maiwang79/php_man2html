dnl $Id$
dnl config.m4 for extension man2html

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(man2html, for man2html support,
dnl Make sure that the comment is aligned:
dnl [  --with-man2html             Include man2html support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(man2html, whether to enable man2html support,
 Make sure that the comment is aligned:
[  --enable-man2html           Enable man2html support])

if test "$PHP_MAN2HTML" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-man2html -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/man2html.h"  # you most likely want to change this
  dnl if test -r $PHP_MAN2HTML/$SEARCH_FOR; then # path given as parameter
  dnl   MAN2HTML_DIR=$PHP_MAN2HTML
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for man2html files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MAN2HTML_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MAN2HTML_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the man2html distribution])
  dnl fi

  dnl # --with-man2html -> add include path
  dnl PHP_ADD_INCLUDE($MAN2HTML_DIR/include)

  dnl # --with-man2html -> check for lib and symbol presence
  dnl LIBNAME=man2html # you may want to change this
  dnl LIBSYMBOL=man2html # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MAN2HTML_DIR/$PHP_LIBDIR, MAN2HTML_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MAN2HTMLLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong man2html lib version or lib not found])
  dnl ],[
  dnl   -L$MAN2HTML_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MAN2HTML_SHARED_LIBADD)

  PHP_NEW_EXTENSION(man2html, man2html.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
