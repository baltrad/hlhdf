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
#
# Prepares the installation directory and makes necessary changes when upgrading
# from an old version of HLHDF to the new version
#
# @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
# @date 2009-08-19
###########################################################################

create_directory() {
  CREATEDIR=$1
  if [ ! -d "$CREATEDIR" ]; then
    mkdir "$CREATEDIR"
    if [ $? -ne 0 ]; then
      echo "Could not create directory $CREATEDIR"
      return 255
    fi
  fi
  return 0
}

replace_path_with_symbolic_link() {
  PATHTOMOVE=$1
  MOVETOPATH=$2
  
  if [ -e "$PATHTOMOVE" ]; then
    \mv "$PATHTOMOVE" "$MOVETOPATH"
    if [ $? -ne 0 ]; then
      echo "Failed to move file"
      return 255
    fi
  fi

  \ln -s "$MOVETOPATH" "$PATHTOMOVE" 
  if [ $? -ne 0 ]; then
    echo "Failed to create symbolic link from $MOVETOPATH to $PATHTOMOVE"
    \mv "$MOVETOPATH" "$PATHTOMOVE"
  fi
  return 0
}

create_symbolic_link() {
  TOPATH=$1
  FROMPATH=$2
  \ln -s "$FROMPATH" "$TOPATH" 
  if [ $? -ne 0 ]; then
    echo "Failed to create symbolic link from $FROMPATH to $TOPATH"
    return 255
  fi
  return 0
}

move_path() {
  FROMPATH=$1
  TOPATH=$2
  \mv "$FROMPATH" "$TOPATH" 2>&1 >> /dev/null
  if [ $? -ne 0 ]; then
    echo "Failed to move $FROMPATH to $TOPATH"
    return 255
  fi
  return 0
}

remove_path() {
  PATHTOREMOVE=$1
  if [ -e "$PATHTOREMOVE" -o -h "$PATHTOREMOVE" ]; then
    \rm  "$PATHTOREMOVE" 2>&1 >> /dev/null
    if [ $? -ne 0 ]; then
      echo "Failed to remove path $PATHTOREMOVE"
      return 255
    fi
  fi
  return 0
}

if [ $# -ne 1 ]; then
  echo "Usage prepare_installdir <directory>"
  exit 255
fi
INSTALLDIR=$1

SCRFILE=`python -c "import os;print os.path.abspath(\"$0\")"`
SCRIPTPATH=`dirname "$SCRFILE"`
VERSION=default
if [ -f "$SCRIPTPATH/../.version" ]; then
  VERSION=`cat "$SCRIPTPATH/../.version"`
fi

NEWHLHDF=1
DIREXISTS=0
if [ -d "$INSTALLDIR" ]; then
  DIREXISTS=1
  echo "Directory $INSTALLDIR already exists, checking if new or old HLHDF release"
  if [ ! -f "$INSTALLDIR/.version" ]; then
    echo "$INSTALLDIR/.version does not exist so this is most likely an old version"
    NEWHLHDF=0
  fi
fi

BAD_INSTALL_DIR=no
if [ -d "${INSTALLDIR}" ]; then
  if [ -d "${INSTALLDIR}/include" ]; then
    PINS_INCL_FILES=`ls -1 "${INSTALLDIR}/include" | grep -v hlhdf`
    if [ "x$PINS_INCL_FILES" != "x" ]; then
      BAD_INSTALL_DIR=yes
    fi
  fi
  
  if [ -d "${INSTALLDIR}/lib" ]; then
    PINS_LIB_FILES=`ls -1 "${INSTALLDIR}/lib" | grep -v hlhdf | grep -v pyhl`
    if [ "x$PINS_LIB_FILES" != "x" ]; then
      BAD_INSTALL_DIR=yes
    fi
  fi
  
  if [ -d "${INSTALLDIR}/bin" ]; then
    PINS_BIN_FILES=`ls -1 "${INSTALLDIR}/bin" | grep -v hllist | grep -v hldec | grep -v hlenc | grep -v hlinstall`
    if [ "x$PINS_BIN_FILES" != "x" ]; then
      BAD_INSTALL_DIR=yes
    fi
  fi
elif [ -e "${INSTALLDIR}" ]; then
  echo "Install directory ${INSTALLDIR} already exists as a non-directory"
  exit 255
fi

if [ "x$BAD_INSTALL_DIR" = "xyes" ]; then
  echo "It is not allowed to install hlhdf in a directory that is shared between other components"
  echo ""
  exit 255
fi

if [ $DIREXISTS -eq 1 -a $NEWHLHDF -eq 0 ]; then
  echo "Converting old HLHDF installation to new version"
  create_directory "$INSTALLDIR/pre-0.7" || exit 255
  replace_path_with_symbolic_link "$INSTALLDIR/bin" "$INSTALLDIR/pre-0.7/bin" || exit 255
  replace_path_with_symbolic_link "$INSTALLDIR/include" "$INSTALLDIR/pre-0.7/include" || exit 255
  replace_path_with_symbolic_link "$INSTALLDIR/lib" "$INSTALLDIR/pre-0.7/lib" || exit 255
  replace_path_with_symbolic_link "$INSTALLDIR/mkf" "$INSTALLDIR/pre-0.7/mkf" || exit 255
  replace_path_with_symbolic_link "$INSTALLDIR/hlhdf.pth" "$INSTALLDIR/pre-0.7/hlhdf.pth" || exit 255
  cat << EOF > "$INSTALLDIR/.version"
pre-0.7
EOF
  if [ $? -ne 0 ]; then
    echo "Failed to create $INSTALLDIR/.version, aborting upgrade"
    exit 255
  fi
  echo "Directory structure converted successfully."
fi

if [ $DIREXISTS -eq 0 ]; then
  echo "Install directory does not exist, creating base"
  create_directory "$INSTALLDIR" || exit 255
  echo "Install directory created successfully"
else
  echo "Install directory exists, will save rollback information"
  move_path "$INSTALLDIR/.version" "$INSTALLDIR/.rollback" || exit 255
  remove_path "$INSTALLDIR/bin" || exit 255
  remove_path "$INSTALLDIR/include" || exit 255
  remove_path "$INSTALLDIR/lib" || exit 255
  remove_path "$INSTALLDIR/mkf" || exit 255
  remove_path "$INSTALLDIR/hlhdf.pth" || exit 255
fi

echo "Setting up the directory structure"
create_directory "$INSTALLDIR/$VERSION" || exit 255
create_directory "$INSTALLDIR/$VERSION/bin" || exit 255
create_directory "$INSTALLDIR/$VERSION/include" || exit 255
create_directory "$INSTALLDIR/$VERSION/lib" || exit 255
create_directory "$INSTALLDIR/$VERSION/mkf" || exit 255
create_symbolic_link "$INSTALLDIR/bin" "$INSTALLDIR/$VERSION/bin" || exit 255
create_symbolic_link "$INSTALLDIR/include" "$INSTALLDIR/$VERSION/include" || exit 255
create_symbolic_link "$INSTALLDIR/lib" "$INSTALLDIR/$VERSION/lib" || exit 255
create_symbolic_link "$INSTALLDIR/mkf" "$INSTALLDIR/$VERSION/mkf" || exit 255

echo "$VERSION" > "$INSTALLDIR/.version"
if [ $? -ne 0 ]; then
  echo "Failed to create $INSTALLDIR/.version with version $VERSION"
  exit 255
else
  exit 0
fi