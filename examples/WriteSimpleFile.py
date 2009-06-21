## @package examplee
# @file WriteSimpleFile.py
#
# Example that writes a simple hdf5 file
import _pyhl
from numpy import *

def writeFile():
  # Create an empty node list instance
  aList = _pyhl.nodelist()

  # Create an group called info
  aNode = _pyhl.node(_pyhl.GROUP_ID,"/info")

  # Add the node to the nodelist
  # Remember that the nodelist takes responsibility
  aList.addNode(aNode)

  # Insert the attribute xscale in the group "/info"
  aNode = _pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/xscale")

  # Set the value to a double with value 10.0
  # Note the -1's that has been used since the data not is compaound
  aNode.setScalarValue(-1,10.0,"double",-1)
  aList.addNode(aNode)

  # Similar for yscale,xsize and ysize
  aNode = _pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/yscale")
  aNode.setScalarValue(-1,20.0,"double",-1)
  aList.addNode(aNode)
  aNode = _pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/xsize")
  aNode.setScalarValue(-1,10,"int",-1)
  aList.addNode(aNode)
  aNode = _pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/ysize")
  aNode.setScalarValue(-1,10,"int",-1)
  aList.addNode(aNode)

  # Add a description
  aNode = _pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/description")
  aNode.setScalarValue(-1,"This is a simple example","string",-1)
  aList.addNode(aNode)

  # Add an array of data
  myArray = arange(100)
  myArray = array(myArray.astype('i'),'i')
  myArray = reshape(myArray,(10,10))
  aNode = _pyhl.node(_pyhl.DATASET_ID,"/data")

  # Set the data as an array, note the list with [10,10] which
  # Indicates that it is an array of 10x10 items
  aNode.setArrayValue(-1,[10,10],myArray,"int",-1)
  aList.addNode(aNode)

  # And now just write the file as "simple_test.hdf" with
  # Compression level 9 (highest compression)
  aList.write("simple_test.hdf",9)

if __name__ == "__main__":
  writeFile()