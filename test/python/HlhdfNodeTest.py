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
Created on Jun 22, 2009

@author: anders
'''
import unittest
import _pyhl

class HlhdfNodeTest(unittest.TestCase):
  TESTFILE = "fixture_VhlhdfRead_datafile.h5"
  
  def setUp(self):
    _pyhl.show_hlhdferrors(0)
    _pyhl.show_hdf5errors(0)    
  
  def tearDown(self):
    pass
 
  def testSelectFetch(self):
    nodelist = _pyhl.read_nodelist(self.TESTFILE)
    nodelist.selectAll()
    nodelist.fetch()
    node = nodelist.getNode("/group1/floatdset")
    self.assertEqual("/group1/floatdset", node.name())
  
  def testSelectFetch_from(self):
    nodelist = _pyhl.read_nodelist(self.TESTFILE, "/group1")
    nodelist.selectNode("/group1/floatdset")
    nodelist.fetch()
    node = nodelist.getNode("/group1/floatdset")
    self.assertEqual("/group1/floatdset", node.name())

  def testFetch_from_group(self):
    nodelist = _pyhl.read_nodelist(self.TESTFILE, "/group1")
    node = nodelist.fetchNode("/group1/floatdset")
    self.assertEqual("/group1/floatdset", node.name())

  def testFetch_from_group_2(self):
    nodelist = _pyhl.read_nodelist(self.TESTFILE, "/group1/")
    node = nodelist.fetchNode("/group1/floatdset")
    self.assertEqual("/group1/floatdset", node.name())
    
  def testFetch_from_root(self):
    nodelist = _pyhl.read_nodelist(self.TESTFILE, "/")
    node = nodelist.fetchNode("/group1/floatdset")
    self.assertEqual("/group1/floatdset", node.name())

  def testFetch_from_attribute(self):
    try:
      nodelist = _pyhl.read_nodelist(self.TESTFILE, "/hboolvalue")
      self.fail("Expected IOError")
    except IOError:
      pass

  def testFetch_from_dataset(self):
    nodelist = _pyhl.read_nodelist(self.TESTFILE, "/dataset1")
    node = nodelist.fetchNode("/dataset1/attribute1")
    self.assertEqual("/dataset1/attribute1", node.name()) 
    node = nodelist.fetchNode("/dataset1/doublearray")
    self.assertEqual("/dataset1/doublearray", node.name()) 
       
  def testFetch_from_dataset_2(self):
    nodelist = _pyhl.read_nodelist(self.TESTFILE, "/dataset1/")
    node = nodelist.fetchNode("/dataset1/attribute1")
    self.assertEqual("/dataset1/attribute1", node.name()) 
    node = nodelist.fetchNode("/dataset1/doublearray")
    self.assertEqual("/dataset1/doublearray", node.name()) 

  def testAddDuplicate_GroupNode(self):
    nodelist = _pyhl.nodelist();
    nodelist.addNode(_pyhl.node(_pyhl.GROUP_ID, "/root"))
    nodelist.addNode(_pyhl.node(_pyhl.GROUP_ID, "/root/group1"))
    nodelist.addNode(_pyhl.node(_pyhl.GROUP_ID, "/root/group2"))
    try:
      nodelist.addNode(_pyhl.node(_pyhl.GROUP_ID, "/root/group1"))
      self.fail("Expected IOError")
    except IOError:
      pass


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()