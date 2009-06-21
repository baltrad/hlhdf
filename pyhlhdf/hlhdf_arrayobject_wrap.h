/**
 * Ensures that the python arrayobject.h file is imported correctly.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-13
 */
#ifndef HLHDF_ARRAYOBJECT_WRAP_H
#define HLHDF_ARRAYOBJECT_WRAP_H

#include <Python.h>
/** See arrayobject.h for information on variable */
#define PY_ARRAY_UNIQUE_SYMBOL PyArrayAPIXXX
/** define this variable if NO_IMPORT_ARRAY should be active */
#ifndef HLHDF_PYMODULE_WITH_IMPORT_ARRAY
/** See arrayobject.h for information on variable */
#define NO_IMPORT_ARRAY
#endif
#include <arrayobject.h>

#endif
