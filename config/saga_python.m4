
#
# SYNOPSIS
#
#   AX_SAGA_CHECK_PYTHON([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the PYTHON libraries of a particular version (or newer)
#
#   If no path to the installed python library is given,
#   the macro searchs under /usr, /usr/local, /opt and
#   /usr/local/package/python-*
#
#   This macro calls:
#
#     AC_SUBST(HAVE_PYTHON)
#     AC_SUBST(PYTHON_LOCATION)
#     AC_SUBST(PYTHON_CPPFLAGS) 
#     AC_SUBST(PYTHON_LDFLAGS)
#
# LAST MODIFICATION
#
#   2007-03-04
#
# COPYLEFT
#
#   Copyright (c) 2007 Andre Merzky      <andre@merzky.net>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

#
# python.m4 is broken IMHO, at least for us: it does not deliver the python install
# prefix, and does not check for devel version
#
# m4_include([python.m4])
#


AC_DEFUN([AX_SAGA_CHECK_PYTHON],
[
  AC_ARG_VAR([PYTHON_LOCATION],[python installation directory])

  HAVE_PYTHON=no

  tmp_location=""
  AC_ARG_WITH([python],
              AS_HELP_STRING([--with-python=DIR],
              [use python (default is YES) at DIR (optional)]),
              [
              if test "$withval" = "no"; then
                want_python="no"
              elif test "$withval" = "yes"; then
                want_python="yes"
                tmp_location=""
              else
                want_python="yes"
                tmp_location="$withval"
              fi
              ],
              [want_python="yes"])

  # use PYTHON_LOCATION if avaialble, and if not 
  # overwritten by --with-python=<dir>

  if test "x$want_python" = "xyes"; then
    
    packages=`ls /usr/local/package/python-* 2>>/dev/null`
    
    for tmp_path in $tmp_location $PYTHON_LOCATION /usr /usr/local /opt $packages; do
      
      PYTHON=$tmp_path/bin/python

      AC_MSG_CHECKING(for Python in $PYTHON)

      if test -e $PYTHON; then

        AC_MSG_RESULT(yes)
        AC_MSG_CHECKING(for Python version >= $1)

        PYTHON_VERSION_NEED=$1
        PYTHON_VERSION_NEED_MAJ=`echo $1 | cut -f 1 -d '.'`
        PYTHON_VERSION_NEED_MIN=`echo $1 | cut -f 2 -d '.'`
        PYTHON_VERSION_NEED_SUB=`echo $1 | cut -f 3 -d '.'`

        PYTHON_VERSION=`$PYTHON -c "import sys; print(sys.version[[0:5]])"`
        PYTHON_VERSION_MAJ=`$PYTHON -c "import sys; print(sys.version[[0:1]])"`
        PYTHON_VERSION_MIN=`$PYTHON -c "import sys; print(sys.version[[2:3]])"`
        PYTHON_VERSION_SUB=`$PYTHON -c "import sys; print(sys.version[[4:5]])"`

        # catch versions w/o minor or subminor version number
        if test "$PYTHON_VERSION_MIN" = "("; then
          PYTHON_VERSION=`$PYTHON -c "import sys; print(sys.version[[0:1]])"`
        fi
        if test "$PYTHON_VERSION_SUB" = "("; then
          PYTHON_VERSION=`$PYTHON -c "import sys; print(sys.version[[0:3]])"`
        fi

        # AC_MSG_NOTICE([PYTHON_VERSION_NEED    : $PYTHON_VERSION_NEED    ])
        # AC_MSG_NOTICE([PYTHON_VERSION_NEED_MAJ: $PYTHON_VERSION_NEED_MAJ])
        # AC_MSG_NOTICE([PYTHON_VERSION_NEED_MIN: $PYTHON_VERSION_NEED_MIN])
        # AC_MSG_NOTICE([PYTHON_VERSION_NEED_SUB: $PYTHON_VERSION_NEED_SUB])
        # 
        # AC_MSG_NOTICE([PYTHON_VERSION         : $PYTHON_VERSION         ])
        # AC_MSG_NOTICE([PYTHON_VERSION_MAJ     : $PYTHON_VERSION_MAJ     ])
        # AC_MSG_NOTICE([PYTHON_VERSION_MIN     : $PYTHON_VERSION_MIN     ])
        # AC_MSG_NOTICE([PYTHON_VERSION_SUB     : $PYTHON_VERSION_SUB     ])

        if test $PYTHON_VERSION_MAJ -lt $PYTHON_VERSION_NEED_MAJ -a \
                $PYTHON_VERSION_MIN -lt $PYTHON_VERSION_NEED_MIN; then

          AC_MSG_RESULT([too old ($PYTHON_VERSION)])
          SAGA_HAVE_PYTHON=no

        else

          AC_MSG_RESULT([ok ($PYTHON_VERSION)])


          # the following lines are taken from postgreasql's python.m4
          python_configdir=`${PYTHON} -c "from distutils.sysconfig import get_python_lib as f; import os; print(os.path.join(f(plat_specific=1,standard_lib=1),'config'))"`
          python_includespec=`${PYTHON} -c "import distutils.sysconfig; print('-I'+distutils.sysconfig.get_python_inc())"`
          
          python_libdir=`${PYTHON} -c "import distutils.sysconfig,string; print(' '.join(filter(None,distutils.sysconfig.get_config_vars('LIBDIR'))))"`
          python_ldlibrary=`${PYTHON} -c "import distutils.sysconfig,string; print(' '.join(filter(None,distutils.sysconfig.get_config_vars('LDLIBRARY'))))"`
          python_so=`${PYTHON} -c "import distutils.sysconfig,string; print(' '.join(filter(None,distutils.sysconfig.get_config_vars('SO'))))"`
          ldlibrary=`echo "${python_ldlibrary}" | sed "s/${python_so}$//"`
          
          if test x"${python_libdir}" != x"" -a x"${python_ldlibrary}" != x"" -a x"${python_ldlibrary}" != x"${ldlibrary}"
          then
          	# New way: use the official shared library
          	ldlibrary=`echo "${ldlibrary}" | sed "s/^lib//"`
          	python_libspec="-L${python_libdir} -l${ldlibrary}"
          else
          	# Old way: use libpython from python_configdir
          	python_libdir="${python_configdir}"
          	python_libspec="-L${python_libdir} -lpython${python_version}"
          fi
          
          python_additional_libs=`${PYTHON} -c "import distutils.sysconfig,string; print(' '.join(filter(None,distutils.sysconfig.get_config_vars('LIBS','LIBC','LIBM','LOCALMODLIBS','BASEMODLIBS'))))"`

        
          PYTHON_CPPFLAGS=$python_includespec
          PYTHON_LDFLAGS=$python_libspec

          ldflags_save=$LDFLAGS
          cppflags_save=$CPPFLAGS

          CPPFLAGS="$CPPFLAGS $PYTHON_CPPFLAGS"
          LDFLAGS="$LDFLAGS $PYTHON_LDFLAGS"

          AC_MSG_CHECKING([trying to link against python])

          AC_LINK_IFELSE([AC_LANG_PROGRAM([[@%:@include <Python.h>]],
                                          [[
                                            PyHeapTypeObject test;
                                            return (0);
                                          ]])],
                                          link_python="yes",
                                          link_python="no")
              
          if test "x$link_python" = "xno"; then
        
            AC_MSG_RESULT(no (could not compile/link test program))

          else

            AC_MSG_RESULT([ok])

            PYTHON_LOCATION=$tmp_path
            HAVE_PYTHON=yes

            PYTHON_PACKAGE_PATH="lib/python$PYTHON_VERSION/site-packages"

            if test "$tmp_location" != "" && \
               test "$tmp_location" != "$tmp_path" ; then
              AC_MSG_WARN([PYTHON found:
                           not in $tmp_location
                           but in $tmp_path])
            fi

            break;

          fi # link test
          
          LDFLAGS=$ldflags_save
          CPPFLAGS=$cppflags_save
        
        fi # version ok

      else # got python binary

        AC_MSG_RESULT([no]) 

      fi # got python binary

    done # foreach path

  fi # want_python


  if  test "$HAVE_PYTHON" != "yes"; then

    AC_MSG_WARN([no python (devel) found])

  else

    AC_ARG_WITH([python-module-base],
                AS_HELP_STRING([--with-python-module-base=name],
                [name base for saga modules (default: saga)]),
                [
                if test "$withval" = "no"; then
                  python_module_base="saga"
                elif test "$withval" = "yes"; then
                  python_module_base="saga"
                else
                  python_module_base="$withval"
                fi
                ],
                [python_module_base="saga"])

    PYTHON_SAGA_MODULE_BASE=$python_module_base

  fi


  AC_SUBST(HAVE_PYTHON)

  AC_SUBST(PYTHON)
  AC_SUBST(PYTHON_VERSION)
  AC_SUBST(PYTHON_LOCATION)

  AC_SUBST(PYTHON_CPPFLAGS)
  AC_SUBST(PYTHON_LDFLAGS)
  AC_SUBST(PYTHON_SAGA_MODULE_BASE)

  AC_SUBST(PYTHON_PACKAGE_PATH)

])




# # reimplementation which relies on a fixed python.m4
# m4_include([config/python.m4])
# AC_DEFUN([AX_SAGA_CHECK_PYTHON],
# [
# 
#   AC_ARG_VAR([PYTHON_LOCATION],[python installation directory])
# 
#   HAVE_PYTHON=no
# 
#   tmp_location=""
#   AC_ARG_WITH([python],
#               AS_HELP_STRING([--with-python=DIR],
#               [use python (default is YES) at DIR (optional)]),
#               [
#               if test "$withval" = "no"; then
#                 want_python="no"
#               elif test "$withval" = "yes"; then
#                 want_python="yes"
#                 tmp_location=""
#               else
#                 want_python="yes"
#                 tmp_location="$withval"
#               fi
#               ],
#               [want_python="yes"])
# 
#   # use PYTHON_LOCATION if avaialble, and if not 
#   # overwritten by --with-python=<dir>
# 
#   if test "x$want_python" = "xyes"; then
# 
#     AM_PATH_PYTHON(2.2)
#     AC_MSG_NOTICE([============================================])
#     AC_MSG_NOTICE([PYTHON:             $PYTHON])
#     AC_MSG_NOTICE([PYTHON_VERSION:     $PYTHON_VERSION])
#     AC_MSG_NOTICE([PYTHON_PREFIX:      $PYTHON_PREFIX])
#     AC_MSG_NOTICE([PYTHON_EXEC_PREFIX: $PYTHO_EXEC_PREFIXN])
#     AC_MSG_NOTICE([PYTHON_PLATFORM:    $PYTHON_PLATFORM])
#     AC_MSG_NOTICE([pythondir:          $pythondir])
#     AC_MSG_NOTICE([pkgpythondir:       $pkgpythondir])
#     AC_MSG_NOTICE([relpkgpythondir:    $relpkgpythondir])
#     AC_MSG_NOTICE([pyexecdir:          $pyexecdir])
#     AC_MSG_NOTICE([pkgpyexecdir:       $pkgpyexecdir])
#     AC_MSG_NOTICE([============================================])
# 
#     
# 
#     AC_SUBST(HAVE_PYTHON)
# 
#     AC_SUBST(PYTHON)
#     AC_SUBST(PYTHON_VERSION)
#     AC_SUBST(PYTHON_LOCATION)
# 
#     AC_SUBST(PYTHON_CPPFLAGS)
#     AC_SUBST(PYTHON_LDFLAGS)
#     AC_SUBST(PYTHON_SAGA_MODULE_BASE)
# 
#     AC_SUBST(PYTHON_PACKAGE_PATH)
#   fi
# 
# ])

