## @package examplee
# @file HDF5ImageWithReference.py
#
# Example that creates a HDF5 image with a reference to a pallette.

import _pyhl
from numpy import *

# Function for creating a dummy palette
def createPalette():
  a = zeros((256, 3), 'b')
  for i in range(0, 256):
    a[i][0] = i
  return a


# Function for creating a dummy image}
def createImage():
  a = zeros((256, 256), 'b')
  for i in range(0, 256):
    for j in range(0, 256):
      a[i][j] = i
  return a

# Function for the HDF5 file
def create_test_image():
  a=_pyhl.nodelist()

  # First create the palette}
  b=_pyhl.node(_pyhl.DATASET_ID,"/PALETTE")
  c=createPalette()
  b.setArrayValue(-1,[256,3],c,"uchar",-1)
  a.addNode(b)
  b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/PALETTE/CLASS")
  b.setScalarValue(-1,"PALETTE","string",-1)
  a.addNode(b)
  b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/PALETTE/PAL_VERSION")
  b.setScalarValue(-1,"1.2","string",-1)
  a.addNode(b)
  b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/PALETTE/PAL_COLORMODEL")
  b.setScalarValue(-1,"RGB","string",-1)
  a.addNode(b)
  b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/PALETTE/PAL_TYPE")
  b.setScalarValue(-1,"STANDARD8","string",-1)
  a.addNode(b)

  # Now create the image to display}
  b=_pyhl.node(_pyhl.DATASET_ID,"/IMAGE1")
  c=createImage()
  b.setArrayValue(1,[256,256],c,"uchar",-1)
  a.addNode(b)
  b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/IMAGE1/CLASS")
  b.setScalarValue(-1,"IMAGE","string",-1)
  a.addNode(b)
  b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/IMAGE1/IMAGE_VERSION")
  b.setScalarValue(-1,"1.2","string",-1)
  a.addNode(b)

  # Finally insert the reference}
  b=_pyhl.node(_pyhl.REFERENCE_ID,"/IMAGE1/PALETTE")
  b.setScalarValue(-1,"/PALETTE","string",-1)
  a.addNode(b)

  a.write("test_image.hdf")

# The main function}
if __name__ == "__main__":
  create_test_image()
