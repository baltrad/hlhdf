#!/bin/sh
############################################################
# Description:	Simple install script that should be used when
#		installing the hlhdf distribution.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################

PROG=`basename "$0"`

FORCED_UPDATE=no
THE_GROUP=
THE_USER=
THE_MODE=755
SUPPRESS_MSGS=no
STRIP_BINARIES=no
SAVE_OLD=no
CREATE_NEEDED_DIRS=no
DEBUG=no

PrintHelp()
{
   echo "Usage: $PROG [-hdsSocfC] [-g group] [-u user] [-m mode]"
   echo "       [-v version] [-p patchlvl] file targetdir"
   if [ $# -eq 0 ]; then
      return
   fi
   cat >&2 <<EOF
   [-h]              Prints this help text.
   [-d]              Enables debug information. (default off)
   [-s]              Suppresses all msgs but errors. (default off)
   [-S]              Strips binaries. (default no)
   [-o]              Saves all files that already exists with OLD<filename>.
   [-C]              Creates all directories that are required (default off).
   [-c]              Installs file in targetdir unless a copy already
                     exists in specified directory (default behaviour).
                     The -f will give the opposite behaviour.
   [-f]              Installs file in targetdir even if a copy already
                     exists in specified directory.
   [-g group]        Specifies if the group owner should be someone else
                     but the one installing it, requires root-priviligies.
   [-u user]         Specified if the user owner should be someone else
                     but the one installing it, requires root-priviligies.
   [-m mode]         Specified in what mode the file should be installed.
   file              Name of the file that should be copied.
   targetdir         Name of the directory to install file in.
EOF
}

create_directory()
{
   TARGET_DIR=$1
   RESULT=0
   if [ ! -d "$TARGET_DIR" ]; then
      mkdir -p "$TARGET_DIR"
      RESULT=$?
   fi
   return $RESULT
}

install_the_file()
{
   ITF_S=$1
   ITF_T=$2
   RESULT=0

   echo "Installing $ITF_T"

   \cp "$ITF_S" "$ITF_T"
   RESULT=$?

   if [ $RESULT -ne 0 ]; then
      echo "Failed to install $ITF_T..."
   fi

   return $RESULT
}

change_group_id()
{
   CGI_T=$1
   CGI_G=$2
   RESULT=0
   \chgrp $CGI_G "$CGI_T"
   RESULT=$?

   if [ $RESULT -ne 0 ]; then
      echo "Failed to change group id on file $CGI_T"
   fi

   return $RESULT
}

change_user_id()
{
   CUI_T=$1
   CUI_U=$2
   RESULT=0
   \chown $CUI_U "$CUI_T"
   RESULT=$?
   if [ $RESULT -ne 0 ]; then
      echo "Failed to change user id on file $CUI_T"
   fi
   return $RESULT
}

change_mode()
{
   CM_T=$1
   CM_M=$2
   RESULT=0
   \chmod $CM_M "$CM_T"
   RESULT=$?
   if [ $RESULT -ne 0 ]; then
      echo "Failed to change user mode on file $CM_T"
   fi
   return $RESULT
}

set -- `getopt hdcfCsSog:u:m: $*`
if [ $? != 0 ]; then
   PrintHelp
   exit 1
fi

for i in $*; do
   case $i in
    -h) PrintHelp all; exit 0;;
    -d) DEBUG=yes; shift;;
    -c) FORCED_UPDATE=no;shift;;
    -f) FORCED_UPDATE=yes;shift;;
    -g) THE_GROUP=$2;shift 2;;
    -u) THE_USER=$2;shift 2;;
    -m) THE_MODE=$2;shift 2;;
    -s) SUPPRESS_MSGS=yes; shift;;
    -S) STRIP_BINARIES=yes; shift;;
    -o) SAVE_OLD=yes; shift;;
    -C) CREATE_NEEDED_DIRS=yes; shift;;
    --) shift; break;;
    esac
done

if [ $# -lt 2 ]; then
   PrintHelp
   exit 1
fi

SOURCE_FILE=$1
shift
TARGET_FILE=$*
#echo "TGT: $TARGET_FILE"

#echo "Would install $SOURCE_FILE as $TARGET_FILE"

#Start by making sure the directory exists, if -C has been specified
FIRST_DIR=`dirname "$TARGET_FILE"`
if [ ! -d "$FIRST_DIR" -a "x$CREATE_NEEDED_DIRS" = "xno" ]; then
   echo "No directory called '$FIRST_DIR'"
   exit 1
fi

if [ ! -d "$FIRST_DIR" ]; then
   create_directory "$FIRST_DIR"
   if [ $? -ne 0 ]; then
      echo "Could not create directory structure"
      exit 1
   fi
fi

if [ -f "$TARGET_FILE" -a "x$FORCED_UPDATE" = "xno" ]; then
   echo "$TARGET_FILE already exists, Ignoring update"
elif [ -f "$TARGET_FILE" -a "x$FORCED_UPDATE" = "xyes" ]; then
   THE_DIRNAME=`dirname "$TARGET_FILE"`
   THE_BASENAME=`basename "$TARGET_FILE"`
   if [ "x$SAVE_OLD" = "xyes" ]; then
      echo "$TARGET_FILE renamed to $THE_DIRNAME/OLD$THE_BASENAME"
      \mv "$TARGET_FILE" "$THE_DIRNAME/OLD$THE_BASENAME"
   else
      \rm -f "$TARGET_FILE"
   fi
   install_the_file "$SOURCE_FILE" "$TARGET_FILE"
   if [ $? -ne 0 ]; then
      exit 1
   fi
else
   install_the_file "$SOURCE_FILE" "$TARGET_FILE"
   if [ $? -ne 0 ]; then
      exit 1
   fi
fi

if [ "x$THE_GROUP" != "x" ]; then
   change_group_id "$TARGET_FILE" "$THE_GROUP"
   if [ $? -ne 0 ]; then
      exit 1
   fi
fi

if [ "x$THE_USER" != "x" ]; then
   change_user_id "$TARGET_FILE" "$THE_USER"
   if [ $? -ne 0 ]; then
      exit 1
   fi
fi

change_mode "$TARGET_FILE" "$THE_MODE"
if [ $? -ne 0 ]; then
   exit 1
fi

exit 0
