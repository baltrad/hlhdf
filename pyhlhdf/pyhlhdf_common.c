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

#ifdef KALLE
/**********************************************************
 *Function:	whatSizeIsHdfFormat
 **********************************************************/
int whatSizeIsHdfFormat(const char* format)
{
   hid_t tmpType;
   int size=-1;
   tmpType=translateCharToDatatype(format);
   if(tmpType<0) {
      fprintf(stderr,"There is no type called %s\n",format);
      return -1;
   }
   size=H5Tget_size(tmpType);
   H5Tclose(tmpType);
   return size;
}
#endif

/**********************************************************
 *Function:	translatePyFormatToHlHdf
 **********************************************************/
char* translatePyFormatToHlHdf(char type)
{
   char* retv=NULL;

   switch(type) {
   case 'c':
      retv=strdup("char");
      break;
   case '1':
      retv=strdup("schar");
      break;
   case 'b':
      retv=strdup("uchar");
      break;
   case 's':
      retv=strdup("short");
      break;
   case 'i':
      retv=strdup("int");
      break;
   case 'l':
      retv=strdup("long");
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
  } else if (strcmp(format, "int") == 0) {
    return PyArray_INT;
  } else if (strcmp(format, "long") == 0) {
    return PyArray_LONG;
  } else if (strcmp(format, "float") == 0) {
    return PyArray_FLOAT;
  } else if (strcmp(format, "double") == 0) {
    return PyArray_DOUBLE;
  } else {
    /*Hmm, there are no more supported types by the PyArray right now, so
     *then I just try to create an array with a datasize equal to the hdf,
     *'char', 'schar', 'uchar', 'short', 'ushort',
     *'int', 'uint', 'long', 'ulong', 'llong',
     *'ullong', 'float', 'double', 'hsize', 'hssize',
     *'herr', 'hbool'
     */
    if (strcmp(format, "ushort") == 0) {
      return PyArray_SHORT;
    } else if (strcmp(format, "uint") == 0) {
      return PyArray_INT;
    } else if (strcmp(format, "ulong") == 0) {
      return PyArray_LONG;
    } else {
      fprintf(stderr, "Unsupported type %s\n", format);
      return -1;
    }
  }
  return -1;
}

/**********************************************************
 *Function:	new1d_ArrayObject
 **********************************************************/
PyArrayObject* new1d_ArrayObject(int nl, \
				 const char* format)
{
   int dims[1];
   PyArrayObject* arr;
   int iformat=pyarraytypeFromHdfType(format);

   if(iformat==-1)
      return NULL;

   dims[0]=nl;

   arr=(PyArrayObject*)PyArray_FromDims(1,dims,iformat);

   return arr;
}

/**********************************************************
 *Function:	new2d_ArrayObject
 **********************************************************/
PyArrayObject* new2d_ArrayObject(int xsize,int ysize, \
					const char* format)
{
   int dims[2];
   PyArrayObject* arr;
   int iformat=pyarraytypeFromHdfType(format);

   if(iformat==-1)
      return NULL;

   dims[0]=ysize;
   dims[1]=xsize;

   arr=(PyArrayObject*)PyArray_FromDims(2,dims,iformat);

   return arr;
}