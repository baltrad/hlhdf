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

## @package examplee
# @file ReadSimpleFile.py
#
# Example that reads a simple hdf5 file
import _pyhl

## Reads the file
def readFile():
  aList = _pyhl.read_nodelist("simple_test.hdf")

  # Select individual nodes, instead of all of them
  aList.selectNode("/info/xscale")
  aList.selectNode("/info/yscale")
  aList.selectNode("/data")

  # Fetch the data for selected nodes
  aList.fetch()

  # Print the data
  aNode = aList.getNode("/info/xscale")
  print "XSCALE=" + `aNode.data()`
  aNode = aList.getNode("/info/yscale")
  print "YSCALE=" + `aNode.data()`
  aNode = aList.getNode("/data")
  print "DATA=" + `aNode.data()`

if __name__ == "__main__":
  readFile()