#!/bin/sh

CURRENT_DIR=`pwd`

SCRIPTPATH=`dirname $(readlink -f $0)`
cd $SCRIPTPATH/..

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
HLHDFDIR=`basename $THISDIR`

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

cat << EOF > $TMPFILE
$HLHDFDIR/.git
$HLHDFDIR/.cproject
$HLHDFDIR/.project
$HLHDFDIR/.pydevproject
EOF

# Create the tar-ball
cd ..
tar --exclude-from $TMPFILE -cvzf "${CURRENT_DIR}/${RELEASE_NAME}" HLHDF

\rm -f $TMPFILE
