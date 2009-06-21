/**
 * Utility functions for defining compound types through the HDF5-API.
 * These functions are going to change in the near future but are here for
 * backward compatibility.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-12
 */
#include "hlhdf.h"
#include "hlhdf_private.h"
#include "hlhdf_debug.h"
#include "hlhdf_defines_private.h"
#include "hlhdf_compound_utils.h"
#include <stdlib.h>
#include <string.h>

hid_t createCompoundType(size_t size)
{
  HL_DEBUG0("ENTER: createCompoundType");
  return H5Tcreate(H5T_COMPOUND, size);
}

herr_t addAttributeToCompoundType(hid_t loc_id, const char* name,
  size_t offset, hid_t type_id)
{
  HL_DEBUG0("ENTER: addAttributeToCompoundType");
  return H5Tinsert(loc_id, name, offset, type_id);
}

herr_t addAttributeToCompoundType_fmt(hid_t loc_id, const char* name,
  size_t offset, const char* fmt)
{
  herr_t status;
  hid_t type_id = translateCharToDatatype(fmt);
  HL_DEBUG0("ENTER: addAttributeToCompoundType_fmt");
  if (type_id < 0)
    return -1;
  status = addAttributeToCompoundType(loc_id, name, offset, type_id);
  HL_H5T_CLOSE(type_id);
  return status;
}

herr_t addArrayToCompoundType(hid_t loc_id, const char* name, size_t offset,
  int ndims, size_t* dims, hid_t type_id)
{
  herr_t status;
  hid_t array_type;
  int i;
  hsize_t* dims_hsize_t = (hsize_t*) malloc(sizeof(hsize_t) * ndims);
  HL_DEBUG0("ENTER: addArrayToCompoundType");

  if (!dims_hsize_t) {
    HL_ERROR0("Failed to allocate memory for temporary hsize_t dims");
    return -1;
  }

  for (i = 0; i < ndims; i++) {
    dims_hsize_t[i] = dims[i];
  }

  array_type = H5Tarray_create(type_id, ndims, dims_hsize_t);
  status = H5Tinsert(loc_id, name, offset, array_type);
  HL_H5T_CLOSE(array_type);

  HLHDF_FREE(dims_hsize_t);
  return status;
}

herr_t addArrayToCompoundType_fmt(hid_t loc_id, const char* name,
  size_t offset, int ndims, size_t* dims, const char* fmt)
{
  hid_t type_id;
  herr_t status;
  HL_DEBUG0("ENTER: addArrayToCompoundType_fmt");
  type_id = translateCharToDatatype(fmt);
  if (type_id < 0)
    return -1;

  status = addArrayToCompoundType(loc_id, name, offset, ndims, dims, type_id);
  HL_H5T_CLOSE(type_id);

  return status;
}
