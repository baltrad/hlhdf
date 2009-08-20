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
# Performs necessary rollbacks if an installation has failed.
#
# @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
# @date 2009-08-19
###########################################################################

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

if [ $# -ne 1 ]; then
  echo "Usage rollback_installation <directory>"
  exit 255
fi
INSTALLDIR=$1

if [ -f "$INSTALLDIR/.rollback" ]; then
  echo "Atempting to rollback the installation"
  VERSION=`cat "$INSTALLDIR/.rollback"`

  remove_path "$INSTALLDIR/.version" || exit 255
  remove_path "$INSTALLDIR/bin" || exit 255
  remove_path "$INSTALLDIR/include" || exit 255
  remove_path "$INSTALLDIR/lib" || exit 255
  remove_path "$INSTALLDIR/mkf" || exit 255
  remove_path "$INSTALLDIR/hlhdf.pth" || exit 255
  
  create_symbolic_link "$INSTALLDIR/bin" "$INSTALLDIR/$VERSION/bin" || exit 255
  create_symbolic_link "$INSTALLDIR/include" "$INSTALLDIR/$VERSION/include" || exit 255
  create_symbolic_link "$INSTALLDIR/lib" "$INSTALLDIR/$VERSION/lib" || exit 255
  create_symbolic_link "$INSTALLDIR/mkf" "$INSTALLDIR/$VERSION/mkf" || exit 255
  create_symbolic_link "$INSTALLDIR/hlhdf.pth" "$INSTALLDIR/$VERSION/hlhdf.pth" || exit 255
  
  \mv "$INSTALLDIR/.rollback" "$INSTALLDIR/.version"
  if [ $? -ne 0 ]; then
    echo "Could not change $INSTALLDIR/.version"
    exit 255
  fi
  echo "System rolled back to version $VERSION"
fi

exit 0