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
# @file ReadCompoundType.py
#
# Example that reads a compound datatype by using the compound_data method in pyhl
import _pyhl

def readFile():
  # There is no meaning creating the type
  aList = _pyhl.read_nodelist("compound_test.hdf")

  # Fetch the node
  aNode = aList.fetchNode("/myCompoundAttribute")

  # Translate from the string representation to object
  cdescr = aNode.compound_data()
  print "XSIZE="+`cdescr["xsize"]`
  print "YSIZE="+`cdescr["ysize"]`
  print "XSCALE="+`cdescr["xscale"]`
  print "YSCALE="+`cdescr["yscale"]`


if __name__ == "__main__":
  readFile()
  