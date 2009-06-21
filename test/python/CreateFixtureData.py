'''
Created on Jun 5, 2009

@author: anders
'''
import _pyhl
import _rave_info_type

import numpy
import os

class CreateFixtureData(object):
  '''
  DO NOT USE UNLESS YOU HAVE ADDED NEW DATATYPES OR SOMETHING LIKE THAT SINCE THIS CREATES
  THE FIXTURE DATA. MANUAL VERIFICATION NEEDS TO BE DONE IN ORDER TO VERIFY THAT THE FIXTURE
  IS CORRECT.
  '''
  
  VHLHDF_READ_DATAFILE = "fixture_VhlhdfRead_datafile.h5"
  
  def __init__(self):
    pass
 
  def createDataset(self, size, numtype):
    if numtype == numpy.character:
      a=numpy.zeros((size[0],size[1]),numtype)
      ll = ['a','b','c','d','e','f','0','1','2','3','4','5','6','A','B','C','D','E']
      ctr=0
      for x in range(0, size[0]):
        for y in range(0,size[1]):
          a[y][x] = ll[ctr]
          ctr += 1
          if ctr >= len(ll):
            ctr = 0
      return a
      
    else:
      a=numpy.zeros((size[0],size[1]),numtype)
      ctr=0
      for x in range(0, size[0]):
        for y in range(0,size[1]):
          a[y][x] = ctr
          ctr += 1
          if ctr >= 100:
            ctr = 0
      return a
     
  def createVHLHDF_READ_DATAFILE(self):
    a=_pyhl.nodelist()

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/stringvalue")
    b.setScalarValue(-1,"My String","string",-1)
    a.addNode(b)    
    
    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/charvalue")
    b.setScalarValue(-1,123,"char",-1)
    a.addNode(b)    
    
    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/scharvalue")
    b.setScalarValue(-1,45,"schar",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/ucharvalue")
    b.setScalarValue(-1,99,"uchar",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/shortvalue")
    b.setScalarValue(-1,4321,"short",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/ushortvalue")
    b.setScalarValue(-1,9999,"ushort",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/intvalue")
    b.setScalarValue(-1,989898,"int",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/uintvalue")
    b.setScalarValue(-1,987654,"uint",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/longvalue")
    b.setScalarValue(-1,-123456789,"long",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/ulongvalue")
    b.setScalarValue(-1,123456789,"ulong",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/llongvalue")
    b.setScalarValue(-1,-123456789012,"llong",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/ullongvalue")
    b.setScalarValue(-1,123456789012,"ullong",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/floatvalue")
    b.setScalarValue(-1,12.65,"float",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/doublevalue")
    b.setScalarValue(-1,12999.8989,"double",-1)
    a.addNode(b)    

    #Not supported yet
    #b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/ldoublevalue")
    #b.setScalarValue(-1,65765.762525,"ldouble",-1)
    #a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/hsizevalue")
    b.setScalarValue(-1,65765,"hsize",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/herrvalue")
    b.setScalarValue(-1,12,"herr",-1)
    a.addNode(b)    

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/hboolvalue")
    b.setScalarValue(-1,0,"herr",-1)
    a.addNode(b)
    
    b=_pyhl.node(_pyhl.GROUP_ID, "/group1")
    a.addNode(b)    

    b=_pyhl.node(_pyhl.GROUP_ID, "/group1/group11")
    a.addNode(b)    
    
    #
    #DATASETS FOR char, schar, uchar, short, ushort, int, uint, long, float, double
    #
    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/chardset")
    c = self.createDataset([5,5],numpy.character)
    b.setArrayValue(1,[5,5],c,"char",-1)  
    a.addNode(b)    

    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/schardset")
    c = self.createDataset([5,5],numpy.int8)
    b.setArrayValue(1,[5,5],c,"schar",-1)  
    a.addNode(b)    
        
    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/uchardset")
    c = self.createDataset([5,5],numpy.uint8)
    b.setArrayValue(1,[5,5],c,"uchar",-1)  
    a.addNode(b)    
    
    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/shortdset")
    c = self.createDataset([5,5],numpy.int16)
    b.setArrayValue(1,[5,5],c,"short",-1)  
    a.addNode(b)    

    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/ushortdset")
    c = self.createDataset([5,5],numpy.uint16)
    b.setArrayValue(1,[5,5],c,"ushort",-1)  
    a.addNode(b)    

    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/intdset")
    c = self.createDataset([5,5],numpy.int32)
    b.setArrayValue(1,[5,5],c,"int",-1)  
    a.addNode(b)    

    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/uintdset")
    c = self.createDataset([5,5],numpy.uint32)
    b.setArrayValue(1,[5,5],c,"uint",-1)  
    a.addNode(b)    
    
    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/longdset")
    c = self.createDataset([5,5],numpy.int64)
    b.setArrayValue(1,[5,5],c,"long",-1)  
    a.addNode(b)    

    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/floatdset")
    c = self.createDataset([5,5],numpy.float32)
    b.setArrayValue(1,[5,5],c,"float",-1)  
    a.addNode(b)    
    
    b=_pyhl.node(_pyhl.DATASET_ID, "/group1/doubledset")
    c = self.createDataset([5,5],numpy.float64)
    b.setArrayValue(1,[5,5],c,"double",-1)  
    a.addNode(b)    
    
    # ARRAYS OF TYPE string, double, float, int and long
    b=_pyhl.node(_pyhl.DATASET_ID, "/stringarray")
    c = ["ABC", "def", "EFG"]
    b.setArrayValue(1,[len(c)],c,"string",-1)  
    a.addNode(b)   
    
    b=_pyhl.node(_pyhl.DATASET_ID, "/doublearray")
    c = [1.0, 2.1, 3.2]
    b.setArrayValue(1,[len(c)],c,"double",-1)  
    a.addNode(b)   

    b=_pyhl.node(_pyhl.DATASET_ID, "/floatarray")
    c = [1.1, 2.2, 3.3]
    b.setArrayValue(1,[len(c)],c,"float",-1)  
    a.addNode(b)  
         
    b=_pyhl.node(_pyhl.DATASET_ID, "/intarray")
    c = [1, 2, 3, 4]
    b.setArrayValue(1,[len(c)],c,"int",-1)  
    a.addNode(b)  

    b=_pyhl.node(_pyhl.DATASET_ID, "/longarray")
    c = [2, 3, 4, 5, 6]
    b.setArrayValue(1,[len(c)],c,"long",-1)  
    a.addNode(b)
    
    # REFERENCES POINTING AT DIFFERENT PLACES
    b=_pyhl.node(_pyhl.GROUP_ID, "/references")
    a.addNode(b)
    
    b=_pyhl.node(_pyhl.REFERENCE_ID,"/references/doublearray")
    b.setScalarValue(-1,"/doublearray","string",-1)
    a.addNode(b)  

    b=_pyhl.node(_pyhl.REFERENCE_ID,"/references/floatdset")
    b.setScalarValue(-1,"/group1/floatdset","string",-1)
    a.addNode(b)  

    b=_pyhl.node(_pyhl.REFERENCE_ID,"/references/group1")
    b.setScalarValue(-1,"/group1","string",-1)
    a.addNode(b)

    #b=_pyhl.node(_pyhl.REFERENCE_ID,"/references/tosomethingdefinedafterthis")
    #b.setScalarValue(-1,"/groupdefinedafterreferences","string",-1)
    #a.addNode(b)

    #b=_pyhl.node(_pyhl.GROUP_ID, "/groupdefinedafterreferences")
    #a.addNode(b)

    b=_pyhl.node(_pyhl.REFERENCE_ID,"/rootreferencetolongarray")
    b.setScalarValue(-1,"/longarray","string",-1)
    a.addNode(b)

    #Add a compound type as well
    rinfo_type=_rave_info_type.type()
    rinfo_obj=_rave_info_type.object()
    rinfo_obj.xsize=10
    rinfo_obj.ysize=10
    rinfo_obj.xscale=150.0
    rinfo_obj.yscale=150.0
    b=_pyhl.node(_pyhl.TYPE_ID,"/RaveDatatype")
    b.commit(rinfo_type.hid())
    a.addNode(b)
    b=_pyhl.node(_pyhl.GROUP_ID, "/compoundgroup")
    a.addNode(b)
    
    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/compoundgroup/attribute")
    b.setScalarValue(rinfo_type.size(),rinfo_obj.tostring(),"compound",rinfo_type.hid())
    a.addNode(b)

    rinfo_obj.xsize=99
    rinfo_obj.ysize=109
    rinfo_obj.area_extent=(10.0, 20.0, 30.0, 40.0)
    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/compoundgroup/attribute2")
    b.setScalarValue(rinfo_type.size(),rinfo_obj.tostring(),"compound",rinfo_type.hid())
    a.addNode(b)

    rinfo_obj.xscale=170.0
    rinfo_obj.area_extent=(10.0, 20.0, 35.0, 40.0)
    b=_pyhl.node(_pyhl.DATASET_ID,"/compoundgroup/dataset")
    b.setArrayValue(rinfo_type.size(),[1],rinfo_obj.tostring(),"compound",rinfo_type.hid())
    a.addNode(b)

    # Create a compund array containing 2x2 items
    rinfo_obj2 =_rave_info_type.object()
    rinfo_obj2.xsize = 98
    rinfo_obj2.ysize = 97
    rinfo_obj2.xscale = 120.0
    rinfo_obj2.yscale = 130.0
    rinfo_obj2.area_extent = (33.0,32.0,31.0,30.0)

    rinfo_obj3 =_rave_info_type.object()
    rinfo_obj3.xsize = 88
    rinfo_obj3.ysize = 87
    rinfo_obj3.xscale = 100.0
    rinfo_obj3.yscale = 110.0
    rinfo_obj3.area_extent = (43.0,42.0,41.0,40.0)

    rinfo_obj4 =_rave_info_type.object()
    rinfo_obj4.xsize = 78
    rinfo_obj4.ysize = 77
    rinfo_obj4.xscale = 90.0
    rinfo_obj4.yscale = 91.0
    rinfo_obj4.area_extent = (53.0,52.0,51.0,50.0)
    
    b=_pyhl.node(_pyhl.DATASET_ID,"/compoundgroup/dataset2")
    str = rinfo_obj.tostring() + rinfo_obj2.tostring() + rinfo_obj3.tostring() + rinfo_obj4.tostring()
    b.setArrayValue(rinfo_type.size(),[2,2],str,"compound",rinfo_type.hid())
    a.addNode(b)

    # Create an unamed compound value
    rinfo_unnamed_type=_rave_info_type.type()
    rinfo_unnamed_obj=_rave_info_type.object()

    rinfo_unnamed_obj.xsize = 1
    rinfo_unnamed_obj.ysize = 2
    rinfo_unnamed_obj.xscale = 10.0
    rinfo_unnamed_obj.yscale = 20.0
    rinfo_unnamed_obj.area_extent = (1.0, 2.0, 3.0, 4.0)

    b=_pyhl.node(_pyhl.ATTRIBUTE_ID,"/compoundgroup/unnamed_type_attribute")
    b.setScalarValue(rinfo_unnamed_type.size(),rinfo_unnamed_obj.tostring(),"compound",rinfo_unnamed_type.hid())
    a.addNode(b)

    a.write(self.VHLHDF_READ_DATAFILE)
    
if __name__ == "__main__":
  a = CreateFixtureData()
  a.createVHLHDF_READ_DATAFILE()
  
            