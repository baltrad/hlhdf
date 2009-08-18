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
# @file ReadCompoundTypeRaw.py
#
# Example that reads a compound datatype by using the raw method in pyhl
import _pyhl
import _rave_info_type

def readFile():
  # There is no meaning creating the type
  obj = _rave_info_type.object()
  aList = _pyhl.read_nodelist("compound_test.hdf")

  # Select everything for retrival
  aList.selectAll()
  aList.fetch()
  aNode = aList.getNode("/myCompoundAttribute")

  # Translate from the string representation to object
  obj.fromstring(aNode.rawdata())

  # Display the values
  print "XSIZE="+`obj.xsize`
  print "YSIZE="+`obj.ysize`
  print "XSCALE="+`obj.xscale`
  print "YSCALE="+`obj.yscale`

if __name__ == "__main__":
  readFile()
