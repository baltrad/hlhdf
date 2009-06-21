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
