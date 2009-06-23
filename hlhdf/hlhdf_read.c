/**
 * Functions for working with HL_Node's and HL_NodeList's.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-12
 */
#include "hlhdf.h"
#include "hlhdf_private.h"
#include "hlhdf_debug.h"
#include "hlhdf_defines_private.h"
#include <string.h>
#include <stdlib.h>

/*@{ Typedefs */

/**
 * Struct used internally for beeing able to do a reverse name lookup when
 * reading a file with references. We dont want to have the data in the object
 * referenced to, but instead we want to have the name, thats why this
 * has to be used.
 */
typedef struct ReferenceLookup
{
  hid_t file_id;                /**< the file id identifier */
  hobj_ref_t* ref;              /**< the reference */
  char tmp_name[512];           /**< tmp name used for generating paths */
  char found_name[512];         /**< the found name */
  int reffound;                 /**< if the reference has been found */
  struct ReferenceLookup* next; /**< next reference */
} ReferenceLookup;

/**
 * Used when traversing over the different nodes during reading.
 */
typedef struct VisitorStruct {
  char* path; /**< the root path initiating the visitor */
  HL_NodeList* nodelist; /**< the nodelist where to add nodes */
} VisitorStruct;

/*@} End of Typedefs */

/*@{ Private functions */
static HL_CompoundTypeDescription* buildTypeDescriptionFromTypeHid(hid_t type_id)
{
  HL_CompoundTypeDescription* typelist = NULL;
  HL_CompoundTypeAttribute* typenode;
  int i, j, nmembers;
  char* fname = NULL;
  hid_t mtype = -1;
  hid_t fixedType = -1;
  size_t* dims = NULL;
  hsize_t* dims_h514 = NULL;
  size_t dSize;
  int ndims;
  char* tmpName = NULL;
  HL_DEBUG0("ENTER: buildTypeDescriptionFromTypeHid");
  if (!(typelist = newHL_CompoundTypeDescription())) {
    HL_ERROR0("Failed to create datatype nodelist");
    goto fail;
  }

  typelist->size = H5Tget_size(type_id);
  nmembers = H5Tget_nmembers(type_id);
  for (i = 0; i < nmembers; i++) {
    size_t offset = H5Tget_member_offset(type_id, i);
    fname = H5Tget_member_name(type_id, i);
    mtype = H5Tget_member_type(type_id, i);

    if ((fixedType = getFixedType(mtype)) < 0) {
      HL_ERROR0("Failed to convert to fixed type");
      goto fail;
    }

    if (!(tmpName = getFormatNameString(fixedType))) {
      goto fail;
    }

    if (H5T_ARRAY == H5Tget_member_class(type_id, i)) {
      ndims = H5Tget_array_ndims(mtype);
      dims_h514 = (hsize_t*) malloc(sizeof(hsize_t) * ndims);
      if (H5Tget_array_dims(mtype, dims_h514) < 0) {
        HL_ERROR0("Failed to retrieve array dimensions");
      }
      dims = (size_t*) malloc(sizeof(size_t) * ndims);
      for (j = 0; j < ndims; j++) {
        dims[j] = (size_t) dims_h514[j];
      }
    } else {
      ndims = 1;
      dims = (size_t*) malloc(sizeof(size_t) * ndims);
      dims[0] = 1;
    }
    dSize = H5Tget_size(fixedType);

    if (fname == NULL) {
      HL_ERROR0("fname is NULL, cant use it to create CompoundTypeAttribute\n");
      goto fail;
    }
    typenode = newHL_CompoundTypeAttribute(fname, offset, tmpName, dSize, ndims, dims);
    if (!addHL_CompoundTypeAttribute(typelist, typenode))
      goto fail;
    HLHDF_FREE(dims);
    HLHDF_FREE(dims_h514);
    HL_H5T_CLOSE(mtype);
    HLHDF_FREE(fname);
    HLHDF_FREE(tmpName);
    HL_H5T_CLOSE(fixedType);
  }

  HLHDF_FREE(dims_h514);
  HLHDF_FREE(dims);
  HL_H5T_CLOSE(mtype);
  HL_H5T_CLOSE(fixedType);
  HLHDF_FREE(fname);
  HLHDF_FREE(tmpName);
  return typelist;
fail:
  HLHDF_FREE(dims);
  HLHDF_FREE(dims_h514);
  HL_H5T_CLOSE(mtype);
  HL_H5T_CLOSE(fixedType);
  HLHDF_FREE(fname);
  HLHDF_FREE(tmpName);
  freeHL_CompoundTypeDescription(typelist);
  return NULL;
}

static int checkIfReferenceMatch(hid_t loc_id, char* path, hobj_ref_t* ref)
{
  hobj_ref_t matchref;
  herr_t status;
  HL_DEBUG0("ENTER:  checkIfReferenceMatch");
  status = H5Rcreate(&matchref, loc_id, path, H5R_OBJECT, -1);
  if (status < 0) {
    HL_ERROR1("Could not create reference to '%s'",path);
    return 0;
  }
  if (memcmp(ref, &matchref, sizeof(hobj_ref_t)) == 0) {
    return 1;
  }
  return 0;
}

/**
 * Opens a group or dataset hid and returns the type as well.
 * @param[in] file_id - the file pointer
 * @param[in] name - the node name to open
 * @param[out] lid - the opened hid
 * @param[out] type - the type of the opened hid
 * @return 0 on failure, otherwise 1
 */
static int openGroupOrDataset(hid_t file_id, const char* name, hid_t* lid, HL_Type* type)
{
  int status = 0;

  if (name == NULL || lid == NULL || type == NULL) {
    HL_ERROR0("Inparameters NULL");
    goto fail;
  }
  *lid = -1;
  *type = UNDEFINED_ID;
  if (strcmp(name, "") != 0) {
    H5O_info_t objectInfo;
    herr_t infoStatus = -1;
    disableErrorReporting(); /*Bypass the error reporting, if failed to open a dataset/or group*/
    infoStatus = H5Oget_info_by_name(file_id, name, &objectInfo, H5P_DEFAULT);
    enableErrorReporting();
    if (infoStatus >= 0) {
      if (objectInfo.type == H5O_TYPE_GROUP) {
        *type = GROUP_ID;
      } else if (objectInfo.type == H5O_TYPE_DATASET) {
        *type = DATASET_ID;
      } else {
        infoStatus = -1;
        *type = UNDEFINED_ID;
      }
    }
    if (infoStatus < 0) {
      HL_ERROR0("name needs to be a dataset or group.");
      goto fail;
    }
    if ((*lid = H5Oopen(file_id, name, H5P_DEFAULT))<0) {
      HL_ERROR1("Node '%s' could not be opened", name);
      goto fail;
    }
  } else {
    if ((*lid = H5Gopen(file_id, "/", H5P_DEFAULT)) < 0) {
      HL_ERROR0("Could not open root group");
      goto fail;
    }
    *type = GROUP_ID;
  }
  status = 1;
fail:
  if (status == 0) {
    HL_H5O_CLOSE(*lid);
    *type = UNDEFINED_ID;
  }
  return status;
}
/* ---------------------------------------
 * REF_GROUP_LOCATION_ITERATOR
 * Iterator function for checking group
 * references.
 * ---------------------------------------*/
static herr_t refGroupLocationIterator(hid_t gid, const char* name,
  void* op_data)
{
  ReferenceLookup* lookup = (ReferenceLookup*) op_data;
  char tmp1[512], tmp2[512];
  H5G_stat_t statbuf;
  hid_t obj = -1;
  HL_DEBUG0("ENTER: refGroupLocationIterator");
  H5Gget_objinfo(gid, name, 0, &statbuf);

  strcpy(tmp1, lookup->tmp_name);

  switch (statbuf.type) {
  case H5G_GROUP:
    if ((obj = H5Gopen(gid, name, H5P_DEFAULT)) >= 0) {
      sprintf(tmp2, "%s/%s", lookup->tmp_name, name);
      strcpy(lookup->tmp_name, tmp2);
      if (checkIfReferenceMatch(lookup->file_id, lookup->tmp_name, lookup->ref) == 1) {
        strcpy(lookup->found_name, lookup->tmp_name);
        lookup->reffound = 1;
      }
      H5Giterate(obj, ".", NULL, refGroupLocationIterator, lookup);
      strcpy(lookup->tmp_name, tmp1);
      HL_H5G_CLOSE(obj);
    }
    break;
  case H5G_DATASET:
    if ((obj = H5Dopen(gid, name, H5P_DEFAULT)) >= 0) {
      sprintf(tmp2, "%s/%s", lookup->tmp_name, name);
      strcpy(lookup->tmp_name, tmp2);
      if (checkIfReferenceMatch(lookup->file_id, lookup->tmp_name, lookup->ref)
          == 1) {
        strcpy(lookup->found_name, lookup->tmp_name);
        lookup->reffound = 1;
      }
      /*H5Aiterate(obj,NULL,refAttributeLocationIterator,lookup);*/
      strcpy(lookup->tmp_name, tmp1);
      HL_H5D_CLOSE(obj);
    }
    break;
  case H5G_TYPE:
    sprintf(tmp2, "%s/%s", lookup->tmp_name, name);
    strcpy(lookup->tmp_name, tmp2);
    if (checkIfReferenceMatch(lookup->file_id, lookup->tmp_name, lookup->ref) == 1) {
      strcpy(lookup->found_name, lookup->tmp_name);
      lookup->reffound = 1;
    }
    strcpy(lookup->tmp_name, tmp1);
    break;
  default:
    HL_ERROR1("Undefined type for %s",name);
    HL_ERROR1("Name: %s",tmp1);
    HL_ERROR1("Type id %d",statbuf.type);
    break;
  }
  return 0;
}

/* ---------------------------------------
 * LOCATE_NAME_FOR_REFERENCE
 * --------------------------------------- */
static char* locateNameForReference(hid_t file_id, hobj_ref_t* ref)
{
  hid_t gid = -1;
  ReferenceLookup lookup;

  HL_DEBUG0("ENTER: locateNameForReference");

  lookup.file_id = file_id;
  lookup.ref = ref;
  strcpy(lookup.tmp_name, "");
  strcpy(lookup.found_name, "");
  lookup.reffound = 0;

  if ((gid = H5Gopen(file_id, ".", H5P_DEFAULT)) < 0) {
    HL_ERROR0("Failed to open root group");
    goto fail;
  }

  H5Giterate(file_id, ".", NULL, refGroupLocationIterator, &lookup);
  HL_H5G_CLOSE(gid);

  if (lookup.reffound == 0)
    goto fail;

  return strdup(lookup.found_name);
fail:
  return NULL;
}


/**
 * Fills an attribute with data
 */
static int fillAttributeNode(hid_t file_id, HL_Node* node)
{
  hid_t obj = -1;
  hid_t loc_id = -1;
  hid_t type = -1, mtype = -1;
  hid_t f_space = -1;
  hsize_t* all_dims = NULL;
  hsize_t npoints;
  int ndims, i;
  char* parent = NULL;
  char* child = NULL;
  unsigned char* dataptr = NULL;
  unsigned char* rawdataptr = NULL;
  char* tmpName = NULL;
  HL_Type parentType = UNDEFINED_ID;
  H5G_stat_t statbuf;
  int result = 0;

  HL_SPEWDEBUG0("ENTER: fillAttributeNode");

  if (!extractParentChildName(node, &parent, &child)) {
    HL_ERROR0("Failed to extract parent/child");
    goto fail;
  }

  if (!openGroupOrDataset(file_id, parent, &loc_id, &parentType)) {
    HL_ERROR1("Failed to determine and open '%s'", parent);
    goto fail;
  }

  if ((obj = H5Aopen_name(loc_id, child)) < 0) {
    goto fail;
  }

  if ((type = H5Aget_type(obj)) < 0) {
    HL_ERROR0("Could not get attribute type");
    goto fail;
  }

  if ((mtype = getFixedType(type)) < 0) {
    HL_ERROR0("Could not create fixed attribute type");
    goto fail;
  }

  if (H5Tget_class(mtype) == H5T_COMPOUND) {
    if (H5Tcommitted(type) > 0) {
      H5Gget_objinfo(type, ".", TRUE, &statbuf);
    }
    if (!(node->compoundDescription = buildTypeDescriptionFromTypeHid(mtype))) {
      HL_ERROR0("Failed to create compound data description for attribute");
      goto fail;
    }
    if (H5Tcommitted(type) > 0) {
      node->compoundDescription->objno[0] = statbuf.objno[0];
      node->compoundDescription->objno[1] = statbuf.objno[1];
    }
  }

  if ((f_space = H5Aget_space(obj)) >= 0) {
    ndims = H5Sget_simple_extent_ndims(f_space);
    npoints = H5Sget_simple_extent_npoints(f_space);
    if (ndims > 0) {
      if (!(all_dims = (hsize_t*) malloc(sizeof(hsize_t) * ndims))) {
        HL_ERROR0("Could not allocate memory for node dims\n");
        goto fail;
      }
    }

    H5Sget_simple_extent_dims(f_space, all_dims, NULL);
    if (H5Sis_simple(f_space) >= 0) {
      node->dSize = H5Tget_size(mtype);
      if (!(dataptr = (unsigned char*) malloc(H5Tget_size(mtype) * npoints))) {
        HL_ERROR0("Could not allocate memory for attribute data");
        goto fail;
      }
      if (H5Aread(obj, mtype, dataptr) < 0) {
        HL_ERROR0("Could not read attribute data\n");
        goto fail;
      }

      /* Ahe added this 2001-01-17 */
      node->rdSize = H5Tget_size(type);
      if (!(rawdataptr = (unsigned char*) malloc(H5Tget_size(type) * npoints))) {
        HL_ERROR0("Could not allocate memory for raw attribute data");
        goto fail;
      }
      if (H5Aread(obj, type, rawdataptr) < 0) {
        HL_ERROR0("Could not read raw attribute data");
        goto fail;
      }
    } else {
      HL_ERROR0("Attribute dataspace was not simple, can't handle");
      goto fail;
    }
  } else {
    HL_ERROR0("Could not get dataspace for attribute");
    goto fail;
  }

  if (!(tmpName = getFormatNameString(mtype))) {
    goto fail;
  }
  strcpy(node->format, tmpName);

  if (all_dims) {
    for (i = 0; i < ndims; i++)
      node->dims[i] = all_dims[i];
  }
  node->ndims = ndims;
  node->typeId = H5Tcopy(mtype);
  node->mark = NMARK_ORIGINAL;
  node->data = dataptr;
  node->rawdata = rawdataptr; /* Ahe added this 2001-01-17 */
  dataptr = NULL; // Responsibility for memory transfered
  rawdataptr = NULL; // Responsibility for memory transfered

  result = 1;
fail:
  HL_H5A_CLOSE(obj);
  HL_H5O_CLOSE(loc_id);
  HL_H5T_CLOSE(type);
  HL_H5T_CLOSE(mtype);
  HL_H5S_CLOSE(f_space);
  HLHDF_FREE(all_dims);
  HLHDF_FREE(parent);
  HLHDF_FREE(child);
  HLHDF_FREE(dataptr);
  HLHDF_FREE(rawdataptr);
  HLHDF_FREE(tmpName);

  return result;
}

/**
 * Fills a reference node
 */
static int fillReferenceNode(hid_t file_id, HL_Node* node)
{
  HL_Type parentType = UNDEFINED_ID;
  hobj_ref_t ref;
  hid_t obj = -1;
  hid_t loc_id = -1;
  char* parent = NULL;
  char* child = NULL;
  char* refername = NULL;
  char* tmpName = NULL;
  int status = 0;

  HL_DEBUG0("ENTER: fillReferenceNode");
  if (!extractParentChildName(node, &parent, &child)) {
    HL_ERROR0("Failed to extract parent/child");
    goto fail;
  }

  if (!openGroupOrDataset(file_id, parent, &loc_id, &parentType)) {
    HL_ERROR1("Failed to determine and open '%s'", parent);
    goto fail;
  }

  if ((obj = H5Aopen_name(loc_id, child)) < 0) {
    goto fail;
  }
  if (H5Aread(obj, H5T_STD_REF_OBJ, &ref) < 0) {
    HL_ERROR0("Could not read reference\n");
    goto fail;
  }

  if (!(refername = locateNameForReference(file_id, &ref))) {
    HL_ERROR2("WARNING: Could not locate name of object referenced by: %s/%s"
        " will set referenced object to UNKNOWN.", parent,child);
    refername = strdup("UNKNOWN");
  }
  node->dSize = strlen(refername) + 1;
  node->data = (unsigned char*) strdup(refername);
  node->rdSize = strlen(refername) + 1;
  node->rawdata = (unsigned char*) strdup(refername);
  node->ndims = 0;
  node->mark = NMARK_ORIGINAL;
  node->typeId = H5Tcopy(H5T_C_S1);
  H5Tset_size(node->typeId, node->dSize);
  if (!(tmpName = getFormatNameString(node->typeId))) {
    HL_ERROR0("Failed to get name string");
    goto fail;
  }
  strcpy(node->format, tmpName);

  status = 1;
fail:
  HL_H5A_CLOSE(obj);
  HL_H5O_CLOSE(loc_id);
  HLHDF_FREE(parent);
  HLHDF_FREE(child);
  HLHDF_FREE(refername);
  HLHDF_FREE(tmpName);

  return status;
}

/**
 * Fills a dataset node
 */
static int fillDatasetNode(hid_t file_id, HL_Node* node)
{
  hid_t obj = -1;
  hid_t type = -1;
  H5G_stat_t statbuf;
  int ndims, i;
  hsize_t* all_dims = NULL;
  hsize_t npoints;
  hid_t f_space = -1;
  hid_t mtype = -1;
  unsigned char* dataptr = NULL;
  char* tmpName = NULL;

  HL_DEBUG0("ENTER: fillDatasetNode");

  if ((obj = H5Dopen(file_id, node->name, H5P_DEFAULT)) < 0) {
    goto fail;
  }

  /* What datatype was this dataset stored as? */
  if ((type = H5Dget_type(obj)) < 0) {
    HL_ERROR0("Failed to get type from dataset");
    goto fail;
  }

  /* What size does the type have? */
  if ((f_space = H5Dget_space(obj)) > 0) { /*Get the space description for the dataset */
    ndims = H5Sget_simple_extent_ndims(f_space);
    npoints = H5Sget_simple_extent_npoints(f_space);
    all_dims = (hsize_t*) malloc(sizeof(hsize_t) * ndims);
    if (H5Sget_simple_extent_dims(f_space, all_dims, NULL) != ndims) {
      HL_ERROR0("Could not get dimensions from dataset");
      goto fail;
    }
    /* Translate the type into a native dataspace */
    mtype = getFixedType(type);
    /*mtype = H5Tcopy(type);*/

    if (H5Tget_class(mtype) == H5T_COMPOUND) {
      if (H5Tcommitted(type) > 0) {
        H5Gget_objinfo(type, ".", TRUE, &statbuf);
      }
      if (!(node->compoundDescription = buildTypeDescriptionFromTypeHid(mtype))) {
        HL_ERROR0("Failed to create compound data description for dataset");
        goto fail;
      }
      if (H5Tcommitted(type) > 0) {
        node->compoundDescription->objno[0] = statbuf.objno[0];
        node->compoundDescription->objno[1] = statbuf.objno[1];
      }
    }

    if (H5Sis_simple(f_space) >= 0) { /*Only allow simple dataspace, nothing else supported by HDF5 anyway */
      node->dSize = H5Tget_size(mtype);
      node->rdSize = 0;
      dataptr = (unsigned char*) malloc(H5Tget_size(mtype) * npoints);
      H5Sselect_all(f_space); /* Mark for selection */
      if (H5Dread(obj, mtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataptr) < 0) {
        HL_ERROR0("Failed to read dataset");
        goto fail;
      }
    } else {
      HL_ERROR0("Dataspace for dataset was not simple, this is not supported");
      goto fail;
    }
  } else {
    HL_ERROR0("Failure getting space description for dataset");
    goto fail;
  }

  /* Translate the type into a recognizable string, e.g. int, short, string, compound */
  if (!(tmpName = getFormatNameString(mtype))) {
    goto fail;
  }
  strcpy(node->format, tmpName);

  /* Filling dataset */
  node->ndims = ndims;
  for (i = 0; i < ndims; i++) {
    node->dims[i] = all_dims[i];
  }

  /* Remember to keep the type pointer, can be hard to interpreet the data
   * later otherwise. */
  node->typeId = mtype;

  /*Better keep data also*/
  node->data = (unsigned char*) dataptr;

  /* Mark the node as original */
  node->mark = NMARK_ORIGINAL;

  HL_H5D_CLOSE(obj);
  HL_H5T_CLOSE(type);
  HLHDF_FREE(all_dims);
  HL_H5S_CLOSE(f_space);
  HLHDF_FREE(tmpName);
  return 1;
fail:
  HL_H5D_CLOSE(obj);
  HL_H5T_CLOSE(type);
  HLHDF_FREE(all_dims);
  HL_H5S_CLOSE(f_space);
  HL_H5T_CLOSE(mtype);
  HLHDF_FREE(dataptr);
  HLHDF_FREE(tmpName);
  return 0;
}

/**
 * Fills a group node
 */
static int fillGroupNode(hid_t file_id, HL_Node* node)
{
  hid_t obj;
  HL_SPEWDEBUG0("ENTER: fillGroupNode");
  if ((obj = H5Gopen(file_id, node->name, H5P_DEFAULT)) < 0) {
    return 0;
  }

  /* Mark the node as original */
  node->mark = NMARK_ORIGINAL;

  HL_H5G_CLOSE(obj);
  return 1;
}

/* ---------------------------------------
 * FILL_TYPE_NODE
 * --------------------------------------- */
static int fillTypeNode(hid_t file_id, HL_Node* node)
{
  hid_t obj = -1;
  HL_CompoundTypeDescription* typelist = NULL;
  H5G_stat_t statbuf;
  HL_SPEWDEBUG0("ENTER: fillTypeNode");
  if ((obj = H5Topen(file_id, node->name, H5P_DEFAULT)) < 0) {
    HL_ERROR1("Failed to open %s ", node->name);
    return 0;
  }
  H5Gget_objinfo(obj, ".", TRUE, &statbuf);

  if (!(typelist = buildTypeDescriptionFromTypeHid(obj))) {
    HL_ERROR0("Failed to create datatype nodelist");
    goto fail;
  }
  strcpy(typelist->hltypename, node->name);
  typelist->objno[0] = statbuf.objno[0];
  typelist->objno[1] = statbuf.objno[1];

  node->compoundDescription = typelist;

  /* Mark the node as original */
  node->mark = NMARK_ORIGINAL;

  node->hdfId = obj; /*Save hdfid for later use, which means that obj not should be closed now. */
  //@todo This causes the file not to be closed when atempting to update a file.
  return 1;
fail:
  HL_H5T_CLOSE(obj);
  freeHL_CompoundTypeDescription(typelist);
  return 0;
}

/**
 * Fills the node with the appropriate data.
 */
static int fillNodeWithData(hid_t file_id, HL_Node* node)
{
  HL_SPEWDEBUG0("ENTER: fillNodeWithData");
  switch (node->type) {
  case ATTRIBUTE_ID:
    return fillAttributeNode(file_id, node);
  case DATASET_ID:
    return fillDatasetNode(file_id, node);
  case GROUP_ID:
    return fillGroupNode(file_id, node);
  case TYPE_ID:
    return fillTypeNode(file_id, node);
  case REFERENCE_ID:
    return fillReferenceNode(file_id, node);
  default:
    HL_ERROR1("Can't handle other nodetypes but '%d'",node->type);
    break;
  }
  return 0;
}

/**
 * Creates an absolute path from <b>root</b> and <b>name</b> parts.
 * @param[in] root - the root path
 * @param[in] name - the name
 * @return a path in the form <b>root/name</b>
 */
static char* hlhdf_read_createPath(const char* root, const char* name)
{
  char* newpath = NULL;
  int status = FALSE;
  int len = 0;

  if (root == NULL || name == NULL) {
    HL_ERROR0("hlhdf_read_createPath: arguments NULL");
    goto fail;
  }

  newpath = malloc(sizeof(char)*(strlen(root) + strlen(name) + 2));
  if (newpath == NULL) {
    HL_ERROR0("Failed to allocate memory\n");
    goto fail;
  }

  if (strcmp(".", root) == 0 || strcmp("/", root) == 0) {
    strcpy(newpath, "");
  } else {
    int i = 0;
    sprintf(newpath, "%s", root);
    i = strlen(newpath) - 1;
    while (i>1 && newpath[i] == '/') {
      newpath[i] = '\0';
      i--;
    }
  }

  if (strcmp(name, ".") == 0) {
    int i = strlen(newpath);
    sprintf(&newpath[i], "/");
  } else {
    int i = strlen(newpath);
    sprintf(&newpath[i], "/%s", name);
  }

  len = strlen(newpath);
  if (len > 1 && newpath[len-1]=='/') {
    newpath[len-1]='\0';
  }

  /*fprintf(stderr, "root='%s', name='%s' => newpath = '%s'\n", root, name, newpath);*/

  status = TRUE;
fail:
  if (status == FALSE) {
    HLHDF_FREE(newpath);
  }
  return newpath;
}

/**
 * Called by H5Aiterate_by_name when iterating over all attributes in a group.
 * @param[in] location_id - the root group from where the iterator started
 * @param[in] name - the name of the attribute
 * @param[in] ainfo - the attribute info
 * @param[in] op_data - the \ref VisitorStruct
 * @return -1 on failure, otherwise 0.
 */
static herr_t hlhdf_node_attribute_visitor(hid_t location_id, const char *name, const H5A_info_t *ainfo, void *op_data)
{
  VisitorStruct* vsp = (VisitorStruct*)op_data;
  herr_t status = -1;
  char* path = hlhdf_read_createPath(vsp->path, name);
  hid_t attrid = -1;
  hid_t typeid = -1;

  if (path == NULL) {
    HL_ERROR0("Could not create path");
    goto fail;
  }

  if ((attrid = H5Aopen(location_id, name, H5P_DEFAULT))<0) {
    HL_ERROR1("Could not open attribute: %s", name);
    goto fail;
  }

  if ((typeid = H5Aget_type(attrid)) < 0) {
    HL_ERROR1("Could not get type for %s", name);
    goto fail;
  }

  if (H5Tget_class(typeid) == H5T_REFERENCE) {
    addHL_Node(vsp->nodelist, newHL_Reference(path));
  } else {
    addHL_Node(vsp->nodelist, newHL_Attribute(path));
  }

  status = 0;
fail:
  HL_H5A_CLOSE(attrid);
  HL_H5T_CLOSE(typeid);
  HLHDF_FREE(path);
  return status;
}

/**
 * Called by H5Ovisit_by_name when iterating over all group/dataasets.
 * @param[in] location_id - the root group from where the iterator started
 * @param[in] name - the name of the attribute
 * @param[in] info - the object info
 * @param[in] op_data - the \ref VisitorStruct
 * @return -1 on failure, otherwise 0.
 */
static herr_t hlhdf_node_visitor(hid_t g_id, const char *name, const H5O_info_t *info, void *op_data)
{
  VisitorStruct* vsp = (VisitorStruct*)op_data;
  VisitorStruct vs;
  herr_t status = -1;
  char* path = hlhdf_read_createPath(vsp->path, name);

  if (path == NULL) {
    HL_ERROR0("Could not create path");
    goto fail;
  }

  vs.nodelist = vsp->nodelist;
  vs.path = path;
  switch (info->type) {
  case H5O_TYPE_GROUP: {
    hsize_t n=0;
    addHL_Node(vsp->nodelist, newHL_Group(vs.path));
    if (H5Aiterate_by_name(g_id, name, H5_INDEX_NAME, H5_ITER_INC, &n, hlhdf_node_attribute_visitor, &vs, H5P_DEFAULT) < 0) {
      HL_ERROR1("Failed to iterate over %s", vs.path);
      goto fail;
    }
    break;
  }
  case H5O_TYPE_DATASET: {
    hsize_t n=0;
    addHL_Node(vsp->nodelist, newHL_Dataset(vs.path));
    if (H5Aiterate_by_name(g_id,  name, H5_INDEX_NAME, H5_ITER_INC, &n, hlhdf_node_attribute_visitor, &vs, H5P_DEFAULT) < 0) {
      HL_ERROR1("Failed to iterate over %s", vs.path);
      goto fail;
    }
    break;
  }
  case H5O_TYPE_NAMED_DATATYPE: {
    addHL_Node(vsp->nodelist, newHL_Datatype(vs.path));
    break;
  }
  default: {
    fprintf(stderr, "(%d) UNKNOWN: %s\n", g_id, name);
    break;
  }
  }
  status = 0;
fail:
  HLHDF_FREE(path);
  return status;
}

/*@} End of Private functions */

/*@{ Interface functions */
HL_NodeList* readHL_NodeListFrom(const char* filename, const char* fromPath)
{
  hid_t file_id = -1, gid = -1;
  HL_NodeList* retv = NULL;
  VisitorStruct vs;
  H5O_info_t objectInfo;

  HL_DEBUG0("ENTER: readHL_NodeListFrom");

  if (fromPath == NULL) {
    HL_ERROR0("fromPath == NULL");
    goto fail;
  }

  if ((file_id = openHlHdfFile(filename, "r")) < 0) {
    HL_ERROR1("Failed to open file %s",filename);
    goto fail;
  }

  disableErrorReporting();
  if (H5Oget_info_by_name(file_id, fromPath, &objectInfo, H5P_DEFAULT)<0) {
    HL_ERROR0("fromPath needs to be a dataset or group when opening a file.");
    goto fail;
  }
  enableErrorReporting();

  if (!(retv = newHL_NodeList())) {
    HL_ERROR0("Could not allocate NodeList\n");
    goto fail;
  }
  setHL_NodeListFileName(retv, filename);
  //strcpy(retv->filename, filename);

  vs.path = (char*)fromPath;
  vs.nodelist = retv;

  if (H5Ovisit_by_name(file_id, fromPath, H5_INDEX_NAME, H5_ITER_INC, hlhdf_node_visitor, &vs, H5P_DEFAULT)<0) {
    HL_ERROR0("Could not iterate over file");
    goto fail;
  }

  markHL_NodeListNodes(retv, NMARK_ORIGINAL);

  HL_H5F_CLOSE(file_id);
  HL_H5G_CLOSE(gid);
  HL_DEBUG0("EXIT: readHL_NodeListFrom ");
  return retv;

fail:
  HL_H5F_CLOSE(file_id);
  HL_H5G_CLOSE(gid);
  freeHL_NodeList(retv);
  HL_DEBUG0("EXIT: readHL_NodeListFrom with Error");
  return NULL;
}

/* ---------------------------------------
 * READ_HL_NODE_LIST
 * --------------------------------------- */
HL_NodeList* readHL_NodeList(const char* filename)
{
  HL_NodeList* retv = NULL;
  HL_DEBUG0("ENTER: readHL_NodeList");

  retv = readHL_NodeListFrom(filename, ".");

  HL_DEBUG0("EXIT: readHL_NodeList");
  return retv;
}

/* ---------------------------------------
 * SELECT_NODE
 * --------------------------------------- */
int selectNode(HL_NodeList* nodelist, const char* name)
{
  HL_Node* node = NULL;

  HL_DEBUG0("ENTER: selectNode");
  if (!name) {
    HL_ERROR0("Can not select any node when name is NULL");
    return 0;
  }

  if ((node = getHL_Node(nodelist, name)) != NULL) {
    node->mark = NMARK_SELECT;
    return 1;
  }

  HL_ERROR1("Could not find any node called '%s'",name);
  return 0;
}

/* ---------------------------------------
 * SELECT_ALL_NODES
 * --------------------------------------- */
int selectAllNodes(HL_NodeList* nodelist)
{
  markHL_NodeListNodes(nodelist, NMARK_SELECT);

  return 1;
}

/* ---------------------------------------
 * SELECT_METADATA_NODES
 * VOLATILE: Do not attempt to access dataset arrays after calling this.
 * --------------------------------------- */
int selectMetadataNodes(HL_NodeList* nodelist)
{
  int i = 0;
  int nNodes = -1;
  HL_DEBUG0("ENTER: selectMetadataNodes");
  nNodes = getHL_NodeListNumberOfNodes(nodelist);
  for (i = 0; i < nNodes; i++) {
    HL_Node* node = getHL_NodeListNodeByIndex(nodelist, i);
    if (node->type != DATASET_ID && node->dataType != HL_ARRAY) {
      setHL_NodeMark(node, NMARK_SELECT);
    }
  }

  return 1;
}

/* ---------------------------------------
 * DE-SELECT_NODE
 * --------------------------------------- */
int deselectNode(HL_NodeList* nodelist, const char* name)
{
  HL_Node* node = NULL;

  HL_DEBUG0("ENTER: deselectNode");
  if (!name) {
    HL_ERROR0("Can not deselect any node when name is NULL");
    return 0;
  }

  node = getHL_Node(nodelist, name);
  if (node != NULL) {
    setHL_NodeMark(node, NMARK_ORIGINAL);
    return 1;
  }

  HL_ERROR1("Could not find any node called '%s'",name);
  return 0;
}

/* ---------------------------------------
 * FETCH_MARKED_NODES
 * --------------------------------------- */
int fetchMarkedNodes(HL_NodeList* nodelist)
{
  int i;
  hid_t file_id = -1;
  hid_t gid = -1;
  char* filename = NULL;
  int nNodes = 0;
  int result = 0;

  HL_DEBUG0("ENTER: fetchMarkedNodes");
  if (nodelist == NULL) {
    HL_ERROR0("Inparameters NULL");
    goto fail;
  }

  if ((filename = getHL_NodeListFileName(nodelist)) == NULL) {
    HL_ERROR0("Could not get filename from nodelist");
    goto fail;
  }

  if ((file_id = openHlHdfFile(filename, "r")) < 0) {
    HL_ERROR1("Could not open file '%s' when fetching data",filename);
    goto fail;
  }

  if ((gid = H5Gopen(file_id, ".", H5P_DEFAULT)) < 0) {
    HL_ERROR0("Could not open root group\n");
    goto fail;
  }

  if ((nNodes =  getHL_NodeListNumberOfNodes(nodelist)) < 0) {
    HL_ERROR0("Failed to get number of nodes");
    goto fail;
  }

  for (i = 0; i < nNodes; i++) {
    HL_Node* node = NULL;
    if ((node = getHL_NodeListNodeByIndex(nodelist, i)) == NULL) {
      HL_ERROR1("Error occured when fetching node at index %d", i);
      goto fail;
    }
    if (node->mark == NMARK_SELECT) {
      if (!fillNodeWithData(file_id, node)) {
        HL_ERROR1("Error occured when trying to fill node '%s'",node->name);
        goto fail;
      }
    }
  }
  result = 1;
fail:
  HL_H5F_CLOSE(file_id);
  HL_H5G_CLOSE(gid);
  HLHDF_FREE(filename);
  HL_DEBUG1("EXIT: fetchMarkedNodes with status = %d", result);
  return result;
}

/* ---------------------------------------
 * FETCH_NODE
 * --------------------------------------- */
HL_Node* fetchNode(HL_NodeList* nodelist, const char* name)
{
  hid_t file_id = -1;
  hid_t gid = -1;
  HL_Node* result = NULL;
  HL_Node* foundnode = NULL;
  char* filename = NULL;

  HL_DEBUG0("ENTER: fetchNode");
  if (name == NULL || nodelist == NULL) {
    HL_ERROR0("Inparameters NULL");
    goto fail;
  }
  if ((filename = getHL_NodeListFileName(nodelist)) == NULL) {
    HL_ERROR0("Could not get filename from nodelist");
    goto fail;
  }

  if ((foundnode = getHL_Node(nodelist, name))==NULL) {
    HL_ERROR1("No node: '%s' found", name);
    goto fail;
  }

  if (!(file_id = openHlHdfFile(filename, "r")) < 0) {
    HL_ERROR1("Could not open file '%s' when fetching data",filename);
    goto fail;
  }

  if ((gid = H5Gopen(file_id, ".", H5P_DEFAULT)) < 0) {
    HL_ERROR0("Could not open root group\n");
    goto fail;
  }

  if (!fillNodeWithData(file_id, foundnode)) {
    HL_ERROR1("Error occured when trying to fill node '%s'", foundnode->name);
    goto fail;
  }

  result = foundnode;
fail:
  HL_H5F_CLOSE(file_id);
  HL_H5G_CLOSE(gid);
  HLHDF_FREE(filename);
  HL_DEBUG0("EXIT: fetchNode");
  return result;
}
/*@} End of Interface functions */


