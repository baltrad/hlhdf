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
# Description: Script that performs build tests of HL-HDF
#   (used by continous integration tools like hudson).
#
# Author(s):   Anders Henja
#
# Copyright:   Swedish Meteorological and Hydrological Institute, 2009
#
# History:  2009-10-09 Created by Anders Henja
#
# Usage: buildtest_hlhdf.sh <configure cmd> <buildnumber>
############################################################

if [ $# -ne 2 ]; then
  echo "Usage: $0 <configure cmd> <buildnumber>"
  exit 255
fi

CONFIGURECMD="$1"
RELNBR="$2"

SCRFILE=`python -c "import os;print(os.path.abspath(\"$0\"))"`
SCRIPTPATH=`dirname "$SCRFILE"`
cd "$SCRIPTPATH/.."

CURRENT_VERSION=`git describe`
if [ $? -ne 0 ]; then
  echo "No 'git tag -a <version> -m <message>' has been run"
  exit 255
fi

NEW_VERSION="$CURRENT_VERSION-build$RELNBR"

echo "Generating tar-ball..."
\rm -fr "/tmp/hlhdf-$NEW_VERSION/"
\rm -f "/tmp/hlhdf-$NEW_VERSION.tgz"
git archive --format=tar --prefix="hlhdf-$NEW_VERSION/" HEAD | (cd /tmp/ && tar xf -)
if [ $? -ne 0 ]; then
  echo "Failed to copy source files"
  \rm -fr "/tmp/hlhdf-$NEW_VERSION/"
  exit 255
fi
cat << EOF > "/tmp/hlhdf-$NEW_VERSION/.version"
$NEW_VERSION
EOF

if [ $? -ne 0 ]; then
  echo "Failed to create version file"
  \rm -fr "/tmp/hlhdf-$NEW_VERSION/"
  exit 255
fi

#
# CREATE A RELEASE SUMMARY TO KEEP TRACK ON CHANGES
#
if [ -f /tmp/hlhdf-$NEW_VERSION/RELEASE ]; then
  echo "RELEASE has been added to the source tree, this file should automatically be generated from the revision history"
  \rm -fr "/tmp/hlhdf-$NEW_VERSION/"
  exit 255
fi

NLINES=`git tag | wc -l | awk '{print $1}'`
LASTLINE=`expr $NLINES - 1`
LASTTAG=`git tag | sed "1,$LASTLINE d"`
COMMITLOG=`git log $LASTTAG.. --pretty=format:"%s"`
RHISTORY=`git tag -n10`

cat << EOF > "/tmp/hlhdf-$NEW_VERSION/RELEASE"
News for $NEW_VERSION: $TAG_MESSAGE

$COMMITLOG

Previous releases:
$RHISTORY
EOF

if [ $? -ne 0 ]; then
  echo "Failed to create RELEASE file"
  \rm -fr "/tmp/hlhdf-$NEW_VERSION/"
  exit 255
fi

cd /tmp/
tar -cvzf "/tmp/hlhdf-$NEW_VERSION.tgz" "hlhdf-$NEW_VERSION/"
if [ $? -ne 0 ]; then
  echo "Failed to create tar-ball"
  \rm -fr "/tmp/hlhdf-$NEW_VERSION/"
  exit 255
fi

echo "Running build and installation tests...."
$SCRIPTPATH/test_build_and_install.sh "/tmp/hlhdf-$NEW_VERSION.tgz" "$NEW_VERSION"
if [ $? -ne 0 ]; then
  echo "Build and installation tests failed"
  \rm -fr "/tmp/hlhdf-$NEW_VERSION/"
  \rm -f "/tmp/hlhdf-$NEW_VERSION.tgz"
  exit 255
fi

\rm -fr "/tmp/hlhdf-$NEW_VERSION/"
\rm -f "/tmp/hlhdf-$NEW_VERSION.tgz"

echo "Build and installation tests successful."

