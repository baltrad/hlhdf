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

static PyMethodDef functions[] = {
  {"sizeoflong", (PyCFunction)_varioustests_sizeoflong, 1},
  {"sizeoflonglong", (PyCFunction)_varioustests_sizeoflonglong, 1},
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

  ErrorObject = PyString_FromString("_pyhl.error");
  if (ErrorObject == NULL || PyDict_SetItemString(d, "error", ErrorObject) != 0) {
    Py_FatalError("Can't define _pyhl.error");
  }
}
