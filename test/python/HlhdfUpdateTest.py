'''
Created on Jun 21, 2009

@author: anders
'''
import unittest
import _pyhl
import _rave_info_type
import numpy
import os

class HlhdfUpdateTest(unittest.TestCase):
  TESTFILE = "testskrivning.hdf"
  
  def setUp(self):
    #_pyhl.show_hlhdferrors(1)
    #_pyhl.show_hdf5errors(1)
    if os.path.isfile(self.TESTFILE):
      os.unlink(self.TESTFILE)
    a = _pyhl.nodelist()
    b = _pyhl.node(_pyhl.GROUP_ID, "/root")
    a.addNode(b)
    a.write(self.TESTFILE)
    
  def tearDown(self):
    if os.path.isfile(self.TESTFILE):
      os.unlink(self.TESTFILE)

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

  def testUpdateAttribute(self):
    a = _pyhl.read_nodelist(self.TESTFILE)
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/intvalue", -1, -123, "int", -1)
    self.addArrayValueNode(a, _pyhl.ATTRIBUTE_ID, "/intarray", -1, [4], [1,2,3,4], "int", -1)
    a.update()
    
    # Verify
    nl = _pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/intvalue")
    self.assertEqual("int", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertEqual(-123, b.data())

    b=a.fetchNode("/intarray")
    self.assertEqual("int", b.format())
    self.assertEqual(_pyhl.ATTRIBUTE_ID, b.type())
    self.assertTrue(numpy.all([1,2,3,4] == b.data()))

  def testUpdateDataset(self):
    a = _pyhl.read_nodelist(self.TESTFILE)
    c=numpy.arange(100)
    c=numpy.array(c.astype(numpy.int32),numpy.int32)
    c=numpy.reshape(c,(10,10)).astype(numpy.int32)    
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/intdataset", -1, numpy.shape(c), c, "int", -1)
    a.update()
    
    # Verify
    nl = _pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/intdataset")
    self.assertEqual("int", b.format())
    self.assertEqual(_pyhl.DATASET_ID, b.type())
    self.assertTrue(numpy.all(c == b.data()))

  def testUpdateGroup(self):
    a = _pyhl.read_nodelist(self.TESTFILE)
    self.addGroupNode(a, "/root/group1")
    self.addGroupNode(a, "/group2")

    a.update()
    
    # Verify
    nl = _pyhl.read_nodelist(self.TESTFILE)
    b = nl.fetchNode("/root/group1")
    self.assertEquals("/root/group1", b.name())
    self.assertEquals(_pyhl.GROUP_ID, b.type())

    b = nl.fetchNode("/group2")
    self.assertEquals("/group2", b.name())
    self.assertEquals(_pyhl.GROUP_ID, b.type())

  def testUpdateReferences(self):
    a = _pyhl.read_nodelist(self.TESTFILE)
    self.addArrayValueNode(a, _pyhl.DATASET_ID, "/doublearray", -1, [4], [1.1,2.2,3.3,4.4], "double", -1)
    self.addReference(a, "/reference", "/doublearray")
    self.addReference(a, "/reference2", "/root")
    a.update()    
    
    #verify
    a=_pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/reference")
    self.assertEqual("string", b.format())
    self.assertEqual("/doublearray", b.data())
    self.assertEqual(_pyhl.REFERENCE_ID, b.type())

    b=a.fetchNode("/reference2")
    self.assertEqual("string", b.format())
    self.assertEqual("/root", b.data())
    self.assertEqual(_pyhl.REFERENCE_ID, b.type())

  def testUpdateUnnamedCompoundAttribute(self):
    a = _pyhl.read_nodelist(self.TESTFILE)
    rinfo_obj =_rave_info_type.object()
    rinfo_type=_rave_info_type.type()    
    rinfo_obj.xsize = 98
    rinfo_obj.ysize = 97
    rinfo_obj.xscale = 120.0
    rinfo_obj.yscale = 130.0
    rinfo_obj.area_extent = (33.0,32.0,31.0,30.0)
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/attribute", rinfo_type.size(), rinfo_obj.tostring(), "compound", rinfo_type.hid())
    a.update()    
    
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
    
  def testUpdateNamedCompoundAttribute(self):
    a = _pyhl.read_nodelist(self.TESTFILE)
    rinfo_obj =_rave_info_type.object()
    rinfo_type=_rave_info_type.type()    
    rinfo_obj.xsize = 98
    rinfo_obj.ysize = 97
    rinfo_obj.xscale = 120.0
    rinfo_obj.yscale = 130.0
    rinfo_obj.area_extent = (33.0,32.0,31.0,30.0)
    self.addTypeNode(a, "/RaveType", rinfo_type.hid())
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/attribute", rinfo_type.size(), rinfo_obj.tostring(), "compound", rinfo_type.hid())
    a.update()    
    
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

  # This test does not work yet. I have added an issue on this matter
  # The reason is that when reading types we keep the hdfid in memory which means
  # that the file not will be closed...
  def XtestUpdateNamedCompoundAttribute_2(self):
    #a = _pyhl.read_nodelist(self.TESTFILE)

    # Name the type
    rinfo_type=_rave_info_type.type()    
    #self.addTypeNode(a, "/RaveType", rinfo_type.hid())
    #a.update()    
    #del a
    
    # Verify that the type has been committed
    a = _pyhl.read_nodelist(self.TESTFILE)
    b=a.fetchNode("/RaveType")
    #self.assertEqual("UNDEFINED", b.format())
    #self.assertEqual(_pyhl.TYPE_ID, b.type())
    #print "DATA: " + `b.data()`
    # Add a value using the named type
    rinfo_obj =_rave_info_type.object()
    rinfo_obj.xsize = 98
    rinfo_obj.ysize = 97
    rinfo_obj.xscale = 120.0
    rinfo_obj.yscale = 130.0
    rinfo_obj.area_extent = (33.0,32.0,31.0,30.0)
    self.addScalarValueNode(a, _pyhl.ATTRIBUTE_ID, "/attribute", rinfo_type.size(), rinfo_obj.tostring(), "compound", rinfo_type.hid())
    a.update() 
    
    #verify that the data has been updated
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
    
if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testUpdateAttribute']
    unittest.main()