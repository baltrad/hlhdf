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
  