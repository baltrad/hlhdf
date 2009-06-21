#!/bin/sh
############################################################
# Description: Script that performs the actual unit test of
#   HL-HDF.
#
# Author(s):   Anders Henja
#
# Copyright:   Swedish Meteorological and Hydrological Institute, 2009
#
# History:  2009-06-15 Created by Anders Henja
############################################################
if [ ! -f def.mk ]; then
  echo "configure has not been run"
  exit 255
fi

RESULT=0

TESTPYTHON=`fgrep COMPILE_FOR_PYTHON def.mk | sed -e"s/\(COMPILE_FOR_PYTHON=[ \t]*\)//"`

# RUN THE PYTHON TESTS
if test "${TESTPYTHON}" = "yes"; then
HDF5_LDPATH=`fgrep HDF5_LIBDIR def.mk | sed -e"s/\(HDF5_LIBDIR=[ \t]*-L\)//"`
export LD_LIBRARY_PATH="${HDF5_LDPATH}"
export PYHLPATH=`pwd`/pyhl

if test "${PYTHONPATH}" != ""; then
  export PYTHONPATH="${PYHLPATH}:${PYTHONPATH}"
else
  export PYTHONPATH="${PYHLPATH}"
fi

cd test/python
python HlhdfTestSuite.py
VAL=$?
if [ $VAL != 0 ]; then
  RESULT=$VAL
fi
fi

#RUN OTHER ESSENTIAL TESTS

# EXIT WITH A STATUS CODE, 0 == OK, ANY OTHER VALUE = FAIL
exit $RESULT

