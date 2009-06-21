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