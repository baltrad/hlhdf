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
