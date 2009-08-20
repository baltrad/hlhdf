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
 * Various functions that are used during the testing
 */
#include <Python.h>
/** To ensure that arrayobject is imported correctly */
#define HLHDF_PYMODULE_WITH_IMPORT_ARRAY
#include "pyhlhdf_common.h"

static PyObject *ErrorObject;

/**
 * Sets the error string in the python interpreeter
 */
#define setException(type,msg)\
{PyErr_SetString(type,msg);}

static PyObject* _varioustests_sizeoflong(PyObject* self, PyObject* args)
{
  return PyInt_FromLong((int)sizeof(long));
}

static PyObject* _varioustests_sizeoflonglong(PyObject* self, PyObject* args)
{
  return PyInt_FromLong((int)sizeof(long long));
}

static PyObject* _varioustests_translatePyFormatToHlHdf(PyObject* self, PyObject* args)
{
  char c=(char)0;
  char* format = NULL;
  PyObject* result = NULL;

  if (!PyArg_ParseTuple(args, "c", &c)) {
    return NULL;
  }

  format = translatePyFormatToHlHdf(c);
  if (format == NULL) {
    return NULL;
  }

  result = PyString_FromString(format);
  free(format);
  return result;
}

static PyMethodDef functions[] = {
  {"sizeoflong", (PyCFunction)_varioustests_sizeoflong, 1},
  {"sizeoflonglong", (PyCFunction)_varioustests_sizeoflonglong, 1},
  {"translatePyFormatToHlhdf", (PyCFunction)_varioustests_translatePyFormatToHlHdf, 1},
  {NULL,NULL} /*Sentinel*/
};

/**
 * Initializes the _varioustests module
 */
void init_varioustests(void)
{
  PyObject *m, *d;

  m = Py_InitModule("_varioustests",functions);
  d = PyModule_GetDict(m);

  import_array(); /*To make sure I get access to Numeric*/

  ErrorObject = PyString_FromString("_pyhl.error");
  if (ErrorObject == NULL || PyDict_SetItemString(d, "error", ErrorObject) != 0) {
    Py_FatalError("Can't define _pyhl.error");
  }
}
