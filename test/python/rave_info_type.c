/* --------------------------------------------------------------------
Copyright (C) 2009 Swedish Meteorological and Hydrological Institute, SMHI,

This file is part of HLHDF.

HLHDF is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

HLHDF is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with HLHDF.  If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------------------------------*/

/**
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-15
 *
 * C-definition of a compound type usable from python
 */
#include <Python.h>
/** To ensure that arrayobject is imported correctly */
#define HLHDF_PYMODULE_WITH_IMPORT_ARRAY
#include "pyhlhdf_common.h"  /* this includes arrayobject.h */
#include "hlhdf.h"
#include "hlhdf_compound_utils.h"

/** Sets an exception in the python interpreter and returns NULL which will generate an exception */
#define raiseException(type,msg)\
{PyErr_SetString(type,msg);return NULL;}

/** Sets an exception in the python interpreter */
#define setException(type,msg)\
{PyErr_SetString(type,msg);}

/**
 * The compound struct
 */
typedef struct {
   int xsize; /**< xsize */
   int ysize; /**< ysize */
   double xscale; /**< xscale */
   double yscale; /**< yscale */
   double area_extent[4]; /**< area extent */
} RaveInfoStruct;

/**
 * The python type object
 */
typedef struct {
   PyObject_HEAD /*Always have to be on top*/
   hid_t rave_info_hid; /**< The HDF5 hid for the rave info type */
} RaveInfoType;

/**
 * The python data object
 */
typedef struct {
   PyObject_HEAD /*Always have to be on top*/
   RaveInfoStruct info; /**< The rave info struct */
} RaveInfoObject;


/**
 * Forward declaration of the RaveInfoType
 */
staticforward PyTypeObject RaveInfoType_Type;

/**
 * Checks if the object is a RaveInfoType
 */
#define RaveInfoType_Check(op) ((op)->ob_type == &RaveInfoType_Type)

/**
 * Forward declaration of the RaveInfoObject
 */
staticforward PyTypeObject RaveInfoObject_Type;

/**
 * Checks if the object is a RaveInfoObject
 */
#define RaveInfoObject_Check(op) ((op)->ob_type == &RaveInfoObject_Type)

/**
 * Creating the compound type definition in HDF5 using HLHDFs utility functions
 */
static hid_t createRaveInfoType(void)
{
   hid_t type_id=-1;
   size_t dims[]={4};
   if((type_id=createCompoundType(sizeof(RaveInfoStruct)))<0)
      return -1;

   if(addAttributeToCompoundType_fmt(type_id,"xsize",HOFFSET(RaveInfoStruct,xsize),"int")<0)
      goto fail;
   if(addAttributeToCompoundType_fmt(type_id,"ysize",HOFFSET(RaveInfoStruct,ysize),"int")<0)
      goto fail;
   if(addAttributeToCompoundType_fmt(type_id,"xscale",HOFFSET(RaveInfoStruct,xscale),"double")<0)
      goto fail;
   if(addAttributeToCompoundType_fmt(type_id,"yscale",HOFFSET(RaveInfoStruct,yscale),"double")<0)
      goto fail;
   if(addArrayToCompoundType_fmt(type_id,"area_extent",HOFFSET(RaveInfoStruct,area_extent[0]),1,dims,"double")<0)
      goto fail;
   return type_id;
 fail:
   if(type_id>=0)
      H5Tclose(type_id);
   return -1;
}

static void _dealloc_type(RaveInfoType* rinfo)
{
   if(rinfo->rave_info_hid>=0) {
      HL_disableErrorReporting();
      H5Tclose(rinfo->rave_info_hid);
      HL_enableErrorReporting();
   }
   PyObject_Del(rinfo);
}

static PyObject* _rave_info_type_new_type(PyObject* self, PyObject* args)
{
   RaveInfoType* retv=NULL;
   retv=PyObject_NEW(RaveInfoType,&RaveInfoType_Type);
   if(!retv)
      return NULL;
   retv->rave_info_hid=-1;
   if((retv->rave_info_hid=createRaveInfoType())<0) {
      setException(PyExc_TypeError,"Could not create rave info type");
      _dealloc_type(retv);
      retv=NULL;
   }
   return (PyObject*)retv;
}

static void _dealloc_object(RaveInfoObject* robj)
{
   PyObject_Del(robj);
}

/**
 * Creates a new RaveInfoObject
 */
static PyObject* _rave_info_type_new_object(PyObject* self, PyObject* args)
{
   RaveInfoObject* retv=NULL;
   retv=PyObject_NEW(RaveInfoObject,&RaveInfoObject_Type);
   if(!retv)
      return NULL;
   retv->info.xsize=0;
   retv->info.ysize=0;
   retv->info.xscale=0.0;
   retv->info.yscale=0.0;
   memset(retv->info.area_extent,0,sizeof(double)*4);
   return (PyObject*)retv;
}

/**
 * Returns the type hid as a long
 */
static PyObject* _rave_info_type_hid(RaveInfoType* self, PyObject* args)
{
   return PyInt_FromLong(self->rave_info_hid);
}

/**
 * Returns the size of the stored compound data
 */
static PyObject* _rave_info_type_size(RaveInfoType* self, PyObject* args)
{
   return PyInt_FromLong(sizeof(RaveInfoStruct));
}

/**
 * Creates a RaveInfoObject from a string value
 */
static PyObject* _rave_info_object_fromstring(RaveInfoObject* self, PyObject* args)
{
   char* thestr=NULL;
   size_t len;
   char errbuf[256];
   if(!PyArg_ParseTuple(args,"s#",&thestr,&len))
      return NULL;
   if(len!=sizeof(RaveInfoStruct)) {
      sprintf(errbuf,"Length of data (%ld) is not the same size as of RaveInfoStruct (%ld)",
	      len,sizeof(RaveInfoStruct));
      setException(PyExc_TypeError,errbuf);
      return NULL;
   }
   memcpy(&self->info,thestr,sizeof(RaveInfoStruct));
   Py_INCREF(Py_None);
   return Py_None;
}

/**
 * Creates a string from a RaveInfoObject
 */
static PyObject* _rave_info_object_tostring(RaveInfoObject* self, PyObject* args)
{
   return PyString_FromStringAndSize((char*)&self->info,sizeof(RaveInfoStruct));
}

static struct PyMethodDef type_methods[] =
{
   {"hid",(PyCFunction)_rave_info_type_hid,1},
   {"size",(PyCFunction)_rave_info_type_size,1},
   {NULL,NULL} /*Sentinel*/
};

static struct PyMethodDef object_methods[] =
{
   {"fromstring",(PyCFunction)_rave_info_object_fromstring,1},
   {"tostring",(PyCFunction)_rave_info_object_tostring,1},
   {"xsize",NULL},
   {"ysize",NULL},
   {"xscale",NULL},
   {"yscale",NULL},
   {"area_extent",NULL},
   {NULL,NULL} /*Sentinel*/
};

static PyObject* _getattr_type(RaveInfoType* self, char* name)
{
    PyObject* res;

    res = Py_FindMethod(type_methods, (PyObject*) self, name);
    if (res)
	return res;

    PyErr_Clear();

    PyErr_SetString(PyExc_AttributeError, name);

    return NULL;
}

static PyObject* _getattr_object(RaveInfoObject* self, char* name)
{
    PyObject* res;

    if(strcmp(name,"xsize")==0)
       return PyInt_FromLong(self->info.xsize);
    if(strcmp(name,"ysize")==0)
       return PyInt_FromLong(self->info.ysize);
    if(strcmp(name,"xscale")==0)
       return PyFloat_FromDouble(self->info.xscale);
    if(strcmp(name,"yscale")==0)
       return PyFloat_FromDouble(self->info.yscale);
    if(strcmp(name,"area_extent")==0) {
       return Py_BuildValue("(dddd)",self->info.area_extent[0],
			    self->info.area_extent[1],
			    self->info.area_extent[2],
			    self->info.area_extent[3]);
    }
    res = Py_FindMethod(object_methods, (PyObject*) self, name);
    if (res)
	return res;

    PyErr_Clear();

    PyErr_SetString(PyExc_AttributeError, name);

    return NULL;
}

static int _setattr_object(RaveInfoObject* self, char* name, PyObject* op)
{
  if (strcmp(name, "xsize") == 0) {
    if (PyFloat_Check(op) || PyInt_Check(op)) {
      self->info.xsize = PyInt_AsLong(op);
      return 0;
    }
    setException(PyExc_ValueError,"Value not of type int");
    return -1;
  }
  if (strcmp(name, "ysize") == 0) {
    if (PyFloat_Check(op) || PyInt_Check(op)) {
      self->info.ysize = PyInt_AsLong(op);
      return 0;
    }
    setException(PyExc_ValueError,"Value not of type int");
    return -1;
  }
  if (strcmp(name, "xscale") == 0) {
    if (PyFloat_Check(op) || PyInt_Check(op)) {
      self->info.xscale = PyFloat_AsDouble(op);
      return 0;
    }
    setException(PyExc_ValueError,"Value not of type float");
    return -1;
  }
  if (strcmp(name, "yscale") == 0) {
    if (PyFloat_Check(op) || PyInt_Check(op)) {
      self->info.yscale = PyFloat_AsDouble(op);
      return 0;
    }
    setException(PyExc_ValueError,"Value not of type float");
    return -1;
  }
  if (strcmp(name, "area_extent") == 0) {
    if (PySequence_Check(op)) {
      if (PyArg_ParseTuple(op, "dddd", &self->info.area_extent[0],
                           &self->info.area_extent[1],
                           &self->info.area_extent[2],
                           &self->info.area_extent[3])) {
        return 0;
      }
      setException(PyExc_ValueError,"Area extent not set as tuple");
      return -1;
    }
  }
  PyErr_SetString(PyExc_AttributeError,
                  "Attribute does not exist or cannot be set");
  return -1;
}

statichere PyTypeObject RaveInfoType_Type = {
    PyObject_HEAD_INIT(NULL)
    0,				/*ob_size*/
    "RaveInfoTypeCore",		/*tp_name*/
    sizeof(RaveInfoType),	/*tp_size*/
    0,				/*tp_itemsize*/
    /* methods */
    (destructor)_dealloc_type,	/*tp_dealloc*/
    0,				/*tp_print*/
    (getattrfunc)_getattr_type,	/*tp_getattr*/
    0,				/*tp_setattr*/
    0,				/*tp_compare*/
    0,				/*tp_repr*/
    0,                          /*tp_as_number */
    0,
    0,                          /*tp_as_mapping */
    0                           /*tp_hash*/
};

statichere PyTypeObject RaveInfoObject_Type = {
    PyObject_HEAD_INIT(NULL)
    0,				/*ob_size*/
    "RaveInfoObjectCore",	/*tp_name*/
    sizeof(RaveInfoObject),	/*tp_size*/
    0,				/*tp_itemsize*/
    /* methods */
    (destructor)_dealloc_object,/*tp_dealloc*/
    0,				/*tp_print*/
    (getattrfunc)_getattr_object,/*tp_getattr*/
    (setattrfunc)_setattr_object,/*tp_setattr*/
    0,				/*tp_compare*/
    0,				/*tp_repr*/
    0,                          /*tp_as_number */
    0,
    0,                          /*tp_as_mapping */
    0                           /*tp_hash*/
};

static PyMethodDef functions[] = {
   {"type",(PyCFunction)_rave_info_type_new_type, 1},
   {"object",(PyCFunction)_rave_info_type_new_object, 1},
   {NULL,NULL}
};

/**
 * Initializes the _rave_info_type
 */
void init_rave_info_type(void)
{
   PyObject *m;
   RaveInfoType_Type.ob_type=&PyType_Type;
   RaveInfoObject_Type.ob_type=&PyType_Type;

   m=Py_InitModule("_rave_info_type",functions);

   import_array(); /*To make sure I get access to Numeric*/
   /*Always have to do this*/
   HL_init();
   /*And this I just do to be able to get debugging info from hdf*/
   HL_setDebugMode(2);
}
