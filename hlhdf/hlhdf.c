/**
 * Common functions for working with an HDF5 file through the HL-HDF API.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-15
 */
#include "hlhdf.h"
#include "hlhdf_debug.h"
#include "hlhdf_defines_private.h"
#include <string.h>
#include <stdlib.h>

/*For internal use*/
static int errorReportingOn=1;
static void *edata;
static herr_t (*errorFunction)(hid_t estack, void *client_data);

/** Flag toggling the debugging */
static int _debug_hdf;

const char* HLHDF_CHAR    = "char";
const char* HLHDF_SCHAR   = "schar";
const char* HLHDF_UCHAR   = "uchar";
const char* HLHDF_SHORT   = "short";
const char* HLHDF_USHORT  = "ushort";
const char* HLHDF_INT     = "int";
const char* HLHDF_UINT    = "uint";
const char* HLHDF_LONG    = "long";
const char* HLHDF_ULONG   = "ulong";
const char* HLHDF_LLONG   = "llong";
const char* HLHDF_ULLONG  = "ullong";
const char* HLHDF_FLOAT   = "float";
const char* HLHDF_DOUBLE  = "double";
const char* HLHDF_HSIZE   = "hsize";
const char* HLHDF_HSSIZE  = "hssize";
const char* HLHDF_HERR    = "herr";
const char* HLHDF_HBOOL   = "hbool";

/************************************************
 * disableErrorReporting
 ***********************************************/
void disableErrorReporting()
{
  /*Disable error reporting*/
  if (errorReportingOn == 1) {
    H5Eget_auto2(H5E_DEFAULT, &errorFunction, &edata);
    H5Eset_auto2(H5E_DEFAULT, NULL, NULL);
    errorReportingOn = 0;
  }
}

/************************************************
 * enableErrorReporting
 ***********************************************/
void enableErrorReporting()
{
  if (errorReportingOn == 0) {
    H5Eset_auto2(H5E_DEFAULT, errorFunction, edata);
    errorReportingOn = 1;
  }
}

/************************************************
 * initHlHdf
 ***********************************************/
void initHlHdf()
{
  _debug_hdf = 0;
  H5Eset_auto2(H5E_DEFAULT, HL_hdf5_debug_function, NULL); /* Force logging to always goto HLHDFs handler */
  HL_InitializeDebugger();
  HL_enableHdf5ErrorReporting();
  disableErrorReporting();
}

/************************************************
 * debugHlhdf
 ***********************************************/
void debugHlHdf(int flag)
{
  if (flag == 0) {
    /*Don't debug anything*/
    _debug_hdf = 0;
    disableErrorReporting();
  } else if (flag == 1) {
    /*Only debug HLHDF stuff*/
    _debug_hdf = 1;
    HL_setDebugLevel(HLHDF_DEBUG);
    disableErrorReporting();
  } else {
    /*Debug everything*/
    _debug_hdf = 1;
    HL_setDebugLevel(HLHDF_DEBUG);
    enableErrorReporting();
  }
}
/************************************************
 * isHdf5File
 ***********************************************/
int isHdf5File(const char* filename)
{
  htri_t checkValue = H5Fis_hdf5(filename);
  HL_DEBUG0("isHdf5File");
  if (checkValue > 0)
    return TRUE;
  else
    return FALSE;
}

/************************************************
 * openHlHdfFile
 ***********************************************/
hid_t openHlHdfFile(const char* filename, const char* how)
{
  unsigned flags = H5F_ACC_RDWR;
  HL_DEBUG0("ENTER: openHlHdfFile");

  if (strcmp(how, "r") == 0) {
    flags = H5F_ACC_RDONLY;
  } else if (strcmp(how, "w") == 0 || strcmp(how, "rw") == 0 || strcmp(how, "wr") == 0) {
    flags = H5F_ACC_RDWR;
  } else {
    HL_ERROR0("Illegal mode given when opening file, should be (r|w|rw)");
    return (hid_t) -1;
  }
  return H5Fopen(filename, flags, H5P_DEFAULT);
}

/************************************************
 * createHlHdfFile
 ***********************************************/
hid_t createHlHdfFile(const char* filename,
  HL_FileCreationProperty* property)
{
  hid_t propId = -1;
  hid_t fileId = -1;
  hid_t fileaccesspropertyId = -1;

  HL_DEBUG0("ENTER: createHlHdfFileWithProperty"); HL_DEBUG0("Using default properties");
  if (property == NULL) {
    return H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  } else {
    HL_DEBUG0("Using specific properties");
    if ((propId = H5Pcreate(H5P_FILE_CREATE)) < 0) {
      HL_ERROR0("Failed to create the property");
      goto fail;
    }

    HL_DEBUG1("Setting userblock property to %d",property->userblock);
    if (H5Pset_userblock(propId, property->userblock) < 0) {
      HL_ERROR0("Failed to set the userblock property");
      goto fail;
    }

    HL_DEBUG2("Setting sizes to %d, %d",property->sizes.sizeof_addr,property->sizes.sizeof_size);
    if (H5Pset_sizes(propId, property->sizes.sizeof_addr,
                     property->sizes.sizeof_size) < 0) {
      HL_ERROR0("Failed to set the sizes property");
      goto fail;
    }

    HL_DEBUG2("Setting sym_k to %d, %d",property->sym_k.ik,property->sym_k.lk);
    if (H5Pset_sym_k(propId, property->sym_k.ik, property->sym_k.lk) < 0) {
      HL_ERROR0("Failed to set the sym_k property");
      goto fail;
    }

    HL_DEBUG1("Setting istore_k to %d",property->istore_k);
    if (H5Pset_istore_k(propId, property->istore_k) < 0) {
      HL_ERROR0("Failed to set the istore_k property");
      goto fail;
    }

    if (property->meta_block_size != 2048) {
      if ((fileaccesspropertyId = H5Pcreate(H5P_FILE_ACCESS)) < 0) {
        HL_ERROR0("Failed to create the H5P_FILE_ACCESS property");
        goto fail;
      }
      if (H5Pset_meta_block_size(fileaccesspropertyId,
                                 property->meta_block_size) < 0) {
        HL_ERROR0("Failed to set the meta block size");
        goto fail;
      }
      fileId = H5Fcreate(filename, H5F_ACC_TRUNC, propId, fileaccesspropertyId);
      H5Pclose(fileaccesspropertyId);
      H5Pclose(propId);
      return fileId;
    } else {
      fileId = H5Fcreate(filename, H5F_ACC_TRUNC, propId, H5P_DEFAULT);
      H5Pclose(propId);
      return fileId;
    }
  }

fail:
  HL_H5P_CLOSE(propId);
  HL_H5P_CLOSE(fileaccesspropertyId);
  return -1;
}

/************************************************
 * createHlHdfFileCreationProperty
 ***********************************************/
HL_FileCreationProperty* createHlHdfFileCreationProperty()
{
  HL_FileCreationProperty* retv = NULL;
  hid_t theHid = -1;

  HL_DEBUG0("ENTER: createHlHdfFileCreationProperty");

  if ((retv
      = (HL_FileCreationProperty*) malloc(sizeof(HL_FileCreationProperty)))
      ==NULL) {
    HL_ERROR0("Failure when allocating memory for HL_FileCreationProperty");
    return NULL;
  }

  if ((theHid = H5Pcreate(H5P_FILE_CREATE)) < 0) {
    HL_ERROR0("Failure when creating the property list");
    free(retv);
    return NULL;
  }

  /* Fetch default information */
  if (H5Pget_version(theHid, &retv->version.super, &retv->version.freelist,
                     &retv->version.stab, &retv->version.shhdr) < 0) {
    HL_ERROR0("Failure while getting version for property");
    goto fail;
  }

  if (H5Pget_userblock(theHid, &retv->userblock) < 0) {
    HL_ERROR0("Failure while getting the userblock for property");
    goto fail;
  }

  if (H5Pget_sizes(theHid, &retv->sizes.sizeof_addr, &retv->sizes.sizeof_size)
      < 0) {
    HL_ERROR0("Failure while getting the sizes for property");
    goto fail;
  }

  if (H5Pget_sym_k(theHid, &retv->sym_k.ik, &retv->sym_k.lk) < 0) {
    HL_ERROR0("Failure while getting the sym_k for property");
    goto fail;
  }

  if (H5Pget_istore_k(theHid, &retv->istore_k) < 0) {
    HL_ERROR0("Failure while getting the istore_k for property");
    goto fail;
  }

  HL_H5P_CLOSE(theHid);
  if ((theHid = H5Pcreate(H5P_FILE_ACCESS)) < 0) {
    HL_ERROR0("Failure when creating the file access property list");
    goto fail;
  }
  if (H5Pget_meta_block_size(theHid, &retv->meta_block_size) < 0) {
    HL_ERROR0("Failure while getting the meta_block_size for property");
    goto fail;
  }
  HL_H5P_CLOSE(theHid);

  return retv;
fail:
  HL_H5P_CLOSE(theHid);
  freeHL_fileCreationProperty(retv);
  return NULL;
}

/************************************************
 * freeHL_fileCreationProperty
 ***********************************************/
void freeHL_fileCreationProperty(HL_FileCreationProperty* prop)
{
  HL_DEBUG0("ENTER: freeHL_fileCreationProperty");
  if (prop == NULL) {
    return;
  }
  free(prop);
}

/************************************************
 * closeHlhdfFile
 ***********************************************/
herr_t closeHlHdfFile(hid_t file_id)
{
  HL_DEBUG0("ENTER: closeHlHdfFile");
  return H5Fclose(file_id);
}

/************************************************
 * getFixedType
 ***********************************************/
hid_t getFixedType(hid_t type)
{
  size_t size;
  hid_t mtype = -1;
  H5T_str_t strpad;
  hid_t* memb = NULL;
  char** name = NULL;
  int nmembs = 0, i, j, *ndims = NULL;
  size_t offset, nelmts;
  hsize_t *dims141 = NULL;
  hid_t f_memb = -1;
  hid_t member_type;
  hid_t tmpt = -1;

  HL_SPEWDEBUG0("ENTER: getFixedType, version > 1.4");

  size = H5Tget_size(type);

  switch (H5Tget_class(type)) {
  case H5T_INTEGER:
    HL_SPEWDEBUG0("This is of type H5T_INTEGER");
    if (size <= sizeof(char))
      mtype = H5Tcopy(H5T_NATIVE_SCHAR);
    else if (size <= sizeof(short))
      mtype = H5Tcopy(H5T_NATIVE_SHORT);
    else if (size <= sizeof(int))
      mtype = H5Tcopy(H5T_NATIVE_INT);
    else if (size <= sizeof(long))
      mtype = H5Tcopy(H5T_NATIVE_LONG);
    else
      mtype = H5Tcopy(H5T_NATIVE_LLONG);
    H5Tset_sign(mtype, H5Tget_sign(type));
    break;
  case H5T_FLOAT:
    HL_SPEWDEBUG0("This is of type H5T_FLOAT");
    if (size <= sizeof(float))
      mtype = H5Tcopy(H5T_NATIVE_FLOAT);
    else if (size <= sizeof(double))
      mtype = H5Tcopy(H5T_NATIVE_DOUBLE);
    else
      mtype = H5Tcopy(H5T_NATIVE_LDOUBLE);
    break;
  case H5T_STRING:
    HL_SPEWDEBUG0("This is of type H5T_STRING");
    mtype = H5Tcopy(H5T_C_S1);
    H5Tset_size(mtype, size);
    strpad = H5Tget_strpad(type);
    H5Tset_strpad(mtype, strpad);
    if (H5Tequal(mtype, type) < 0) {
      H5Tclose(mtype);
      mtype = H5Tcopy(H5T_FORTRAN_S1);
      H5Tset_size(mtype, size);
      H5Tset_strpad(mtype, strpad);
      if (H5Tequal(mtype, type) < 0)
        mtype = -1;
    }
    break;
  case H5T_COMPOUND:
    HL_SPEWDEBUG0("This is of type H5T_COMPOUND");
    nmembs = H5Tget_nmembers(type);
    memb = calloc(nmembs, sizeof(hid_t));
    name = calloc(nmembs, sizeof(char*));
    ndims = calloc(nmembs, sizeof(int));
    dims141 = calloc(nmembs * 4, sizeof(hsize_t));

    if (!memb || !name || !ndims || !dims141) {
      HL_ERROR0("Failed to allocate memory");
      goto done;
    }

    for (i = 0; i < nmembs; i++) {
      name[i] = NULL;
    }

    for (i = 0, size = 0; i < nmembs; i++) {
      f_memb = H5Tget_member_type(type, i);
      memb[i] = getFixedType(f_memb);

      if (memb[i] < 0) {
        HL_H5T_CLOSE(f_memb);
        goto done;
      }

      if (H5T_ARRAY == H5Tget_member_class(type, i)) {
        hsize_t tmpdims[4];
        H5Tget_array_dims2(f_memb, tmpdims);
        ndims[i] = H5Tget_array_ndims(f_memb);
        if (ndims[i] < 0 || ndims[i] > 4) {
          HL_ERROR0("Number of dims (the rank) not between 0-4");
          if (f_memb >= 0)
            H5Tclose(f_memb);
          goto done;
        }
        for (j = 0, nelmts = 1; j < ndims[i]; j++) {
          dims141[i * 4 + j] = tmpdims[j];
          nelmts *= tmpdims[j];
        }
      } else {
        nelmts = 1;
      }

      H5Tclose(f_memb);

      name[i] = H5Tget_member_name(type, i);
      if (name[i] == NULL) {
        goto done;
      }
      size = HLHDF_ALIGN(size, H5Tget_size(memb[i])) + nelmts * H5Tget_size(memb[i]);
    }

    mtype = H5Tcreate(H5T_COMPOUND, size);

    for (i = 0, offset = 0; i < nmembs; i++) {
      for (j = 0, nelmts = 1; j < ndims[i]; j++)
        nelmts *= dims141[i * 4 + j];

      if (H5T_ARRAY == H5Tget_member_class(type, i)) {
        f_memb = H5Tget_member_type(type, i);

        member_type = H5Tarray_create2(memb[i], ndims[i], dims141 + i * 4);
        H5Tinsert(mtype, name[i], offset, member_type);
        H5Tclose(member_type);
        offset = HLHDF_ALIGN(offset, H5Tget_size(memb[i])) + H5Tget_size(f_memb);
        if (f_memb >= 0)
          H5Tclose(f_memb);
      } else {
        H5Tinsert(mtype, name[i], offset, memb[i]);
        offset = HLHDF_ALIGN(offset, H5Tget_size(memb[i])) + nelmts
            * H5Tget_size(memb[i]);
      }
    }

    break;
  case H5T_ENUM:
    HL_SPEWDEBUG0("This is of type H5T_ENUM");
    break;
  case H5T_REFERENCE:
    HL_SPEWDEBUG0("This is of type H5T_REFERENCE");
    break;
  case H5T_OPAQUE:
    HL_SPEWDEBUG0("This is of type H5T_OPAQUE");
    mtype = H5Tcopy(type);
    break;
  case H5T_BITFIELD:
    HL_SPEWDEBUG0("This is of type H5T_BITFIELD");
    mtype = H5Tcopy(type);
    H5Tset_offset(mtype, 0);
    H5Tset_order(mtype, H5T_ORDER_LE);
    break;
  case H5T_TIME:
    HL_SPEWDEBUG0("This is of type H5T_TIME");
    break;
  case H5T_ARRAY:
    HL_SPEWDEBUG0("This is of type H5T_ARRAY");
    if ((mtype = H5Tget_super(type)) < 0) {
      HL_ERROR0("Failed getting the type id for the type that the array is made up of...");
      goto done;
    }
    tmpt = mtype;
    mtype = getFixedType(mtype);
    if (H5Tclose(tmpt) < 0) {
      HL_ERROR0("Failed closing super type");
    }
    break;

  default:
    HL_INFO0("HRRM, I don't recognize this datatype");
    break;
  }

done:
  for (i = 0; i < nmembs; i++) {
    if (memb != NULL) {
      HL_H5T_CLOSE(memb[i]);
    }
    if (name != NULL) {
      HLHDF_FREE(name[i]);
    }
  }
  HLHDF_FREE(memb);
  HLHDF_FREE(name);
  HLHDF_FREE(ndims);
  HLHDF_FREE(dims141);

  return mtype;
}

/************************************************
 * translateCharToDatatype
 ***********************************************/
hid_t translateCharToDatatype(const char* dataType)
{
  hid_t retv = -1;
  HL_DEBUG0("ENTER: translateCharToDatatype");

  if (!dataType) {
    HL_ERROR0("No type name specified");
    return retv;
  }
  if (strcmp(dataType, "char") == 0)
    retv = H5Tcopy(H5T_NATIVE_CHAR);
  else if (strcmp(dataType, "schar") == 0)
    retv = H5Tcopy(H5T_NATIVE_SCHAR);
  else if (strcmp(dataType, "uchar") == 0)
    retv = H5Tcopy(H5T_NATIVE_UCHAR);
  else if (strcmp(dataType, "short") == 0)
    retv = H5Tcopy(H5T_NATIVE_SHORT);
  else if (strcmp(dataType, "ushort") == 0)
    retv = H5Tcopy(H5T_NATIVE_USHORT);
  else if (strcmp(dataType, "int") == 0)
    retv = H5Tcopy(H5T_NATIVE_INT);
  else if (strcmp(dataType, "uint") == 0)
    retv = H5Tcopy(H5T_NATIVE_UINT);
  else if (strcmp(dataType, "long") == 0)
    retv = H5Tcopy(H5T_NATIVE_LONG);
  else if (strcmp(dataType, "ulong") == 0)
    retv = H5Tcopy(H5T_NATIVE_ULONG);
  else if (strcmp(dataType, "llong") == 0)
    retv = H5Tcopy(H5T_NATIVE_LLONG);
  else if (strcmp(dataType, "ullong") == 0)
    retv = H5Tcopy(H5T_NATIVE_ULLONG);
  else if (strcmp(dataType, "float") == 0)
    retv = H5Tcopy(H5T_NATIVE_FLOAT);
  else if (strcmp(dataType, "double") == 0)
    retv = H5Tcopy(H5T_NATIVE_DOUBLE);
  else if (strcmp(dataType, "ldouble") == 0)
    retv = H5Tcopy(H5T_NATIVE_LDOUBLE);
  else if (strcmp(dataType, "hsize") == 0)
    retv = H5Tcopy(H5T_NATIVE_HSIZE);
  else if (strcmp(dataType, "hssize") == 0)
    retv = H5Tcopy(H5T_NATIVE_HSSIZE);
  else if (strcmp(dataType, "herr") == 0)
    retv = H5Tcopy(H5T_NATIVE_HERR);
  else if (strcmp(dataType, "hbool") == 0)
    retv = H5Tcopy(H5T_NATIVE_HBOOL);

  if (retv == -1) {
    HL_ERROR1("There is no type called %s",dataType);
  }
  return retv;
}

/************************************************
 * getTypeNameString
 ***********************************************/
char* getTypeNameString(hid_t type)
{
  char* retv = NULL;
  HL_DEBUG0("ENTER: getTypeNameString");

  switch (H5Tget_class(type)) {
  case H5T_INTEGER:
    if (H5Tequal(type, H5T_STD_I8BE))
      retv = strdup("H5T_STD_I8BE");
    else if (H5Tequal(type, H5T_STD_I8LE))
      retv = strdup("H5T_STD_I8LE");
    else if (H5Tequal(type, H5T_STD_I16BE))
      retv = strdup("H5T_STD_I16BE");
    else if (H5Tequal(type, H5T_STD_I16LE))
      retv = strdup("H5T_STD_I16LE");
    else if (H5Tequal(type, H5T_STD_I32BE))
      retv = strdup("H5T_STD_I32BE");
    else if (H5Tequal(type, H5T_STD_I32LE))
      retv = strdup("H5T_STD_I32LE");
    else if (H5Tequal(type, H5T_STD_I64BE))
      retv = strdup("H5T_STD_I64BE");
    else if (H5Tequal(type, H5T_STD_I64LE))
      retv = strdup("H5T_STD_I64LE");
    else if (H5Tequal(type, H5T_STD_U8BE))
      retv = strdup("H5T_STD_U8BE");
    else if (H5Tequal(type, H5T_STD_U8LE))
      retv = strdup("H5T_STD_U8LE");
    else if (H5Tequal(type, H5T_STD_U16BE))
      retv = strdup("H5T_STD_U16BE");
    else if (H5Tequal(type, H5T_STD_U16LE))
      retv = strdup("H5T_STD_U16LE");
    else if (H5Tequal(type, H5T_STD_U32BE))
      retv = strdup("H5T_STD_U32BE");
    else if (H5Tequal(type, H5T_STD_U32LE))
      retv = strdup("H5T_STD_U32LE");
    else if (H5Tequal(type, H5T_STD_U64BE))
      retv = strdup("H5T_STD_U64BE");
    else if (H5Tequal(type, H5T_STD_U64LE))
      retv = strdup("H5T_STD_U64LE");
    else if (H5Tequal(type, H5T_NATIVE_SCHAR))
      retv = strdup("H5T_NATIVE_SCHAR");
    else if (H5Tequal(type, H5T_NATIVE_UCHAR))
      retv = strdup("H5T_NATIVE_UCHAR");
    else if (H5Tequal(type, H5T_NATIVE_SHORT))
      retv = strdup("H5T_NATIVE_SHORT");
    else if (H5Tequal(type, H5T_NATIVE_USHORT))
      retv = strdup("H5T_NATIVE_USHORT");
    else if (H5Tequal(type, H5T_NATIVE_INT))
      retv = strdup("H5T_NATIVE_INT");
    else if (H5Tequal(type, H5T_NATIVE_UINT))
      retv = strdup("H5T_NATIVE_UINT");
    else if (H5Tequal(type, H5T_NATIVE_LONG))
      retv = strdup("H5T_NATIVE_LONG");
    else if (H5Tequal(type, H5T_NATIVE_ULONG))
      retv = strdup("H5T_NATIVE_ULONG");
    else if (H5Tequal(type, H5T_NATIVE_LLONG))
      retv = strdup("H5T_NATIVE_LLONG");
    else if (H5Tequal(type, H5T_NATIVE_ULLONG))
      retv = strdup("H5T_NATIVE_ULLONG");
    else {
      HL_ERROR0("Undefined integer type");
    }
    break;
  case H5T_FLOAT:
    if (H5Tequal(type, H5T_IEEE_F32BE))
      retv = strdup("H5T_IEEE_F32BE");
    else if (H5Tequal(type, H5T_IEEE_F32LE))
      retv = strdup("H5T_IEEE_F32LE");
    else if (H5Tequal(type, H5T_IEEE_F64BE))
      retv = strdup("H5T_IEEE_F64BE");
    else if (H5Tequal(type, H5T_IEEE_F64LE))
      retv = strdup("H5T_IEEE_F64LE");
    else if (H5Tequal(type, H5T_NATIVE_FLOAT))
      retv = strdup("H5T_NATIVE_FLOAT");
    else if (H5Tequal(type, H5T_NATIVE_DOUBLE))
      retv = strdup("H5T_NATIVE_DOUBLE");
    else if (H5Tequal(type, H5T_NATIVE_LDOUBLE))
      retv = strdup("H5T_NATIVE_LDOUBLE");
    else {
      HL_ERROR0("Undefined float type");
    }
    break;
  case H5T_TIME:
    HL_INFO0("Unsupported type: H5T_TIME");
    break;
  case H5T_STRING:
    retv = strdup("H5T_STRING");
    break;
  case H5T_COMPOUND:
    retv = strdup("H5T_COMPOUND");
    break;
  case H5T_ARRAY:
    retv = strdup("H5T_ARRAY");
    break;
  default:
    HL_INFO0("Unsupported class");
    break;
  }

  return retv;
}

/************************************************
 * getFormatNameString
 ***********************************************/
char* getFormatNameString(hid_t type)
{
  char* retv = NULL;

  HL_SPEWDEBUG0("ENTER: getFormatNameString");

  if (H5Tget_class(type) == H5T_STRING) {
    retv = strdup("string");
    return retv;
  }

  if (H5Tequal(type, H5T_NATIVE_SCHAR))
    retv = strdup("schar");
  else if (H5Tequal(type, H5T_NATIVE_UCHAR))
    retv = strdup("uchar");
  else if (H5Tequal(type, H5T_NATIVE_CHAR))
    retv = strdup("char");
  else if (H5Tequal(type, H5T_NATIVE_SHORT))
    retv = strdup("short");
  else if (H5Tequal(type, H5T_NATIVE_USHORT))
    retv = strdup("ushort");
  else if (H5Tequal(type, H5T_NATIVE_INT))
    retv = strdup("int");
  else if (H5Tequal(type, H5T_NATIVE_UINT))
    retv = strdup("uint");
  else if (H5Tequal(type, H5T_NATIVE_LONG))
    retv = strdup("long");
  else if (H5Tequal(type, H5T_NATIVE_ULONG))
    retv = strdup("ulong");
  else if (H5Tequal(type, H5T_NATIVE_LLONG))
    retv = strdup("llong");
  else if (H5Tequal(type, H5T_NATIVE_ULLONG))
    retv = strdup("ullong");
  else if (H5Tequal(type, H5T_NATIVE_FLOAT))
    retv = strdup("float");
  else if (H5Tequal(type, H5T_NATIVE_DOUBLE))
    retv = strdup("double");
  else if (H5Tequal(type, H5T_NATIVE_LDOUBLE))
    retv = strdup("ldouble");
  else if (H5Tequal(type, H5T_NATIVE_HSIZE))
    retv = strdup("hsize");
  else if (H5Tequal(type, H5T_NATIVE_HSSIZE))
    retv = strdup("hssize");
  else if (H5Tequal(type, H5T_NATIVE_HERR))
    retv = strdup("herr");
  else if (H5Tequal(type, H5T_NATIVE_HBOOL))
    retv = strdup("hbool");
  else if (H5Tget_class(type) == H5T_COMPOUND)
    retv = strdup("compound");
  else if (H5Tget_class(type) == H5T_ARRAY)
    retv = strdup("array");
  else {
    HL_INFO0("Not possible to translate from given type to string");

  }
  return retv;
}

/************************************************
 * getStringPadName
 ***********************************************/
char* getStringPadName(hid_t type)
{
  H5T_str_t strpad = H5Tget_strpad(type);
  char* retv = NULL;
  HL_DEBUG0("ENTER: getStringPadName");

  if (strpad == H5T_STR_NULLTERM)
    retv = strdup("H5T_STR_NULLTERM");
  else if (strpad == H5T_STR_NULLPAD)
    retv = strdup("H5T_STR_NULLPAD");
  else if (strpad == H5T_STR_SPACEPAD)
    retv = strdup("H5T_STR_SPACEPAD");
  else
    retv = strdup("ILLEGAL STRPAD");

  return retv;
}

/************************************************
 * getStringCsetName
 ***********************************************/
char* getStringCsetName(hid_t type)
{
  H5T_cset_t cset = H5Tget_cset(type);
  char* retv = NULL;
  HL_DEBUG0("ENTER: getStringCsetName");
  if (cset == H5T_CSET_ASCII)
    retv = strdup("H5T_CSET_ASCII");
  else
    retv = strdup("UNKNOWN CHARACTER SET");

  return retv;
}

/************************************************
 * getStringCtypeName
 ***********************************************/
char* getStringCtypeName(hid_t type)
{
  hid_t strtype = H5Tcopy(H5T_C_S1);
  char* retv = NULL;
  H5T_cset_t cset = H5Tget_cset(type);
  H5T_str_t strpad = H5Tget_strpad(type);
  size_t size = H5Tget_size(type);
  H5Tset_cset(strtype, cset);
  H5Tset_size(strtype, size);
  H5Tset_strpad(strtype, strpad);

  HL_DEBUG0("ENTER: getStringCtypeName");
  if (H5Tequal(type, strtype)) {
    retv = strdup("H5T_C_S1");
    H5Tclose(strtype);
  } else {
    H5Tclose(strtype);
    strtype = H5Tcopy(H5T_FORTRAN_S1);
    H5Tset_cset(strtype, cset);
    H5Tset_size(strtype, size);
    H5Tset_strpad(strtype, strpad);
    if (H5Tequal(type, strtype))
      retv = strdup("H5T_FORTRAN_S1");
    else
      retv = strdup("UNKNOWN CHARACTER TYPE");
    HL_H5T_CLOSE(strtype);
  }
  return retv;
}

/**********************************************************
 *Function:	whatSizeIsHdfFormat
 **********************************************************/
int whatSizeIsHdfFormat(const char* format)
{
  hid_t tmpType;
  int size = -1;
  HL_DEBUG0("ENTER: whatSizeIsHdfFormat");
  tmpType = translateCharToDatatype(format);
  if (tmpType < 0) {
    HL_ERROR1("There is no type called %s",format);
    return -1;
  }
  size = H5Tget_size(tmpType);
  H5Tclose(tmpType);
  return size;
}

/**********************************************************
 *Function:	isFormatSupported
 **********************************************************/
int isFormatSupported(const char* format)
{
  hid_t tmpHid = -1;
  int retv = 1;
  HL_DEBUG0("ENTER: isFormatSupported");
  if ((tmpHid = translateCharToDatatype(format)) < 0) {
    retv = 0;
  }
  HL_H5T_CLOSE(tmpHid);
  return retv;
}

/**********************************************************
 *Function:	newHL_Compression
 **********************************************************/
HL_Compression* newHL_Compression(HL_CompressionType aType)
{
  HL_Compression* retv = NULL;
  HL_DEBUG0("ENTER: newHL_Compression");
  if (!(retv = (HL_Compression*) malloc(sizeof(HL_Compression)))) {
    HL_ERROR0("Failed to allocate memory for HL_Compression");
    return NULL;
  }
  initHL_Compression(retv, aType);
  return retv;
}

/**********************************************************
 *Function:	dupHL_Compression
 **********************************************************/
HL_Compression* dupHL_Compression(HL_Compression* inv)
{
  HL_Compression* retv = NULL;
  HL_DEBUG0("ENTER: dupHL_Compression");
  if (!inv) {
    return NULL;
  }
  if (!(retv = (HL_Compression*) malloc(sizeof(HL_Compression)))) {
    HL_ERROR0("Failed to allocate memory for HL_Compression");
    return NULL;
  }
  retv->type = inv->type;
  retv->level = inv->level;
  retv->szlib_mask = inv->szlib_mask;
  retv->szlib_px_per_block = inv->szlib_px_per_block;
  return retv;
}

/**********************************************************
 *Function:	initHL_Compression
 **********************************************************/
void initHL_Compression(HL_Compression* inv, HL_CompressionType aType)
{
  HL_DEBUG0("ENTER: initHL_Compression");
  if (!inv) {
    HL_ERROR0("Trying to initialize NULL");
    return;
  }
  inv->type = aType;
  inv->level = 6;
  inv->szlib_mask = H5_SZIP_ALLOW_K13_OPTION_MASK | H5_SZIP_EC_OPTION_MASK;
  inv->szlib_px_per_block = 16;
}

/**********************************************************
 *Function:	freeHL_Compression
 **********************************************************/
void freeHL_Compression(HL_Compression* inv)
{
  HL_SPEWDEBUG0("ENTER: freeHL_Compression");
  HLHDF_FREE(inv);
}

int extractParentChildName(HL_Node* node, char* parent, char* child)
{
   char tmpStr[256];
   char* tmpPtr;
   HL_SPEWDEBUG0("ENTER: extractParentChildName");
   if(!parent || !child) {
      HL_ERROR0("When extracting ParentChild name, both parent and child must be specified");
      return 0;
   }

   strcpy(parent,"");
   strcpy(child,"");

   strcpy(tmpStr,node->name);
   if(!(tmpPtr=strrchr(tmpStr,'/'))) {
      HL_ERROR1("Could not extract '/' from node name %s",node->name);
      return 0;
   }
   tmpPtr[0]='\0';
   if(strcmp(tmpStr,"")!=0) {
      strcpy(parent,tmpStr);
   }
   tmpPtr++;
   strcpy(child,tmpPtr);
   return 1;
}
