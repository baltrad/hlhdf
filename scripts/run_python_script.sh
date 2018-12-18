#!/bin/sh
############################################################
# Description: Script sets up the nessecary environment variables
#  to be able to run a python script with the source code version
#  of _pyhl.
#
# Author(s):   Anders Henja
#
# Copyright:   Swedish Meteorological and Hydrological Institute, 2009
#
# History:  2009-08-12 Created by Anders Henja
############################################################
SCRIPTPATH=`dirname "$(readlink -f $0)"`

DEF_MK_FILE="${SCRIPTPATH}/../def.mk"

if [ ! -f "${DEF_MK_FILE}" ]; then
  echo "configure has not been run"
  exit 255
fi

RESULT=0

TESTPYTHON=`fgrep COMPILE_FOR_PYTHON "${DEF_MK_FILE}" | sed -e"s/\(COMPILE_FOR_PYTHON=[ \t]*\)//"`

PYTHON_BIN=`fgrep PYTHON_BINARY "${DEF_MK_FILE}" | sed -e"s/\(PYTHON_BINARY=[ \t]*\)//"`

# RUN THE PYTHON TESTS
if test "${TESTPYTHON}" = "yes"; then
HDF5_LDPATH=`fgrep HDF5_LIBDIR "${DEF_MK_FILE}" | sed -e "s/\(HDF5_LIBDIR=[ \t]*\)\(-L\)*//"`
LIBHLHDFPATH="${SCRIPTPATH}/../hlhdf"
if test "${HDF5_LDPATH}" != ""; then
  export LD_LIBRARY_PATH="${LIBHLHDFPATH}:${HDF5_LDPATH}:${LD_LIBRARY_PATH}"
else
  export LD_LIBRARY_PATH="${LIBHLHDFPATH}:${LD_LIBRARY_PATH}"
fi

export PYHLPATH="${SCRIPTPATH}/../pyhl"

if test "${PYTHONPATH}" != ""; then
  export PYTHONPATH="${PYHLPATH}:${PYTHONPATH}"
else
  export PYTHONPATH="${PYHLPATH}"
fi

$PYTHON_BIN $*
VAL=$?
if [ $VAL != 0 ]; then
  RESULT=$VAL
fi
fi

#RUN OTHER ESSENTIAL TESTS

# EXIT WITH A STATUS CODE, 0 == OK, ANY OTHER VALUE = FAIL
exit $RESULT


