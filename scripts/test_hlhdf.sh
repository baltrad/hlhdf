#!/bin/sh
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
# Description: Script that performs the actual unit test of
#   HL-HDF.
#
# Author(s):   Anders Henja
#
# Copyright:   Swedish Meteorological and Hydrological Institute, 2009
#
# History:  2009-06-15 Created by Anders Henja
############################################################
SCRFILE=`python -c "import os;print os.path.abspath(\"$0\")"`
SCRIPTPATH=`dirname "$SCRFILE"`

DEF_MK_FILE="${SCRIPTPATH}/../def.mk"

if [ ! -f "${DEF_MK_FILE}" ]; then
  echo "configure has not been run"
  exit 255
fi

RESULT=0

TESTPYTHON=`fgrep COMPILE_FOR_PYTHON "${DEF_MK_FILE}" | sed -e"s/\(COMPILE_FOR_PYTHON=[ \t]*\)//"`

# RUN THE PYTHON TESTS
if test "${TESTPYTHON}" = "yes"; then
HDF5_LDPATH=`fgrep HDF5_LIBDIR "${DEF_MK_FILE}" | sed -e"s/\(HDF5_LIBDIR=[ \t]*-L\)//"`
LIBHLHDFPATH="${SCRIPTPATH}/../hlhdf"
export LD_LIBRARY_PATH="${LIBHLHDFPATH}:${HDF5_LDPATH}"
export PYHLPATH="${SCRIPTPATH}/../pyhl"
export XRUNNERPATH="${SCRIPTPATH}/../test/lib"

if test "${PYTHONPATH}" != ""; then
  export PYTHONPATH="${PYHLPATH}:${XRUNNERPATH}:${PYTHONPATH}"
else
  export PYTHONPATH="${PYHLPATH}:${XRUNNERPATH}"
fi

cd "${SCRIPTPATH}/../test/python"
python HlhdfTestSuite.py
VAL=$?
if [ $VAL != 0 ]; then
  RESULT=$VAL
fi
\rm -f TEST-unittest.TestSuite.xml
python HlhdfXmlTestSuite.py
VAL=$?
if [ $RESULT -eq 0 -a $VAL -ne 0 ]; then
  RESULT=$VAL
fi

fi

#RUN OTHER ESSENTIAL TESTS

# EXIT WITH A STATUS CODE, 0 == OK, ANY OTHER VALUE = FAIL
exit $RESULT

