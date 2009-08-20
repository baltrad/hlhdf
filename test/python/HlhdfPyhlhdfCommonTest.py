'''
Created on Aug 20, 2009

@author: anders
'''
import unittest
import _varioustests

class HlhdfPyhlhdfCommonTest(unittest.TestCase):
  def setUp(self):
    pass


  def tearDown(self):
    pass


  def testTranslatePyFormatToHlHdf(self):
    TYPEFORMATS = [('c', "char"),  ('b', "char"),
                   ('B', "uchar"), ('h', "short"),
                   ('H', "ushort"), ('i', "int"),
                   ('l', "long"), ('f', "float"),
                   ('d', "double")]
    for format in TYPEFORMATS:
      result = _varioustests.translatePyFormatToHlhdf(format[0])
      self.assertEquals(format[1], result, "Expected: %s but was: %s"%(format[1],result))

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()