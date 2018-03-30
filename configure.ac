###########################################################################
# Copyright (C) 2009 Swedish Meteorological and Hydrological Institute, SMHI,
#
# This file is part of HLHDF.
#
# HLHDF is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# HLHDF is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with HLHDF.  If not, see <http://www.gnu.org/licenses/>.
###########################################################################

############################################################
#
# $Id: configure.ac,v 1.4 2008/03/25 12:30:33 adybbroe Exp $
#
# Description:	Configure script, tries to determine the
#		configuration of the host.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
dnl	This file is an input file used by the GNU "autoconf" program to
dnl	generate the file "configure", which is run during HLHDF
dnl	installation to configure the system for the local environment.

AC_INIT(./hlhdf/hlhdf.h)

HLHDF_VERSION=1.0
HLHDF_PATCH_LEVEL=0

HL_INSTALL=`pwd`/scripts/install-sh.sh

if test "${prefix}" = "NONE"; then
	prefix=/usr/local/hlhdf
fi

dnl Ensure that the wanted installation directory does not contain
dnl files that not are related to HLHDF.
BAD_INSTALL_DIR=no
if [[ -d "${prefix}" ]]; then
  if [[ -d "${prefix}/include" ]]; then
    PINS_INCL_FILES=`ls -1 "${prefix}/include" | grep -v hlhdf | grep -v pyhl`
    if [[ "x$PINS_INCL_FILES" != "x" ]]; then
      BAD_INSTALL_DIR=yes
    fi
  elif [[ -e "${prefix}/include" ]]; then
    AC_MSG_ERROR("${prefix}/include exists but is not a directory")
  fi
  if [[ -d "${prefix}/lib" ]]; then
    PINS_LIB_FILES=`ls -1 "${prefix}/lib" | grep -v hlhdf | grep -v pyhl`
    if [[ "x$PINS_LIB_FILES" != "x" ]]; then
      BAD_INSTALL_DIR=yes
    fi
  elif [[ -e "${prefix}/lib" ]]; then
    AC_MSG_ERROR("${prefix}/lib exists but is not a directory")
  fi
  if [[ -d "${prefix}/bin" ]]; then
    PINS_BIN_FILES=`ls -1 "${prefix}/bin" | grep -v hllist | grep -v hldec | grep -v hlenc | grep -v hlinstall`
    if [[ "x$PINS_BIN_FILES" != "x" ]]; then
      BAD_INSTALL_DIR=yes
    fi
  elif [[ -e "${prefix}/bin" ]]; then
    AC_MSG_ERROR("${prefix}/bin exists but is not a directory")
  fi
elif [[ -e "${prefix}" ]]; then
  AC_MSG_ERROR("${prefix} is not a directory")
fi

if [[ "x$BAD_INSTALL_DIR" = "xyes" ]]; then
  AC_MSG_ERROR("It is not allowed to install hlhdf in a directory that is shared between other components")
fi

ac_sys_system=`uname -s`
if test "$ac_sys_system" = "AIX" -o "$ac_sys_system" = "Monterey64" \
    -o "$ac_sys_system" = "UnixWare" -o "$ac_sys_system" = "OpenUNIX"; then
  ac_sys_release=`uname -v`
else
  ac_sys_release=`uname -r`
fi

AC_ARG_WITH(python,[  --with-python=(yes|no)     Compile for python],
	    ,withval=yes)

COMPILE_FOR_PYTHON=yes

case $withval in
  no)
	COMPILE_FOR_PYTHON=no
	;;
  *)
	;;
esac

PYBIN=python
py3support=no
AC_ARG_ENABLE(py3support,
  [  --enable-py3support     Builds rave with support for python3.],
  py3support=$enableval)
if [[ "x$py3support" = "xyes" ]]; then
  PYOPT="$PYOPT -DPY3_SUPPORT"
  PYBIN=python3
  COMPILE_FOR_PYTHON=yes
fi

PYTHON_BINARY=$PYBIN

# Check for machine type or hardware-platform (32 bit e.g. = i386 or 64 bit = x86_64):
bit64="no"
AC_MSG_CHECKING([hardware platform])
hardware=`uname -m`
if test "$?" -ne 0; then
    AC_MSG_RESULT([unknown (can't find uname command)])
    hardware=unknown
else
    if [[ "x$hardware" = "xx86_64" ]]; then
      bit64="yes"
    fi
    AC_MSG_RESULT($hardware)
fi

# Check for programs which are useful when generating
# the makefile.
# Hmm, I'd better check the compiler after the python check,
# seems like I've got slight problems otherwise
#
AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CPP
AC_SUBST(AR)
AC_CHECK_PROGS(AR, ar aal, ar)

# Check for python version
PYVERSION=
if [[ "x$COMPILE_FOR_PYTHON" = "xyes" ]]; then
  AC_MSG_CHECKING(for python version)
  PYVERSION=`$PYBIN -c "import sys; print(sys.version[[:3]])"`
  AC_MSG_RESULT($PYVERSION)
fi

# Check if python has got distutils (makes life a whole lot easier)
GOTDISTUTILS=no
enabledistutils=yes
AC_ARG_ENABLE(distutil-check,
  [  --enable-distutil-check     Atempts to use python distutils for inquirying for settings. Defaults to yes.],
  enabledistutils=$enableval)
  
if [[ "x$COMPILE_FOR_PYTHON" = "xyes" -a "x$enabledistutils" = "xyes" ]]; then
  AC_MSG_CHECKING(for python distutils)
  GOTDISTUTILS="no"
  XYZ=`$PYBIN -c "from distutils import sysconfig"`
  if [[ $? -eq 0 ]]; then
    GOTDISTUTILS="yes"
  fi
  AC_MSG_RESULT($GOTDISTUTILS)
fi

# Check for exec install dir if distutils not should be used
PY_EXEC_INSTALLDIR=
if [[ "x$COMPILE_FOR_PYTHON" = "xyes" -a "x$GOTDISTUTILS" = "xno" ]]; then
  AC_MSG_CHECKING(for python install dir)
  PY_EXEC_INSTALLDIR=`$PYBIN -c "import sys; print(sys.exec_prefix)"`
  AC_MSG_RESULT($PY_EXEC_INSTALLDIR)
fi

# Atempt to locate Makefile used for building python
PY_CONFIG_MAKE=
if [[ "x$COMPILE_FOR_PYTHON" = "xyes" ]]; then
  AC_MSG_CHECKING(for python Makefile)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PY_CONFIG_MAKE=`$PYBIN -c "from distutils import sysconfig; print(sysconfig.get_makefile_filename())" | grep -v "^None\$"`
  else
    if [[ "x$bit64" = "xyes" ]]; then
      PY_CONFIG_MAKE="$PY_EXEC_INSTALLDIR/lib64/python$PYVERSION/config/Makefile"
    fi
    if [[ ! -f "$PY_CONFIG_MAKE" ]]; then
      PY_CONFIG_MAKE="$PY_EXEC_INSTALLDIR/lib/python$PYVERSION/config/Makefile"
    fi
  fi
  
  if [[ ! -f "$PY_CONFIG_MAKE" ]]; then
    AC_MSG_ERROR(failed to locate Makefile)
  else
    AC_MSG_RESULT($PY_CONFIG_MAKE)
  fi
fi

#
# Extract what compiler the python binary was compiled with
#
AC_MSG_CHECKING(for the compiler used for generating python)
if [[ "x$COMPILE_FOR_PYTHON" = "xyes" ]]; then
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PYCC=`$PYBIN -c "from distutils import sysconfig; print(sysconfig.get_config_var(\"CC\"))" | grep -v "^None\$"`
  else
    PYCC=`cat $PY_CONFIG_MAKE | sed -n "/^CC=/p" | sed -e "s/CC=//g" | awk '{print $1}'`
  fi
  AC_MSG_RESULT($PYCC)
  CC=$PYCC
else
  AC_MSG_RESULT(suppressed)
fi

dnl Check for programs which are useful when generating
dnl the makefile.
dnl Check for fortran unless --with-fortran=no
COMPILE_FOR_FORTRAN=no
AC_ARG_WITH(fortran,[  --with-fortran=yes|no Configure with fortran],
		,withval=no)
case $withval in
  yes)
	AC_PROG_F77
	COMPILE_FOR_FORTRAN="yes"
	;;
  *)
	;;
esac

#Note, since I've worked a little to determine fortran compiler,
#I must switch back to 'C' in order to get the rest to work.
AC_LANG_C

AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CPP
AC_SUBST(AR)
AC_CHECK_PROGS(AR, ar aal, ar)

PY_INCLUDES=
PY_SITEPACK=
PY_LIBDIR=
PYOPT=
PYLDFLAGS=
PYLDSHARED=
PYCCSHARED=
PYDEFS=

if [[ "x$COMPILE_FOR_PYTHON" = "xyes" ]]; then
  AC_MSG_CHECKING(for python includes)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PY_INCLUDES=`$PYBIN -c "from distutils import sysconfig;print(sysconfig.get_python_inc())" | grep -v "^None\$"`
  else
    PY_INCLUDES="$PY_EXEC_INSTALLDIR/include/python$PYVERSION"
  fi
  AC_MSG_RESULT($PY_INCLUDES)
  PY_INCLUDES="-I$PY_INCLUDES"

  AC_MSG_CHECKING(for python lib site-packages)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PY_SITEPACK=`$PYBIN -c "from distutils import sysconfig; print(sysconfig.get_python_lib())" | grep -v "^None\$"`
  else
    PY_SITEPACK="$PY_EXEC_INSTALLDIR/lib/python$PYVERSION/site-packages"
  fi
  AC_MSG_RESULT($PY_SITEPACK)

  AC_MSG_CHECKING(for python library dir)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PY_LIBDIR=`$PYBIN -c "from distutils import sysconfig; print(sysconfig.get_config_var('LIBDIR'))" | grep -v "^None\$"`
  else
    PY_LIBDIR="$PY_EXEC_INSTALLDIR/lib"
  fi
  AC_MSG_RESULT($PY_LIBDIR)
  
  dnl Checks for typedefs, structures and compiler characteristics
  AC_MSG_CHECKING(for python options)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PYOPT=`$PYBIN -c "from distutils import sysconfig; print(sysconfig.get_config_var(\"OPT\"))" | grep -v "^None\$"`
  else
    PYOPT=`cat $PY_CONFIG_MAKE | sed -n "/^OPT=/p" | sed -n -e"s/^OPT=[[ 	]]*\(.*\)/\1/p"`
  fi
  AC_MSG_RESULT($PYOPT)

  AC_MSG_CHECKING(for ldflags)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PYLDFLAGS=`$PYBIN -c "from distutils import sysconfig; print(sysconfig.get_config_var(\"LDFLAGS\"))" | grep -v "^None\$"`
  else
    PYLDFLAGS=`cat $PY_CONFIG_MAKE | sed -n "/^LDFLAGS=/p" | sed -n -e"s/^LDFLAGS=[[  ]]*\(.*\)/\1/p"`
  fi
  AC_MSG_RESULT($PYLDFLAGS)

  AC_MSG_CHECKING(for ldshared)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PYLDSHARED=`$PYBIN  -c "from distutils import sysconfig; print(sysconfig.get_config_var(\"LDSHARED\"))" | grep -v "^None\$"`
  else
    PYLDSHARED=`cat $PY_CONFIG_MAKE | sed -n "/^LDSHARED=/p" | sed -n -e"s/^LDSHARED=[[   ]]*\(.*\)/\1/p"`
  fi
  AC_MSG_RESULT($PYLDSHARED)

  AC_MSG_CHECKING(for ccshared)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PYCCSHARED=`$PYBIN  -c "from distutils import sysconfig; print(sysconfig.get_config_var(\"CCSHARED\"))" | grep -v "^None\$"`
  else
    PYCCSHARED=`cat $PY_CONFIG_MAKE | sed -n "/^CCSHARED=/p" | sed -n -e"s/^CCSHARED=[[ 	]]*\(.*\)/\1/p"`
  fi
  AC_MSG_RESULT($PYCCSHARED)

  AC_MSG_CHECKING(for defs)
  if [[ "x$GOTDISTUTILS" = "xyes" ]]; then
    PYDEFS=`$PYBIN  -c "from distutils import sysconfig; xyz=sysconfig.get_config_var(\"DEFS\")" | grep -v "^None\$"`
  else
    PYDEFS=`cat $PY_CONFIG_MAKE | sed -n "/^DEFS=/p" | sed -n -e"s/^DEFS=[[ 	]]*\(.*\)/\1/p"`
  fi
  AC_MSG_RESULT($PYDEFS)
else
AC_MSG_CHECKING(CCSHARED)
if test -z "$CCSHARED"
then
  case $ac_sys_system/$ac_sys_release in
    SunOS*)
      if test "$GCC" = yes; then 
        PYCCSHARED="-fPIC";
      elif test `uname -p` = sparc; then 
        PYCCSHARED="-xcode=pic32";
      else
        PYCCSHARED="-Kpic";
      fi
      ;;
    hp*|HP*)
      if test "$GCC" = yes; then
        PYCCSHARED="-fPIC";
      else 
        PYCCSHARED="+z";
      fi
      ;;
    Linux*|GNU*)
      PYCCSHARED="-fPIC"
      ;;
  esac
else
  PYCCSHARED="$CCSHARED"
fi
AC_MSG_RESULT($PYCCSHARED)

AC_MSG_CHECKING(LDSHARED)
if test -z "$LDSHARED"
then
  case $ac_sys_system/$ac_sys_release in
    IRIX/5*) PYLDSHARED="ld -shared";;
    IRIX*/6*) PYLDSHARED="ld ${SGI_ABI} -shared -all";;
    SunOS/5*) 
      if test "$GCC" = "yes"
        then PYLDSHARED='$(CC) -shared'
      else PYLDSHARED='$(CC) -G';
      fi ;;
    hp*|HP*)
      if test "$GCC" = "yes"
        then PYLDSHARED='$(CC) -shared'
      else PYLDSHARED='ld -b';
      fi 
      ;;
    Darwin/*)
      PYLDSHARED='$(CC) $(LDFLAGS) -bundle'
      ;;
    Linux*|GNU*|QNX*)
      PYLDSHARED='$(CC) -shared'
      ;;
    *)
      PYLDSHARED="ld"
      ;;
  esac
else
  PYLDSHARED="$LDSHARED"
fi
AC_MSG_RESULT($PYLDSHARED)
fi

dnl -----------------------------
dnl Try to locate the numpy include file 'arrayobject.h' automatically,
dnl if this does not succeed, then the user has to specify this argument
dnl manually.
dnl -----------------------------
AC_ARG_WITH(numpy,[  --with-numpy=INC     Use the numpy include file],
		,withval=yes)
NUMPY_INCLUDE_DIR=
if [[ "x$COMPILE_FOR_PYTHON" = "xyes" ]]; then
  case $withval in
	no)
		AC_MSG_ERROR(You can not remove dependencies to numpy)
		;;
	*)
		numpy_inc="`echo $withval`"
		FOUND_NUMPY_INC=no
		numpy_inc_app=
      if test "xyes" = "x$numpy_inc"; then
         TESTNUMPYDIR=`$PYBIN -c "import numpy; print(eval(\"numpy.__\"+\"file__\"))"`
         if test $? -ne 0; then
            numpy_inc="$PY_EXEC_INSTALLDIR/include/python$PYVERSION"
         else
            numpy_inc=`echo $TESTNUMPYDIR | sed -e"s/\/__init__.pyc//" | sed -e"s/\/__init__.py//"`
         fi
		fi
		if [[ -f $numpy_inc/arrayobject.h ]]; then
			numpy_inc_app=
			FOUND_NUMPY_INC=yes
		elif [[ -f $numpy_inc/numpy/arrayobject.h ]]; then
			numpy_inc_app="/numpy"
			FOUND_NUMPY_INC=yes
      elif [[ -f $numpy_inc/core/include/numpy/arrayobject.h ]]; then
          numpy_inc_app="/core/include/numpy"
          FOUND_NUMPY_INC=yes
      elif [[ -f $numpy_inc/numpy/core/include/numpy/arrayobject.h ]]; then
          numpy_inc_app="/numpy/core/include/numpy"
          FOUND_NUMPY_INC=yes
		fi
		if test "xyes" != "x$FOUND_NUMPY_INC"; then
			AC_MSG_ERROR(Could not locate arrayobject.h)
		fi
		NUMPY_INCLUDE_DIR="-I$numpy_inc$numpy_inc_app"
		;;
  esac
else
  AC_MSG_CHECKING(for numpy)
  AC_MSG_RESULT(suppressed)
fi

dnl -----------------------------
dnl Is the GNU zlib present? It has a header file 'zlib.h' and a library -lz.
dnl Their locations might be specified with the --enable-zlib'
dnl command-line switch. The value is an include path and/or a library path.
dnl It the library path is specified then it must be preceded by a comma.
dnl -----------------------------
ZLIB_INCDIR=
ZLIB_LIBDIR=

AC_ARG_WITH(zlib,[  --with-zlib=INC,LIB     Use the GNU zlib compression],
            ,withval=yes)
case $withval in
    yes)
	AC_CHECK_HEADERS(zlib.h)
	AC_CHECK_LIB(z, compress)
	;;
    no)
	AC_MSG_CHECKING(for GNU zlib)
	AC_MSG_RESULT(suppressed)
	;;
    *)
	zlib_inc="`echo $withval |cut -f1 -d,`"
	if test "X" != "$zlib_inc"; then
	    saved_CPPFLAGS="$CPPFLAGS"
	    CPPFLAGS="$CPPFLAGS -I$zlib_inc"
	    AC_CHECK_HEADERS(zlib.h,ZLIB_INCDIR="-I$zlib_inc",CPPFLAGS="$saved_CPPFLAGS")
        else
	    AC_CHECK_HEADERS(zlib.h)
	fi

	zlib_lib="`echo $withval |cut -f2 -d, -s`"
	if test "X" != "$zlib_lib"; then
	    saved_LDFLAGS="$LDFLAGS"
	    LDFLAGS="$LDFLAGS -L$zlib_lib"
	    AC_CHECK_LIB(z, compress,ZLIB_LIBDIR="-L$zlib_lib",LDFLAGS="$saved_LDFLAGS")
	else
	    AC_CHECK_LIB(z, compress)
	fi
	;;
esac

GOT_COMPRESS2=no
AC_CHECK_LIB(z, compress2,GOT_COMPRESS=yes,GOT_COMPRESS=no)

dnl Check for how to force static/dynamic libraries
dnl Currently only supported are -Bstatic/-Bdynamic
dnl and -no_so/-so_archive
dnl Also, the first to try should be -Xlinker -Bdynamic and
dnl -Xlinker -Bstatic since the gnu compiler behaves a bit
dnl different and just warns when options not are supported.
LINKER_FORCED_SHARED=""
LINKER_FORCED_STATIC=""

LIBS="-Xlinker -Bstatic -lz -Xlinker -Bdynamic"

AC_MSG_CHECKING(Gnu(Linux) linker options ($LIBS))
AC_TRY_RUN([
#include <stdio.h>
main()
{
    int i;
    return 0;
}],try_ok=yes,try_ok=no,try_ok=no)

if [[ "x$try_ok" = "xyes" ]]; then
  AC_MSG_RESULT("Yes")
  LINKER_FORCED_SHARED="-Xlinker -Bdynamic"
  LINKER_FORCED_STATIC="-Xlinker -Bstatic"
else
  AC_MSG_RESULT("No")
fi

dnl If above doesn't work it might just be that there is no libz.a so just try
dnl if you can use a dynamiz libz instead.

if [[ "$try_ok" = "no" ]]; then
LIBS=" -Xlinker -Bdynamic -lz"
AC_MSG_CHECKING(Gnu(Linux) linker options ($LIBS))
AC_TRY_RUN([
#include <stdio.h>
main()
{
    int i;
    return 0;
}],try_ok=yes,try_ok=no,try_ok=no)
if [[ "x$try_ok" = "xyes" ]]; then
  AC_MSG_RESULT("Yes")
  LINKER_FORCED_SHARED="-Xlinker -Bdynamic"
  LINKER_FORCED_STATIC="-Xlinker -Bstatic"
else
  AC_MSG_RESULT("No")
fi
fi

if [[ "x$LINKER_FORCED_SHARED" = "x" ]]; then
  LIBS="-no_so -lz -so_archive"
  AC_MSG_CHECKING(OSF1 linker options ($LIBS))
  AC_TRY_RUN([
#include <stdio.h>
main()
{
     int i;
     return 0;
}],try_ok=yes,try_ok=no,try_ok=no)

  if [[ "x$try_ok" = "xyes" ]]; then
    AC_MSG_RESULT("Yes")
    LINKER_FORCED_SHARED="-so_archive"
    LINKER_FORCED_STATIC="-no_so"
  else
    AC_MSG_RESULT("No")
  fi
fi

if [[ "x$LINKER_FORCED_SHARED" = "x" ]]; then
  LIBS="-Bstatic -lz -Bdynamic"
  AC_MSG_CHECKING(Sun linker options ($LIBS))
  AC_TRY_RUN([
#include <stdio.h>
main()
{
    int i;
    return 0;
}],try_ok=yes,try_ok=no,try_ok=no)

  if [[ "x$try_ok" = "xyes" ]]; then
    AC_MSG_RESULT("Yes")
    LINKER_FORCED_SHARED="-Bdynamic"
    LINKER_FORCED_STATIC="-Bstatic"
  else
    AC_MSG_RESULT("No")
  fi
fi

if [[ "x$LINKER_FORCED_SHARED" = "x" ]]; then
  LIBS="-static -lz -dynamic"
  AC_MSG_CHECKING(Linux(Should never work) linker options ($LIBS))
  AC_TRY_RUN([
#include <stdio.h>
main()
{
    int i;
    return 0;
}],try_ok=yes,try_ok=no,try_ok=no)

  if [[ "x$try_ok" = "xyes" ]]; then
    AC_MSG_RESULT("Yes")
    LINKER_FORCED_SHARED="-dynamic"
    LINKER_FORCED_STATIC="-static"
  else
    AC_MSG_RESULT("No")
  fi
fi

dnl -----------------------------
dnl Is the HDF5 lib present? One header is 'hdf5.h' and a library -lhdf5 (static).
dnl Their locations might be specified with the --with-hdf5'
dnl command-line switch. The value is an include path and/or a library path.
dnl It the library path is specified then it must be preceded by a comma.
dnl -----------------------------
HDF5_INCDIR=
HDF5_LIBDIR=
GOT_HDF5_INC=no
GOT_HDF5_LIB=no

AC_ARG_WITH(hdf5,[  --with-hdf5=INC,LIB       Use the HDF5 library],
		,withval=yes)
case $withval in
	yes)
		AC_CHECK_HEADERS(hdf5.h,GOT_HDF5_INC=yes)
		AC_CHECK_LIB(hdf5,H5Fopen,GOT_HDF5_LIB=yes)
		;;
	no)
		AC_MSG_CHECKING(for HDF5 lib)
		AC_MSG_RESULT(suppressed)
		;;
	*)
		hdf5_inc="`echo $withval |cut -f1 -d,`"
		if test "X" != "$hdf5_inc"; then
			saved_CPPFLAGS="$CPPFLAGS"
			CPPFLAGS="$CPPFLAGS -I$hdf5_inc"
			AC_CHECK_HEADERS(hdf5.h,GOT_HDF5_INC=yes)
			if [[ "x$GOT_HDF5_INC" = "xyes" ]]; then
				HDF5_INCDIR="-I$hdf5_inc"
			else
				CPPFLAGS="$saved_CPPFLAGS"
			fi
		else
			AC_CHECK_HEADERS(hdf5.h,GOT_HDF5_INC=yes)
		fi

		hdf5_lib="`echo $withval |cut -f2 -d,`"
		if test "X" != "$hdf5_lib"; then
			saved_LDFLAGS="$LDFLAGS"
			LIBS="-lz -lm"
			LDFLAGS="$LDFLAGS -L$hdf5_lib"
			AC_CHECK_LIB(hdf5,H5Fopen,GOT_HDF5_LIB=yes)

			# Hmm, might be touchy about the /usr/lib, lets try.
			if [[ "x$GOT_HDF5_LIB" != "xyes" ]]; then
				LDFLAGS="$LDFLAGS -L$hdf5_lib -L/usr/lib"
				AC_CHECK_LIB(hdf5,H5Fopen,GOT_HDF5_LIB=yes)
			fi	
			if [[ "x$GOT_HDF5_LIB" = "xyes" ]]; then
				HDF5_LIBDIR="-L$hdf5_lib"
			else
				LDFLAGS="$saved_LDFLAGS"
			fi
		else
			AC_CHECK_LIB(hdf5,H5Fopen,GOT_HDF5_LIB=yes)
		fi
		;;
esac

if [[ "x$GOT_HDF_INC" = "xno" -o "x$GOT_HDF_LIB" = "xno" ]]; then
	AC_MSG_ERROR("Could not successfully find the hdf5 library")
fi

dnl -----------------------------
dnl Is the szlib present? It has a header file 'szlib.h' and a library -lsz.
dnl Their locations might be specified with the --enable-szlib'
dnl command-line switch. The value is an include path and/or a library path.
dnl It the library path is specified then it must be preceded by a comma.
dnl -----------------------------
SZLIB_INCDIR=
SZLIB_LIBDIR=
GOT_SZ_COMPRESS=no

AC_ARG_WITH(szlib,[  --with-szlib=INC,LIB     Use the szlib compression],
            ,withval=yes)
case $withval in
    yes)
	AC_CHECK_HEADERS(szlib.h)
	AC_CHECK_LIB(sz, SZ_Compress)
	;;
    no)
	AC_MSG_CHECKING(for szlib)
	AC_MSG_RESULT(suppressed)
	;;
    *)
	szlib_inc="`echo $withval |cut -f1 -d,`"
	if test "X" != "$szlib_inc"; then
	    saved_CPPFLAGS="$CPPFLAGS"
	    CPPFLAGS="$CPPFLAGS -I$szlib_inc"
	    AC_CHECK_HEADERS(szlib.h,SZLIB_INCDIR="-I$szlib_inc",CPPFLAGS="$saved_CPPFLAGS")
        else
	    AC_CHECK_HEADERS(szlib.h)
	fi

	szlib_lib="`echo $withval |cut -f2 -d, -s`"
        echo "szlib_lib=$szlib_lib"
	if test "X" != "$szlib_lib"; then
	    saved_LDFLAGS="$LDFLAGS"
	    LDFLAGS="$LDFLAGS -L$szlib_lib"
            AC_CHECK_LIB(sz,SZ_Compress,GOT_SZ_COMPRESS=yes,GOT_SZ_COMPRESS=no)
            if test "x$GOT_SZ_COMPRESS" != "xyes"; then
              LDFLAGS="$saved_LDFLAGS"
            else
              SZLIB_LIBDIR="-L$szlib_lib"
            fi
	    dnl AC_CHECK_LIB(sz, SZ_Compress,SZLIB_LIBDIR="-L$szlib_lib",LDFLAGS="$saved_LDFLAGS")
	else
	    AC_CHECK_LIB(sz, SZ_Compress,GOT_SZ_COMPRESS=yes,GOT_SZ_COMPRESS=no)
	fi
	;;
esac

dnl AC_CHECK_LIB(sz, SZ_Compress,GOT_SZ_COMPRESS=yes,GOT_SZ_COMPRESS=no)

dnl Check if the machine is big or little endian
dnl Produces one error during autoconf, but it doesn't
dnl seem to matter.
AC_C_BIGENDIAN
if [[ "x$ac_cv_c_bigendian" = "xyes" ]]; then
  BYTE_ORDER=-DBYTE_ORDER_BIG_ENDIAN
else
  BYTE_ORDER=-DBYTE_ORDER_LITTLE_ENDIAN
fi

PYDEFS="$PYDEFS $BYTE_ORDER"

dnl It is possible to turn on memory debugging within HLHDF
debugmemory=no
AC_ARG_ENABLE(debug-memory,
  [  --enable-debug-memory     Turns on the hlhdf memory debugging. This should usually not be activated.],
  debugmemory=$enableval)
if [[ "x$debugmemory" = "xyes" ]]; then
  PYOPT="$PYOPT -DHLHDF_MEMORY_DEBUG"
fi

dnl Okie, now I'd better try to extract all information I can about
dnl the fortran.
AC_MSG_CHECKING([system version])
system=`uname -s`-`uname -r`
if test "$?" -ne 0; then
    AC_MSG_RESULT([unknown (can't find uname command)])
    system=unknown
else
    # Special check for weird MP-RAS system (uname returns weird
    # results, and the version is kept in special file).
    
    if test -r /etc/.relid -a "X`uname -n`" = "X`uname -s`" ; then
        system=MP-RAS-`awk '{print $3}' /etc/.relid`
    fi
    if test "`uname -s`" = "AIX" ; then
	system=AIX-`uname -v`.`uname -r`
    fi
    AC_MSG_RESULT($system)
fi

# This is not very general, but hey, I don't know any other
# way to do this, when trying to use AC_F77_LIBRARY_LDFLAGS it fails
# quite bad.
FORTRAN_CLINK_LIBS=
FORTRAN_CLINK_LDFLAG=
if [[ "x$COMPILE_FOR_FORTRAN" != "xno" ]]; then
  try_ok=no
  LIBS=
  LDFLAGS=
  # Step 3: Check the fortran libraries, so that they can be loaded
  case $system in
    OSF1*)
      LDFLAGS=-L/usr/ccs/lib/cmplrs/fortrtl
      LIBS="$LINKER_FORCED_STATIC -lfor -lUfor -lFutil $LINKER_FORCED_SHARED"
      ;;
    SunO*)
      LDFLAGS=-L/opt/SUNWspro/SC4.2/lib
      LIBS="$LINKER_FORCED_STATIC -lF77 -lM77 -lsunmath $LINKER_FORCED_SHARED"
      ;;
    Linux*)
      LDFLAGS=-L/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.90.29
      LIBS="$LINKER_FORCED_STATIC -lf2c $LINKER_FORCED_SHARED"
      ;;
    *)
      AC_MSG_RESULT("The configure cant determine what fortran library that should be used on $system")
      ;;
  esac

  if [[ "x$LDFLAGS" != "x" ]]; then
    AC_MSG_CHECKING([Linker Options on $system with $LDFLAGS $LIBS])
    AC_TRY_RUN([
#include <stdio.h>
main()
{
  int i;
  return 0;
}],try_ok=yes,try_ok=no,try_ok=no)
  fi

  if [[ "x$try_ok" = "xyes" ]]; then
    AC_MSG_RESULT(Yes)
    FORTRAN_CLINK_LIBS="$LIBS"
    FORTRAN_CLINK_LDFLAG="$LDFLAGS"
  else
    AC_MSG_RESULT(No try do it manually)
  fi
fi

dnl Found some problems with the compiler options when using a HP-UX compiler, try
dnl if it takes the -Ae flag (ansi extended)
try_ok=no
case $system in
  HP-UX*)
    AC_MSG_CHECKING(HP-UX if compiler option -Ae is reckognized by compiler)
    CPPFLAGS="-Ae $PYDEFS -I. $HDF5_INCDIR $ZLIB_INCDIR"
    LDFLAGS="-L. $HDF5_LIBDIR $ZLIB_LIBDIR"
    LIBS="$LINKER_FORCED_STATIC -lhdf5 -lz $LINKER_FORCED_SHARED -lm"
    AC_TRY_RUN([
#include <hdf5.h>
main()
{
  return 0;
}],try_ok=yes,try_ok=no,try_ok=no)
    if [[ "x$try_ok" = "xyes" ]]; then
      PYDEFS="-Ae $PYDEFS"
      AC_MSG_RESULT(Yes)
    else
      AC_MSG_RESULT(No)
    fi
    ;;
  *)
    ;;
esac

dnl Substitue all variables
AC_SUBST(COMPILE_FOR_PYTHON)
AC_SUBST(PYTHON_BINARY)
AC_SUBST(CC)
AC_SUBST(PY_INCLUDES)
AC_SUBST(PY_SITEPACK)
AC_SUBST(PY_LIBDIR)
AC_SUBST(HLHDF_VERSION)
AC_SUBST(HLHDF_PATCH_LEVEL)
AC_SUBST(HL_INSTALL)
AC_SUBST(PYOPT)
AC_SUBST(PYLDFLAGS)
AC_SUBST(PYLDSHARED)
AC_SUBST(PYCCSHARED)
AC_SUBST(PYDEFS)
AC_SUBST(LINKER_FORCED_SHARED)
AC_SUBST(LINKER_FORCED_STATIC)
AC_SUBST(FORTRAN_CLINK_LIBS)
AC_SUBST(FORTRAN_CLINK_LDFLAG)

#Substitute the numpy argument
AC_SUBST(NUMPY_INCLUDE_DIR)

#Substitute the ZLIB and HDF5 arguments
AC_SUBST(ZLIB_INCDIR)
AC_SUBST(ZLIB_LIBDIR)
AC_SUBST(GOT_SZ_COMPRESS)
AC_SUBST(SZLIB_INCDIR)
AC_SUBST(SZLIB_LIBDIR)
AC_SUBST(HDF5_INCDIR)
AC_SUBST(HDF5_LIBDIR)

AC_OUTPUT(def.mk)
