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
CURRENT_DIR=`pwd`

SCRIPTPATH=`dirname "$(readlink -f $0)"`
cd "$SCRIPTPATH/.."

CURRENT_VERSION=`git describe`
if [ $? -ne 0 ]; then
  echo "No 'git tag -a <version> -m <message>' has been run"
  exit 255
fi

RELEASE_NAME=hlhdf_${CURRENT_VERSION}.tgz

# Cleanup the distribution if nessecary
make distclean 2> /dev/null

# This directory
THISDIR=`pwd`
HLHDFDIR=`basename "$THISDIR"`

TARBALLNAME="${CURRENT_DIR}/${RELEASE_NAME}"

echo "Will create release HLHDF $CURRENT_VERSION"
echo "In $TARBALLNAME"
echo ""
echo -n "Do you wish to continue (yes|no) ? [no]:"

RUNSCRIPT=no
read RUNSCRIPT
if [ $? -ne 0 ]; then
  exit 255;
fi

if [ "x$RUNSCRIPT" != "xyes" ]; then
  exit 255;
fi

# Create exclude file
TMPFILE=`tempfile`
if [ $? -ne 0 ]; then
  echo "Could not create temporary exclude file"
  exit 255
fi

cat << EOF > "$TMPFILE"
$HLHDFDIR/.git
$HLHDFDIR/.cproject
$HLHDFDIR/.project
$HLHDFDIR/.pydevproject
EOF

# Create the tar-ball
cd ..
tar --exclude-from "$TMPFILE" -cvzf "${CURRENT_DIR}/${RELEASE_NAME}" HLHDF

\rm -f "$TMPFILE"
