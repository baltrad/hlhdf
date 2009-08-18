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

'''
Created on Jun 4, 2009

@author: anders
'''
import unittest
import _pyhl
import numpy
import os
import _varioustests
import _rave_info_type

class HlhdfWriteTest(unittest.TestCase):
  TESTFILE = "testskrivning.hdf"
  TESTFILE2 = "testskrivning2.hdf"
  
  def setUp(self):
    _pyhl.show_hlhdferrors(0)
    _pyhl.show_hdf5errors(0)
    if os.path.isfile(self.TESTFILE):
      os.unlink(self.TESTFILE)
    if os.path.isfile(self.TESTFILE2):
      os.unlink(self.TESTFILE2)   

  def tearDown(self):
    if os.path.isfile(self.TESTFILE):
      os.unlink(self.TESTFILE)
    if os.path.isfile(self.TESTFILE2):
      os.unlink(self.TESTFILE2)   
  
  def addScalarValueNode(self, nodelist, type, name, sz, value, hltype, hid):
    b = _pyhl.node(type, name)
    b.setScalarValue(sz, value, hltype, hid)
    nodelist.addNode(b)
    return b
  
  def addArrayValueNode(self, nodelist, type, name, sz, dims, value, hltype, hid):
    b = _pyhl.node(type, name)
    b.setArrayValue(sz, dims, value, hltype, hid)
    nodelist.addNode(b)
    return b
 
  def addGroupNode(self, nodelist, name):
    b=_pyhl.node(_pyhl.GROUP_ID, name)
    nodelist.addNode(b)
    return b
  
  def addReference(self, nodelist, name, ref):
    b=_pyhl.node(_pyhl.REFERENCE_ID, name)
    b.setScalarValue(-1,ref,"string",-1)
    nodelist.addNode(b)
    return b
  
  def addTypeNode(self, nodelist, name, hid):
    b=_pyhl.node(_pyhl.TYPE_ID, name)
    b.commit(hid)
    nodelist.addNode(b)
    return b
  
  def testWriteString(self):
    #execute
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/stringvalue", -1, "My String", "string", -1)
    a.write(self.TESTFILE)
 
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/stringvalue")
    self.assertEqual("string", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual("My String", b.data())
    
  def testWriteStringArray(self):
    a=_pyhl.nodelist()
    self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/stringarray", -1, [4], ["abc", "def", "ghi", "jkl"], "string", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/stringarray")
    self.assertEqual("string", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertTrue(numpy.all(["abc", "def", "ghi", "jkl"] == b.data()))

  def testWriteStringDataset(self):
    a=_pyhl.nodelist()
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/stringvalue", -1, [4], ["abc", "def", "ghi", "jkl"], "string", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/stringvalue")
    self.assertEqual("string", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(["abc", "def", "ghi", "jkl"] == b.data()))
  
  def testWriteChar(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/charvalue", -1, 123, "char", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/charvalue")
    self.assertEqual("schar", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(123, b.data())

  def testWriteCharacterDataset(self):
    # Characters are handled as schar or uchar. Should we keep it like this?
    pass
  
  def testWriteUchar(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/ucharvalue", -1, 123, "uchar", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/ucharvalue")
    self.assertEqual("uchar", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(123, b.data())

  def testWriteUcharArray(self):
    a=_pyhl.nodelist()
    try:
      self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/uchararray", -1, [4], [1,2,3,4], "uchar", -1)
      self.fail("Expected TypeError")
    except TypeError, e:
      pass

  def testWriteUcharDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.uint8),numpy.uint8)
    c=numpy.reshape(c,(10,10)).astype(numpy.uint8)    
    self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/uchardataset", -1, numpy.shape(c), c, "uchar", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/uchardataset")
    self.assertEqual("uchar", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteSchar(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/scharvalue", -1, -123, "schar", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/scharvalue")
    self.assertEqual("schar", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(-123, b.data())

  def testWriteScharArray(self):
    a=_pyhl.nodelist()
    try:
      self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/schararray", -1, [4], [1,2,3,4], "schar", -1)
      self.fail("Expected TypeError")
    except TypeError, e:
      pass

  def testWriteScharDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.int8),numpy.int8)
    c=numpy.reshape(c,(10,10)).astype(numpy.int8)    
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/schardataset", -1, numpy.shape(c), c, "schar", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/schardataset")
    self.assertEqual("schar", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteUshort(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/ushortvalue", -1, 123, "ushort", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/ushortvalue")
    self.assertEqual("ushort", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(123, b.data())

  def testWriteUshortArray(self):
    a=_pyhl.nodelist()
    try:
      self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/ushortarray", -1, [4], [1,2,3,4], "ushort", -1)
      self.fail("Expected TypeError")
    except TypeError, e:
      pass

  def testWriteUshortDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.uint16),numpy.uint16)
    c=numpy.reshape(c,(10,10)).astype(numpy.uint16)    
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/ushortdataset", -1, numpy.shape(c), c, "ushort", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/ushortdataset")
    self.assertEqual("ushort", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteShort(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/shortvalue", -1, -123, "short", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/shortvalue")
    self.assertEqual("short", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(-123, b.data())

  def testWriteShortArray(self):
    a=_pyhl.nodelist()
    try:
      self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/shortarray", -1, [4], [1,2,3,4], "short", -1)
      self.fail("Expected TypeError")
    except TypeError, e:
      pass

  def testWriteShortDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.uint16),numpy.uint16)
    c=numpy.reshape(c,(10,10)).astype(numpy.uint16)    
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/shortdataset", -1, numpy.shape(c), c, "short", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/shortdataset")
    self.assertEqual("short", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteUint(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/uintvalue", -1, 123, "uint", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/uintvalue")
    self.assertEqual("uint", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(123, b.data())

  def testWriteUintArray(self):
    a=_pyhl.nodelist()
    try:
      self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/uintarray", -1, [4], [1,2,3,4], "uint", -1)
      self.fail("Expected TypeError")
    except TypeError, e:
      pass    

  def testWriteUintDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.uint32),numpy.uint32)
    c=numpy.reshape(c,(10,10)).astype(numpy.uint32)    
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/uintdataset", -1, numpy.shape(c), c, "uint", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/uintdataset")
    self.assertEqual("uint", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteInt(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/intvalue", -1, -123, "int", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/intvalue")
    self.assertEqual("int", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(-123, b.data())

  def testWriteIntArray(self):
    a=_pyhl.nodelist()
    self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/intarray", -1, [4], [1,2,3,4], "int", -1)
    a.write(self.TESTFILE)
    
    # verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/intarray")
    self.assertEqual("int", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertTrue(numpy.all([1,2,3,4] == b.data()))
    
  def testWriteIntDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.int32),numpy.int32)
    c=numpy.reshape(c,(10,10)).astype(numpy.int32)    
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/intdataset", -1, numpy.shape(c), c, "int", -1)
    a.write(self.TESTFILE)

    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/intdataset")
    self.assertEqual("int", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteUlong(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/ulongvalue", -1, 987654, "ulong", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/ulongvalue")
    self.assertEqual("ulong", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(987654, b.data())
  
  def testWriteUlongArray(self):
    a=_pyhl.nodelist()
    a=_pyhl.nodelist()
    try:
      self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/ulongarray", -1, [4], [1,2,3,4], "ulong", -1)
      self.fail("Expected TypeError")
    except TypeError, e:
      pass       

  def testWriteUlongDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.uint64),numpy.uint64)
    c=numpy.reshape(c,(10,10)).astype(numpy.uint64)
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/ulongdataset", -1, numpy.shape(c), c, "ulong", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/ulongdataset")
    self.assertEqual("ulong", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

    
  def testWriteLong(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/longvalue", -1, 987654, "long", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/longvalue")
    self.assertEqual("long", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(987654, b.data())
  
  def testWriteLongArray(self):
    a=_pyhl.nodelist()
    self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/longarray", -1, [4], [1,2,3,4], "long", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/longarray")
    self.assertEqual("long", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertTrue(numpy.all([1,2,3,4] == b.data()))

  def testWriteLongDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype('l'),'l')
    c=numpy.reshape(c,(10,10)).astype('l')
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/longdataset", -1, numpy.shape(c), c, "long", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/longdataset")
    self.assertEqual("long", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))
    
  def testWriteLongLong(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/llongvalue", -1, 987654, "llong", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/llongvalue")
    if (_varioustests.sizeoflong() < _varioustests.sizeoflonglong()):
      self.assertEqual("llong", b.format())
    else:
      self.assertEqual("long", b.format())
      
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(987654, b.data())
  
  def testWriteLongLongArray(self):
    a=_pyhl.nodelist()
    try:
      self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/llongarray", -1, [4], [1,2,3,4], "llong", -1)
      self.fail("Expected TypeError")
    except TypeError, e:
      pass
    
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    try:
      a.fetchNode("/llongarray")
      self.fail("Expected IOError")
    except IOError, e:
      pass

  def testWriteLongLongDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.int64),numpy.int64)
    c=numpy.reshape(c,(10,10)).astype(numpy.int64)
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/llongdataset", -1, numpy.shape(c), c, "llong", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/llongdataset")
    if (_varioustests.sizeoflong() < _varioustests.sizeoflonglong()):
      self.assertEqual("llong", b.format())
    else:
      self.assertEqual("long", b.format())    
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteFloat(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/floatvalue", -1, 1.123, "float", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/floatvalue")
    self.assertEqual("float", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertAlmostEqual(1.123, b.data(), 4)
  
  def testWriteFloatArray(self):
    a=_pyhl.nodelist()
    self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/floatarray", -1, [4], [1.1,2.2,3.3,4.4], "float", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/floatarray")
    self.assertEqual("float", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    c = b.data()
    self.assertEquals(4, len(c))
    self.assertAlmostEqual(1.1, c[0], 4)
    self.assertAlmostEqual(2.2, c[1], 4)
    self.assertAlmostEqual(3.3, c[2], 4)
    self.assertAlmostEqual(4.4, c[3], 4)

  def testWriteFloatDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.float32),numpy.float32)
    c=numpy.reshape(c,(10,10)).astype(numpy.float32)
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/floatdataset", -1, numpy.shape(c), c, "float", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/floatdataset")
    self.assertEqual("float", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteDouble(self):
    a=_pyhl.nodelist()
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/doublevalue", -1, 1.123, "double", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/doublevalue")
    self.assertEqual("double", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertAlmostEqual(1.123, b.data(), 4)
  
  def testWriteDoubleArray(self):
    a=_pyhl.nodelist()
    self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/doublearray", -1, [4], [1.1,2.2,3.3,4.4], "double", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/doublearray")
    self.assertEqual("double", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertTrue(numpy.all([1.1,2.2,3.3,4.4] == b.data()))

  def testWriteDoubleDataset(self):
    a=_pyhl.nodelist()
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.float64),numpy.float64)
    c=numpy.reshape(c,(10,10)).astype(numpy.float64)
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/doubledataset", -1, numpy.shape(c), c, "double", -1)
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/doubledataset")
    self.assertEqual("double", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testWriteGroup(self):
    a=_pyhl.nodelist()
    self.addGroupNode(a, "/group1")
    self.addGroupNode(a, "/group1/group11")
    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/group1")
    self.assertEqual("UNDEFINED", b.format())
    self.assertEqual(_pyhl.GROUP_ID, b.type())
    b=a.fetchNode("/group1/group11")
    self.assertEqual("UNDEFINED", b.format())
    self.assertEqual(_pyhl.GROUP_ID, b.type())

  def testWriteReference(self):
    a=_pyhl.nodelist()
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/doublearray", -1, [4], [1.1,2.2,3.3,4.4], "double", -1)
    self.addReference(a, "/reference", "/doublearray")
    a.write(self.TESTFILE)    
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/reference")
    self.assertEqual("string", b.format())
    self.assertEqual(_pyhl.REFERENCE_ID, b.type())
    self.assertEqual("/doublearray", b.data())

  def testWriteUnnamedCompoundAttribute(self):
    a=_pyhl.nodelist()
    rinfo_obj =_rave_info_type.object()
    rinfo_type=_rave_info_type.type()    
    rinfo_obj.xsize = 98
    rinfo_obj.ysize = 97
    rinfo_obj.xscale = 120.0
    rinfo_obj.yscale = 130.0
    rinfo_obj.area_extent = (33.0,32.0,31.0,30.0)
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/attribute", rinfo_type.size(), rinfo_obj.tostring(), "compound", rinfo_type.hid())
    a.write(self.TESTFILE)    
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/attribute")
    self.assertEqual("compound", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    result = b.compound_data()
    self.assertEqual(98, result['xsize'])
    self.assertEqual(97, result['ysize'])
    self.assertEqual(120, result['xscale'])
    self.assertEqual(130, result['yscale'])
    self.assertTrue(numpy.all([33.0,32.0,31.0,30.0]==result['area_extent']))
    
  def testWriteNamedCompoundAttribute(self):
    a=_pyhl.nodelist()
    rinfo_obj =_rave_info_type.object()
    rinfo_type=_rave_info_type.type()    
    rinfo_obj.xsize = 98
    rinfo_obj.ysize = 97
    rinfo_obj.xscale = 120.0
    rinfo_obj.yscale = 130.0
    rinfo_obj.area_extent = (33.0,32.0,31.0,30.0)
    self.addTypeNode(a, "/RaveType", rinfo_type.hid())
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/attribute", rinfo_type.size(), rinfo_obj.tostring(), "compound", rinfo_type.hid())
    a.write(self.TESTFILE)    
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/RaveType")
    self.assertEqual("UNDEFINED", b.format())
    self.assertEqual(_pyhl.TYPE_ID, b.type())
    
    b=a.fetchNode("/attribute")
    self.assertEqual("compound", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    result = b.compound_data()
    self.assertEqual(98, result['xsize'])
    self.assertEqual(97, result['ysize'])
    self.assertEqual(120, result['xscale'])
    self.assertEqual(130, result['yscale'])
    self.assertTrue(numpy.all([33.0,32.0,31.0,30.0]==result['area_extent']))
  
  # Bug: longs are written as int32 on MAC.
  def testReadWriteVariousTypes(self):
    a=_pyhl.nodelist()
    
    b=_pyhl.node(_pyhl.GROUP_ID,"/info")
    a.addNode(b)

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/xscale")
    b.setScalarValue(-1,0.85,"double",-1)
    a.addNode(b)

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/yscale")
    b.setScalarValue(-1,1.0,"float",-1)
    a.addNode(b);

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/xsize")
    b.setScalarValue(-1, 240, "int", -1)
    a.addNode(b)

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/info/ysize")
    b.setScalarValue(-1,long(480),"long",-1)
    a.addNode(b)

    a.write(self.TESTFILE)
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    self.assertEquals("double", a.fetchNode("/info/xscale").format())
    self.assertEquals("float", a.fetchNode("/info/yscale").format())
    self.assertEquals("int", a.fetchNode("/info/xsize").format())
    self.assertEquals("long", a.fetchNode("/info/ysize").format())

  def testWriteOnlyRootGroup(self):
    a=_pyhl.nodelist()
    b=_pyhl.node(_pyhl.GROUP_ID, "/")
    a.addNode(b)
    try:
      a.write(self.TESTFILE)
      self.fail("Expected IOError")
    except IOError, e:
      pass
  
  def testReadWriteSameFile(self):
    a=_pyhl.nodelist()
    b=_pyhl.node(_pyhl.GROUP_ID, "/slask")
    a.addNode(b)
    a.write(self.TESTFILE)
    
    a = _pyhl.read_nodelist(self.TESTFILE)
    a.write(self.TESTFILE2)
    
if __name__ == '__main__':
  unittest.main()
