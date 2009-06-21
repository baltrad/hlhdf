#!/bin/sh
############################################################
# Description: Script that generates the documentation.
#
# Author(s):   Anders Henja
#
# Copyright:   Swedish Meteorological and Hydrological Institute, 2009
#
# History:  2009-06-17 Created by Anders Henja
############################################################

SCRIPTPATH=`dirname $(readlink -f $0)`

LASTDIR=`pwd`
cd $SCRIPTPATH/../doxygen
doxygen hlhdf_doxygen.cfg
cd $LASTDIR
