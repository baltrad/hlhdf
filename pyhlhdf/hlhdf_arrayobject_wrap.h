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
