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
 * Utility functions for working with python objects.
 */
#include "pyhlhdf_common.h"

/**********************************************************
 *Function:	getFloatFromDictionary
 **********************************************************/
int getFloatFromDictionary(char* name, float* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyMapping_GetItemString(dict,name);
   if(pyo) {
      (*val)=(float)PyFloat_AsDouble(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getDoubleFromDictionary
 **********************************************************/
int getDoubleFromDictionary(char* name, double* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyMapping_GetItemString(dict,name);
   if(pyo) {
      (*val)=PyFloat_AsDouble(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getIntegerFromDictionary
 **********************************************************/
int getIntegerFromDictionary(char* name, int* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyMapping_GetItemString(dict,name);
   if(pyo) {
      (*val)=PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getLongFromDictionary
 **********************************************************/
int getLongFromDictionary(char* name, long* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyMapping_GetItemString(dict,name);
   if(pyo) {
      (*val)=(long)PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getUnsignedLongFromDictionary
 **********************************************************/
int getUnsignedLongFromDictionary(char* name, unsigned long* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyMapping_GetItemString(dict,name);
   if(pyo) {
      (*val)=(unsigned long)PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}


/**********************************************************
 *Function:	getFloatFromDictionary
 **********************************************************/
int getFloatFromObject(char* name, float* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyObject_GetAttrString(dict,name);
   if(pyo) {
      (*val)=(float)PyFloat_AsDouble(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getDoubleFromObject
 **********************************************************/
int getDoubleFromObject(char* name, double* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyObject_GetAttrString(dict,name);
   if(pyo) {
      (*val)=PyFloat_AsDouble(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getIntegerFromObject
 **********************************************************/
int getIntegerFromObject(char* name, int* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyObject_GetAttrString(dict,name);
   if(pyo) {
      (*val)=PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getLongFromObject
 **********************************************************/
int getLongFromObject(char* name, long* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyObject_GetAttrString(dict,name);
   if(pyo) {
      (*val)=(long)PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getUnsignedLongFromObject
 **********************************************************/
int getUnsignedLongFromObject(char* name, unsigned long* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyObject_GetAttrString(dict,name);
   if(pyo) {
      (*val)=(unsigned long)PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}


/**********************************************************
 *Function:	getShortFromObject
 **********************************************************/
int getShortFromObject(char* name, short* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyObject_GetAttrString(dict,name);
   if(pyo) {
      (*val)=(short)PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getByteFromObject
 **********************************************************/
int getByteFromObject(char* name, unsigned char* val, PyObject* dict)
{
   PyObject* pyo;
   pyo = PyObject_GetAttrString(dict,name);
   if(pyo) {
      (*val)=(unsigned char)PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getStringFromObject
 **********************************************************/
char* getStringFromObject(char* name, PyObject* dict)
{
   PyObject* pyo;
   char* retstr=NULL;
   pyo=PyObject_GetAttrString(dict,name);
   if(pyo) {
      retstr=strdup(PyString_AsString(pyo));
      Py_DECREF(pyo);
   }
   PyErr_Clear();
   return retstr;
}

/**********************************************************
 *Function:	getIdxIntegerFromTuple
 **********************************************************/
int getIdxIntegerFromTuple(int idx, int* val, PyObject* tuple)
{
   PyObject* pyo;
   pyo=PySequence_GetItem(tuple,idx);
   if(pyo) {
      (*val)=PyInt_AsLong(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getIdxDoubleFromTuple
 **********************************************************/
int getIdxDoubleFromTuple(int idx, double* val, PyObject* tuple)
{
   PyObject* pyo;
   pyo=PySequence_GetItem(tuple,idx);
   if(pyo) {
      (*val)=PyFloat_AsDouble(pyo);
      Py_DECREF(pyo);
      return 1;
   }
   PyErr_Clear();
   return 0;
}

/**********************************************************
 *Function:	getStringFromDictionary
 **********************************************************/
char* getStringFromDictionary(char* name, PyObject* dict)
{
   PyObject* pyo;
   char* retstr=NULL;
   pyo=PyMapping_GetItemString(dict,name);
   if(pyo) {
      retstr=strdup(PyString_AsString(pyo));
      Py_DECREF(pyo);
   }
   PyErr_Clear();
   return retstr;
}

/**********************************************************
 *Function:	setMappingInteger
 **********************************************************/
int setMappingInteger(PyObject* info, char* keyname, int in_value)
{
   int ret=1;

   PyObject* out_value=PyInt_FromLong(in_value);
   if(!out_value)
      return 0;

   if(PyMapping_SetItemString(info,keyname,out_value)==-1)
      ret=0;

   Py_DECREF(out_value);

   return ret;
}

/**********************************************************
 *Function:	setMappingDouble
 **********************************************************/
int setMappingDouble(PyObject* info, char* keyname, double in_value)
{
   int ret=1;

   PyObject* out_value=PyFloat_FromDouble(in_value);
   if(!out_value)
      return 0;

   if(PyMapping_SetItemString(info,keyname,out_value)==-1)
      ret=0;

   Py_DECREF(out_value);

   return ret;
}

/**********************************************************
 *Function:	setMappingFloat
 **********************************************************/
int setMappingFloat(PyObject* info, char* keyname, float in_value)
{
   int ret=1;

   PyObject* out_value=PyFloat_FromDouble((float)in_value);
   if(!out_value)
      return 0;

   if(PyMapping_SetItemString(info,keyname,out_value)==-1)
      ret=0;

   Py_DECREF(out_value);

   return ret;
}

/**********************************************************
 *Function:	setMappingString
 **********************************************************/
int setMappingString(PyObject* info,char* keyname,char* in_string)
{
   int ret=1;
   PyObject* out_string=PyString_FromString(in_string);
   if(!out_string)
      return 0;

   if(PyMapping_SetItemString(info,keyname,out_string)==-1)
      ret=0;

   Py_DECREF(out_string);

   return ret;
}

/**********************************************************
 *Function:	setMappingString_Length
 **********************************************************/
int setMappingString_Length(PyObject* info,char* keyname,char* in_string,int len)
{
   int ret=1;
   PyObject* out_string=PyString_FromStringAndSize(in_string,len);
   if(!out_string)
      return 0;

   if(PyMapping_SetItemString(info,keyname,out_string)==-1)
      ret=0;

   Py_DECREF(out_string);

   return ret;
}

/**********************************************************
 *Function:	setObjectInteger
 **********************************************************/
int setObjectInteger(PyObject* info, char* keyname, int in_value)
{
   int ret=1;

   PyObject* out_value=PyInt_FromLong(in_value);
   if(!out_value)
      return 0;

   if(PyObject_SetAttrString(info,keyname,out_value)==-1)
      ret=0;

   Py_DECREF(out_value);

   return ret;
}

/**********************************************************
 *Function:	setObjectLong
 **********************************************************/
int setObjectLong(PyObject* info, char* keyname, long in_value)
{
   int ret=1;

   PyObject* out_value=PyInt_FromLong(in_value);
   if(!out_value)
      return 0;

   if(PyObject_SetAttrString(info,keyname,out_value)==-1)
      ret=0;

   Py_DECREF(out_value);

   return ret;
}
/**********************************************************
 *Function:	setObjectLong
 **********************************************************/
int setObjectUnsignedLong(PyObject* info, char* keyname, unsigned long in_value)
{
   int ret=1;

   PyObject* out_value=PyInt_FromLong((long)in_value);
   if(!out_value)
      return 0;

   if(PyObject_SetAttrString(info,keyname,out_value)==-1)
      ret=0;

   Py_DECREF(out_value);

   return ret;
}

/**********************************************************
 *Function:	setObjectDouble
 **********************************************************/
int setObjectDouble(PyObject* info, char* keyname, double in_value)
{
   int ret=1;

   PyObject* out_value=PyFloat_FromDouble(in_value);
   if(!out_value)
      return 0;

   if(PyObject_SetAttrString(info,keyname,out_value)==-1)
      ret=0;

   Py_DECREF(out_value);

   return ret;
}

/**********************************************************
 *Function:	setObjectFloat
 **********************************************************/
int setObjectFloat(PyObject* info, char* keyname, float in_value)
{
   int ret=1;

   PyObject* out_value=PyFloat_FromDouble((float)in_value);
   if(!out_value)
      return 0;

   if(PyObject_SetAttrString(info,keyname,out_value)==-1)
      ret=0;

   Py_DECREF(out_value);

   return ret;
}

/**********************************************************
 *Function:	setObjectString
 **********************************************************/
int setObjectString(PyObject* info,char* keyname,char* in_string)
{
   int ret=1;
   PyObject* out_string=PyString_FromString(in_string);
   if(!out_string)
      return 0;

   if(PyObject_SetAttrString(info,keyname,out_string)==-1)
      ret=0;

   Py_DECREF(out_string);

   return ret;
}

/**********************************************************
 *Function:	setObjectString_Length
 **********************************************************/
int setObjectString_Length(PyObject* info,char* keyname,char* in_string,int len)
{
   int ret=1;
   PyObject* out_string=NULL;

   if(strlen(in_string)<len) /*I don't want to have padded strings*/
      return setObjectString(info,keyname,in_string);

   out_string=PyString_FromStringAndSize(in_string,len);
   if(!out_string)
      return 0;

   if(PyObject_SetAttrString(info,keyname,out_string)==-1)
      ret=0;

   Py_DECREF(out_string);

   return ret;
}

/**********************************************************
 *Function:	translatePyFormatToHlHdf
 **********************************************************/
char* translatePyFormatToHlHdf(char type)
{
   char* retv=NULL;

   switch(type) {
   case 'c':
   case 'b':
      retv=strdup("char");
      break;
   case 'B':
      retv=strdup("uchar");
      break;
   case 'h':
      retv=strdup("short");
      break;
   case 'H':
      retv=strdup("ushort");
      break;
   case 'i':
      retv=strdup("int");
      break;
   case 'I':
      retv=strdup("uint");
      break;
   case 'l':
      retv=strdup("long");
      break;
   case 'L':
      retv=strdup("ulong");
      break;
   case 'f':
      retv=strdup("float");
      break;
   case 'd':
      retv=strdup("double");
      break;
   case 'F':
   case 'D':
   case 'O':
   default:
      fprintf(stderr,"Unsupported datatype '%c'\n",type);
   }
   return retv;
}

/**********************************************************
 *Function:	pyarraytypeFromHdfType
 **********************************************************/
int pyarraytypeFromHdfType(const char* format)
{
  if (!format)
    return -1;

  if (strcmp(format, "char") == 0) {
    return PyArray_CHAR;
  } else if (strcmp(format, "schar") == 0) {
    return PyArray_BYTE;
  } else if (strcmp(format, "uchar") == 0) {
    return PyArray_UBYTE;
  } else if (strcmp(format, "short") == 0) {
    return PyArray_SHORT;
  } else if (strcmp(format, "ushort") == 0) {
    return PyArray_USHORT;
  } else if (strcmp(format, "int") == 0) {
    return PyArray_INT;
  } else if (strcmp(format, "uint") == 0) {
    return PyArray_UINT;
  } else if (strcmp(format, "long") == 0) {
    return PyArray_LONG;
  } else if (strcmp(format, "ulong") == 0) {
    return PyArray_ULONG;
  } else if (strcmp(format, "float") == 0) {
    return PyArray_FLOAT;
  } else if (strcmp(format, "double") == 0) {
    return PyArray_DOUBLE;
  } else {
    fprintf(stderr, "Unsupported type %s\n", format);
    return -1;
  }
  return -1;
}

/**********************************************************
 *Function:	new1d_ArrayObject
 **********************************************************/
PyArrayObject* new1d_ArrayObject(int nl, \
				 const char* format)
{
   npy_intp dims[1];
   PyArrayObject* arr;
   int iformat=pyarraytypeFromHdfType(format);

   if(iformat==-1)
      return NULL;

   dims[0]=(npy_intp)nl;

   arr=(PyArrayObject*)PyArray_SimpleNew(1,dims,iformat);

   return arr;
}

/**********************************************************
 *Function:	new2d_ArrayObject
 **********************************************************/
PyArrayObject* new2d_ArrayObject(int xsize,int ysize, \
					const char* format)
{
   npy_intp dims[2];
   PyArrayObject* arr;
   int iformat=pyarraytypeFromHdfType(format);

   if(iformat==-1)
      return NULL;

   dims[0]=(npy_intp)ysize;
   dims[1]=(npy_intp)xsize;

   arr=(PyArrayObject*)PyArray_SimpleNew(2,dims,iformat);

   return arr;
}
