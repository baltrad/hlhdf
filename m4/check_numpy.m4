#
# SYNOPSIS
#
#   CHECK_NUMPY(check-numpy, python-binary, python-include-flags)
#
#   - check-numpy            - if the numpy check should be searched for or not. If specifying "no", numpy check will be supressed
#   - python-binary          - the python binary that will be used with numpy
#   - python-include-flags   - when doing compile tests the include flags for including Python.h is necessary
#
#
# DESCRIPTION
#
#   Macro for identifying python numpy
#
#   The macro provides two options.
#
#   --with-numpy=[includepath] 
#                        where path should be the include path where the numpy include headers are residing. Default is to use the python binary to identify
#                        the include path. Otherwise you will need to provide --with-numpy
#
#
#   This macro will atempt to identify and extract all variables that are necessary to compile and build a python c module.
#   It has only been verified for python3. It assumes that python3 sysconfig exists.
#
#   The following variables will be set.
#   - NUMPY_INCLUDE    The include path for building with numpy
#
# LICENSE
#   Copyright (c) 2024 Anders Henja (anders@henjab.se)
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.
AC_DEFUN([CHECK_NUMPY], [

NUMPY_SUPRESSED=yes
NUMPY_FOUND=no
NUMPY_CFG_SUCCESS=no
NUMPY_INCLUDE=no

check_numpy_arg_python_do_check=$1
check_numpy_arg_python_binary=$2
check_numpy_arg_python_include=$3
check_numpy_cfg_error=no

# Add a default --with-numpy configuration option.
withval=
AC_ARG_WITH([numpy],
  AS_HELP_STRING(
    [--with-numpy=[<includepath>]],
            [location of the numpy include headers (excluding /numpy)]
  ),
  [ if test "$withval" != ""; then
      check_numpy_include_path="$withval"
    fi
  ],
  [check_numpy_include_path=""]
)

if [[ "$check_numpy_arg_python_do_check" != "no" ]]; then
  AC_MSG_NOTICE([Using python binary '$check_numpy_arg_python_binary'])

  if [[ "$check_numpy_include_path" == "" ]]; then
    AC_MSG_CHECKING([if it possible to import numpy])
    `$check_numpy_arg_python_binary -c "import numpy" >> /dev/null 2>&1`
    if [[ $? -ne 0 ]]; then
      AC_MSG_RESULT([no])
      check_numpy_cfg_error=yes
    else
      AC_MSG_RESULT([yes])
    fi

    if [[ "$check_numpy_cfg_error" == "no" ]]; then
      AC_MSG_CHECKING([for numpy major version])
      check_numpy_npy_version=`$check_numpy_arg_python_binary -c "import numpy; print(numpy.version.version.split('.')[[0]])"`
      if [[ "$check_numpy_npy_version" == "2" ]]; then
        NUMPY_VERSION=2
      else
        NUMPY_VERSION=1
      fi
      AC_MSG_RESULT([$NUMPY_VERSION])
    fi

    if [[ "$check_numpy_cfg_error" == "no" ]]; then
      # First we do the simplest way and try to use numpy to identify include directory
      AC_MSG_CHECKING([for numpy include using numpy.get_include])
      check_numpy_include_path=`$check_numpy_arg_python_binary -c "import numpy; print(numpy.get_include())"`
      if [[ $? -ne 0 ]]; then
        AC_MSG_RESULT([not working])
      else
        AC_MSG_RESULT([$check_numpy_include_path])
      fi
    fi
  fi

  if [[ "$check_numpy_cfg_error" == "no" ]]; then
    if [[ "$check_numpy_include_path" != "" ]]; then
      AC_MSG_CHECKING([for arrayobject.h])
      check_numpy_found_path=
      if [[ -f $check_numpy_include_path/arrayobject.h ]]; then
        check_numpy_found_path="$check_numpy_include_path"
      elif [[ -f $check_numpy_include_path/numpy/arrayobject.h ]]; then
			  check_numpy_found_path="$check_numpy_include_path/numpy"
      elif [[ -f $check_numpy_include_path/core/include/numpy/arrayobject.h ]]; then
        check_numpy_found_path="$check_numpy_include_path/core/include/numpy"
      elif [[ -f $numpy_inc/numpy/core/include/numpy/arrayobject.h ]]; then
        check_numpy_found_path="$check_numpy_include_path/numpy/core/include/numpy"
      fi
      AC_MSG_RESULT([$check_numpy_found_path])
    fi

    check_numpy_saved_CPPFLAGS="$CPPFLAGS"
  
    if [[ "$check_numpy_found_path" != "" ]]; then
      check_numpy_found_dirname=`dirname $check_numpy_found_path`
      CPPFLAGS="$CPPFLAGS $check_numpy_arg_python_include -I$check_numpy_found_dirname -I$check_numpy_found_path"
    fi

    check_numpy_cc_ok=no
    AC_CHECK_HEADERS(
      [arrayobject.h],
      [check_numpy_cc_ok=yes
       check_numpy_includes="-I$check_numpy_found_dirname -I$check_numpy_found_path"],
      [check_numpy_cc_ok=no]
      )

    CPPFLAGS="$check_numpy_saved_CPPFLAGS"

    if [[ "$check_numpy_cc_ok" != "yes" ]]; then
      AC_MSG_NOTICE([Could not compile with arrayobject.h])
    fi

    if [[ "$check_numpy_found_path" != "" ]]; then
      NUMPY_FOUND=yes
    fi
    NUMPY_CFG_SUCCESS=$check_numpy_cc_ok
    NUMPY_INCLUDE="$check_numpy_includes"
    NUMPY_SUPRESSED=no
  fi
else
  AC_MSG_NOTICE([numpy check suppressed])
fi

])
