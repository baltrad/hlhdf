'''
Created on Jun 5, 2009

@author: anders
'''
import unittest
import _pyhl
import _rave_info_type
import numpy
import os

class HlhdfReadTest(unittest.TestCase):
  TESTFILE = "fixture_VhlhdfRead_datafile.h5"
  
  def setUp(self):
    _pyhl.show_hlhdferrors(0);
    self.h5nodelist = _pyhl.read_nodelist(self.TESTFILE) 

  def tearDown(self):
    self.h5nodelist = None

  def testReadString(self):
    node=self.h5nodelist.fetchNode("/stringvalue")
    self.assertEqual("string", node.format())
    self.assertEqual("My String", node.data())
    self.assertEqual("/stringvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual("My String", node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 
  
  def testReadChar(self):
    node=self.h5nodelist.fetchNode("/charvalue")
    self.assertEqual("schar", node.format()) # We are getting the fixed type format
    self.assertEqual(123, node.data())
    self.assertEqual("/charvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(123, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 
    
  def testReadSchar(self):
    node=self.h5nodelist.fetchNode("/scharvalue")
    self.assertEqual("schar", node.format()) # We are getting the fixed type format
    self.assertEqual(45, node.data())
    self.assertEqual("/scharvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(45, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 
    
  def testReadUchar(self):
    node=self.h5nodelist.fetchNode("/ucharvalue")
    self.assertEqual("uchar", node.format()) # We are getting the fixed type format
    self.assertEqual(99, node.data())
    self.assertEqual("/ucharvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(99, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadShort(self):
    node=self.h5nodelist.fetchNode("/shortvalue")
    self.assertEqual("short", node.format()) # We are getting the fixed type format
    self.assertEqual(-31, node.data())
    self.assertEqual("/shortvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(-31, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 
 
  def testReadUshort(self):
    node=self.h5nodelist.fetchNode("/ushortvalue")
    self.assertEqual("ushort", node.format()) # We are getting the fixed type format
    self.assertEqual(15, node.data())
    self.assertEqual("/ushortvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(15, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadInt(self):
    node=self.h5nodelist.fetchNode("/intvalue")
    self.assertEqual("int", node.format()) # We are getting the fixed type format
    self.assertEqual(989898, node.data())
    self.assertEqual("/intvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(989898, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadUint(self):
    node=self.h5nodelist.fetchNode("/uintvalue")
    self.assertEqual("uint", node.format()) # We are getting the fixed type format
    self.assertEqual(987654, node.data())
    self.assertEqual("/uintvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(987654, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadLong(self):
    node=self.h5nodelist.fetchNode("/longvalue")
    self.assertEqual("long", node.format()) # We are getting the fixed type format
    self.assertEqual(-123456789, node.data())
    self.assertEqual("/longvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(-123456789, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadUlong(self):
    node=self.h5nodelist.fetchNode("/ulongvalue")
    self.assertEqual("ulong", node.format()) # We are getting the fixed type format
    self.assertEqual(123456789, node.data())
    self.assertEqual("/ulongvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(123456789, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadLlong(self):
    node=self.h5nodelist.fetchNode("/llongvalue")
    self.assertEqual("long", node.format()) # We are getting the fixed type format
    self.assertEqual(-123456789012, node.data())
    self.assertEqual("/llongvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(-123456789012, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadUllong(self):
    node=self.h5nodelist.fetchNode("/ullongvalue")
    self.assertEqual("ulong", node.format()) # We are getting the fixed type format
    self.assertEqual(123456789012, node.data())
    self.assertEqual("/ullongvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(123456789012, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadFloat(self):
    node=self.h5nodelist.fetchNode("/floatvalue")
    self.assertEqual("float", node.format()) # We are getting the fixed type format
    self.assertAlmostEqual(12.65, node.data(),4)
    self.assertEqual("/floatvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertAlmostEqual(12.65, node.rawdata(),4)
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadDouble(self):
    node=self.h5nodelist.fetchNode("/doublevalue")
    self.assertEqual("double", node.format()) # We are getting the fixed type format
    self.assertEqual(12999.8989, node.data())
    self.assertEqual("/doublevalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(12999.8989, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 
    
  def testReadHsize(self):
    node=self.h5nodelist.fetchNode("/hsizevalue")
    self.assertEqual("ulong", node.format()) # We are getting the fixed type format
    self.assertEqual(65765, node.data())
    self.assertEqual("/hsizevalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(65765, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 
    
  def testReadHerr(self):
    node=self.h5nodelist.fetchNode("/herrvalue")
    self.assertEqual("int", node.format()) # We are getting the fixed type format
    self.assertEqual(12, node.data())
    self.assertEqual("/herrvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(12, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 
    
  def testReadHbool(self):
    node=self.h5nodelist.fetchNode("/hboolvalue")
    self.assertEqual("int", node.format()) # We are getting the fixed type format
    self.assertEqual(0, node.data())
    self.assertEqual("/hboolvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(0, node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadRootReferenceToLongarray(self):
    node=self.h5nodelist.fetchNode("/rootreferencetolongarray")
    self.assertEqual("string", node.format()) # We are getting the fixed type format
    self.assertEqual("/longarray", node.data())
    self.assertEqual("/rootreferencetolongarray", node.name())
    self.assertEqual(_pyhl.REFERENCE_ID, node.type())
    self.assertEqual("/longarray", node.rawdata())
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass 

  def testReadRaveDatatype(self):
    node=self.h5nodelist.fetchNode("/RaveDatatype")
    self.assertEqual("UNDEFINED", node.format()) # We are getting the fixed type format
    try:
      x=node.data()
      self.fail("expected exception")
    except AttributeError, e:
      pass
    self.assertEqual("/RaveDatatype", node.name())
    self.assertEqual(_pyhl.TYPE_ID, node.type())
    try:
      x=node.rawdata()
      self.fail("expected exception")
    except AttributeError, e:
      pass
    
    try:
      x = node.compound_data()
      self.fail("Expected exception")
    except AttributeError, e:
      pass

  def testReadDoubleArray(self):
    node=self.h5nodelist.fetchNode("/doublearray")
    self.assertEqual("double", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(3, len(arr))
    self.assertEqual(1.0, arr[0])
    self.assertEqual(2.1, arr[1])
    self.assertEqual(3.2, arr[2])
    self.assertEqual("/doublearray", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    
  def testReadFloatArray(self):
    node=self.h5nodelist.fetchNode("/floatarray")
    self.assertEqual("float", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(3, len(arr))
    self.assertAlmostEqual(1.1, arr[0], 4)
    self.assertAlmostEqual(2.2, arr[1], 4)
    self.assertAlmostEqual(3.3, arr[2], 4)
    self.assertEqual("/floatarray", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass

  def verifyCharacterData(self, size, arr):
    a=numpy.zeros((size[0],size[1]),numpy.int8)
    ll = ['a','b','c','d','e','f','0','1','2','3','4','5','6','A','B','C','D','E']
    ctr=0
    for x in range(0, size[0]):
      for y in range(0,size[1]):
        a[y][x] = ord(ll[ctr])
        ctr += 1
        if ctr >= len(ll):
          ctr = 0
    if not numpy.all(a==arr):
      self.fail("A and ARR not equal")

  def verifyDataset(self, size, arr, numtype):
      a=numpy.zeros((size[0],size[1]),numtype)
      ctr=0
      for x in range(0, size[0]):
        for y in range(0,size[1]):
          a[y][x] = ctr
          ctr += 1
          if ctr >= 100:
            ctr = 0
      if not numpy.all(a==arr):
        self.fail("A and ARR not equal")
    
  def testReadCharDataset(self):
    node=self.h5nodelist.fetchNode("/group1/chardset")
    self.assertEqual("schar", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyCharacterData([5,5], arr)
    self.assertEqual("/group1/chardset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass    

  def testReadDoubleDataset(self):
    node=self.h5nodelist.fetchNode("/group1/doubledset")
    self.assertEqual("double", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.float64)
    self.assertEqual("/group1/doubledset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass    

  def testReadFloatDataset(self):
    node=self.h5nodelist.fetchNode("/group1/floatdset")
    self.assertEqual("float", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.float32)
    self.assertEqual("/group1/floatdset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadGroupInGroup(self):
    node=self.h5nodelist.fetchNode("/group1/group11")
    self.assertEqual("UNDEFINED", node.format())
    self.assertEqual("/group1/group11",node.name())
    self.assertEqual(_pyhl.GROUP_ID, node.type())
    try:
      self.data()
      self.fail("Expected AttributeError")
    except:
      pass
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadIntDataset(self):
    node=self.h5nodelist.fetchNode("/group1/intdset")
    self.assertEqual("int", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.int32)
    self.assertEqual("/group1/intdset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadLongDataset(self):
    node=self.h5nodelist.fetchNode("/group1/longdset")
    self.assertEqual("long", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.int64)
    self.assertEqual("/group1/longdset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 
    
  def testReadScharDataset(self):
    node=self.h5nodelist.fetchNode("/group1/schardset")
    self.assertEqual("schar", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.int64)
    self.assertEqual("/group1/schardset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    
  def testReadShortDataset(self):
    node=self.h5nodelist.fetchNode("/group1/shortdset")
    self.assertEqual("short", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.int64)
    self.assertEqual("/group1/shortdset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    
  def testReadUcharDataset(self):
    node=self.h5nodelist.fetchNode("/group1/uchardset")
    self.assertEqual("uchar", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.int64)
    self.assertEqual("/group1/uchardset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass     

  def testReadUintDataset(self):
    node=self.h5nodelist.fetchNode("/group1/uintdset")
    self.assertEqual("uint", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.int64)
    self.assertEqual("/group1/uintdset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadUshortDataset(self):
    node=self.h5nodelist.fetchNode("/group1/ushortdset")
    self.assertEqual("ushort", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertEqual(2,len(numpy.shape(arr)))
    self.assertEqual(5,numpy.shape(arr)[0])
    self.assertEqual(5,numpy.shape(arr)[1])
    self.verifyDataset([5,5], arr, numpy.int64)
    self.assertEqual("/group1/ushortdset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadIntarray(self):
    node=self.h5nodelist.fetchNode("/intarray")
    self.assertEqual("int", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertTrue(numpy.all([1,2,3,4]==arr))
    self.assertEqual("/intarray", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadLongarray(self):
    node=self.h5nodelist.fetchNode("/longarray")
    self.assertEqual("long", node.format()) # We are getting the fixed type format
    arr = node.data()
    self.assertTrue(numpy.all([2,3,4,5,6]==arr))
    self.assertEqual("/longarray", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      a=node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadReferenceToDoubleArray(self):
    node=self.h5nodelist.fetchNode("/references/doublearray")
    self.assertEqual("string", node.format()) # We are getting the fixed type format
    self.assertEqual("/doublearray", node.data())
    self.assertEqual("/references/doublearray", node.name())
    self.assertEqual(_pyhl.REFERENCE_ID, node.type())
    self.assertEqual("/doublearray", node.rawdata())
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadReferenceToFloatDataset(self):
    node=self.h5nodelist.fetchNode("/references/floatdset")
    self.assertEqual("string", node.format()) # We are getting the fixed type format
    self.assertEqual("/group1/floatdset", node.data())
    self.assertEqual("/references/floatdset", node.name())
    self.assertEqual(_pyhl.REFERENCE_ID, node.type())
    self.assertEqual("/group1/floatdset", node.rawdata())
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 

  def testReadReferenceToGroup(self):
    node=self.h5nodelist.fetchNode("/references/group1")
    self.assertEqual("string", node.format()) # We are getting the fixed type format
    self.assertEqual("/group1", node.data())
    self.assertEqual("/references/group1", node.name())
    self.assertEqual(_pyhl.REFERENCE_ID, node.type())
    self.assertEqual("/group1", node.rawdata())
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass 
    
  def testReadStringArray(self):
    node=self.h5nodelist.fetchNode("/stringarray")
    self.assertEqual("string", node.format()) # We are getting the fixed type format
    self.assertTrue(numpy.all(["ABC","def","EFG"]==node.data()))
    self.assertEqual("/stringarray", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    try:
      arr = node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    try:
      a=node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass

  def testReadDatatype(self):
    node=self.h5nodelist.fetchNode("/RaveDatatype")
    self.assertEqual("UNDEFINED", node.format())
    self.assertEqual(_pyhl.TYPE_ID, node.type())
    try:
      node.data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass

    try:
      node.rawdata()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    
    try:
      node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
     
  def testReadCompoundAttribute_byType(self):
    rinfo_type=_rave_info_type.type()
    rinfo_obj=_rave_info_type.object()
    node=self.h5nodelist.fetchNode("/compoundgroup/attribute")
    self.assertEqual("compound", node.format())
    self.assertEqual("/compoundgroup/attribute", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    rinfo_obj.fromstring(node.rawdata())
    self.assertEqual(10, rinfo_obj.xsize)
    self.assertEqual(10, rinfo_obj.ysize)
    self.assertEqual(150.0, rinfo_obj.xscale)
    self.assertEqual(150.0, rinfo_obj.yscale)
    self.assertTrue(numpy.all(rinfo_obj.area_extent == (0.0,0.0,0.0,0.0)))
  
  def testReadCompoundAttribute_byDictionary(self):
    node=self.h5nodelist.fetchNode("/compoundgroup/attribute")
    self.assertEqual("compound", node.format())
    self.assertEqual("/compoundgroup/attribute", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    comp = node.compound_data()
    self.assertEqual(10, comp["xsize"])
    self.assertEqual(10, comp["ysize"])
    self.assertEqual(150.0, comp["xscale"])
    self.assertEqual(150.0, comp["yscale"])
    self.assertTrue(numpy.all(comp["area_extent"] == [0.0,0.0,0.0,0.0]))

  def testReadCompoundAttribute2_byType(self):
    rinfo_type=_rave_info_type.type()
    rinfo_obj=_rave_info_type.object()
    node=self.h5nodelist.fetchNode("/compoundgroup/attribute2")
    self.assertEqual("/compoundgroup/attribute2", node.name())
    self.assertEqual("compound", node.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    rinfo_obj.fromstring(node.rawdata())
    self.assertEqual(99, rinfo_obj.xsize)
    self.assertEqual(109, rinfo_obj.ysize)
    self.assertEqual(150.0, rinfo_obj.xscale)
    self.assertEqual(150.0, rinfo_obj.yscale)
    self.assertTrue(numpy.all(rinfo_obj.area_extent == (10.0,20.0,30.0,40.0)))
  
  def testReadCompoundAttribute2_byDictionary(self):
    node=self.h5nodelist.fetchNode("/compoundgroup/attribute2")
    self.assertEqual("compound", node.format())
    self.assertEqual("/compoundgroup/attribute2", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    comp = node.compound_data()
    self.assertEqual(99, comp["xsize"])
    self.assertEqual(109, comp["ysize"])
    self.assertEqual(150.0, comp["xscale"])
    self.assertEqual(150.0, comp["yscale"])
    self.assertTrue(numpy.all(comp["area_extent"] == [10.0,20.0,30.0,40.0]))

  def testReadCompoundDataset_byDictionary(self):
    node=self.h5nodelist.fetchNode("/compoundgroup/dataset")
    self.assertEqual("compound", node.format())
    self.assertEqual("/compoundgroup/dataset", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    comp = node.compound_data()
    self.assertEqual(99, comp["xsize"])
    self.assertEqual(109, comp["ysize"])
    self.assertEqual(170.0, comp["xscale"])
    self.assertEqual(150.0, comp["yscale"])
    self.assertTrue(numpy.all(comp["area_extent"] == [10.0,20.0,35.0,40.0]))

  def testReadCompoundDataset2_byDictionary(self):
    node=self.h5nodelist.fetchNode("/compoundgroup/dataset2")
    self.assertEqual("compound", node.format())
    self.assertEqual("/compoundgroup/dataset2", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    # Currently we do not support arrays of compound data
    try:
      node.compound_data()
      self.fail("Expected AttributeError")
    except AttributeError, e:
      pass
    
  def testReadUnnamedCompoundAttribute_byDictionary(self):
    node=self.h5nodelist.fetchNode("/compoundgroup/unnamed_type_attribute")
    self.assertEqual("compound", node.format())
    self.assertEqual("/compoundgroup/unnamed_type_attribute", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    comp = node.compound_data()
    self.assertEqual(1, comp["xsize"])
    self.assertEqual(2, comp["ysize"])
    self.assertEqual(10.0, comp["xscale"])
    self.assertEqual(20.0, comp["yscale"])
    self.assertTrue(numpy.all(comp["area_extent"] == [1.0,2.0,3.0,4.0]))
  
  def testSelectAndFetch_1(self):
    self.h5nodelist.selectAll()
    self.h5nodelist.fetch()
    node = self.h5nodelist.getNode("/ucharvalue")
    self.assertEqual("uchar", node.format()) # We are getting the fixed type format
    self.assertEqual(99, node.data())
    self.assertEqual("/ucharvalue", node.name())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, node.type())
    self.assertEqual(99, node.rawdata())

    node = self.h5nodelist.getNode("/doublearray")
    self.assertEqual("double", node.format()) # We are getting the fixed type format
    self.assertTrue(numpy.all([1.0,2.1,3.2]==node.data()))
    self.assertEqual("/doublearray", node.name())
    self.assertEqual(_pyhl.DATASET_ID, node.type())
    
    node = self.h5nodelist.getNode("/rootreferencetolongarray")
    self.assertEqual("string", node.format()) # We are getting the fixed type format
    self.assertEqual("/longarray", node.data())
    self.assertEqual("/rootreferencetolongarray", node.name())
    self.assertEqual(_pyhl.REFERENCE_ID, node.type())
    self.assertEqual("/longarray", node.rawdata()) 

  def testSelectAndFetch_2(self):
    self.h5nodelist.selectNode("/ucharvalue")
    self.h5nodelist.selectNode("/doublearray")
    self.h5nodelist.selectNode("/rootreferencetolongarray")
    self.h5nodelist.fetch()
    
    node = self.h5nodelist.getNode("/ucharvalue")
    self.assertEqual(99, node.data())

    node = self.h5nodelist.getNode("/doublearray")
    self.assertTrue(numpy.all([1.0,2.1,3.2]==node.data()))
    
    node = self.h5nodelist.getNode("/rootreferencetolongarray")
    self.assertEqual("/longarray", node.data())
       
if __name__ == "__main__":
    unittest.main()
