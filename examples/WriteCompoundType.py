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
# @file WriteCompoundType.py
#
# Example that writes a compound datatype.
import _pyhl
import _rave_info_type

## Function that writes the actual file
def writeFile():
  # Create the rave info HDF5 type
  typedef = _rave_info_type.type()

  # Create the rave info HDF5 object
  obj = _rave_info_type.object()

  # Set the values
  obj.xsize=10
  obj.ysize=10
  obj.xscale=150.0
  obj.yscale=150.0

  aList = _pyhl.nodelist()

  # Create a datatype node
  aNode = _pyhl.node(_pyhl.TYPE_ID,"/MyDatatype")

  # Make the datatype named
  aNode.commit(typedef.hid())
  aList.addNode(aNode)

  # Create an attribute containing the compound type
  aNode = _pyhl.node(_pyhl.ATTRIBUTE_ID,"/myCompoundAttribute")
  
  # Note that I use both itemSize and lhid
  # Also note how I translate the compound object to a string
  aNode.setScalarValue(typedef.size(),obj.tostring(),"compound",typedef.hid())
  aList.addNode(aNode)

  # Better create a dataset also with the compound type
  obj.xsize=1
  obj.ysize=1
  aNode = _pyhl.node(_pyhl.DATASET_ID,"/myCompoundDataset")

  # I use setArrayValue instead
  aNode.setArrayValue(typedef.size(),[1],obj.tostring(),"compound",typedef.hid())
  aList.addNode(aNode)

  # And finally write the HDF5 file.
  aList.write("compound_test.hdf")

if __name__== "__main__":
  writeFile()
