/**
 * Functions for working with HL_Node's and HL_NodeList's.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-11
 */
#include "hlhdf.h"
#include "hlhdf_defines_private.h"
#include "hlhdf_debug.h"
#include <string.h>
#include <stdlib.h>

/*@{ Structs */
/**
 * Represents a HDF5 file.
 * @ingroup hlhdf_c_apis
 */
struct  _HL_NodeList {
   char* filename;     /**< The file name */
   int nNodes;         /**< Number of nodes */
   int nAllocNodes;    /**< Number of allocated nodes */
   HL_Node** nodes;    /**< The list of nodes (max size is nNodes - 1) */
};
/*@{ End of Structs */

/*@{ Private functions */

static HL_Node* newHL_NodeWithType(const char* name, HL_Type type)
{
  HL_Node* retv = NULL;
  HL_SPEWDEBUG0("ENTER: newHL_NodeWithType");
  if (!(retv = newHL_Node(name))) {
    HL_ERROR0("Failed to allocate HL_Node item");
    goto fail;
  }
  retv->type = type;
fail:
  HL_SPEWDEBUG0("EXIT: newHL_NodeWithType");
  return retv;
}

static hid_t HLNode_createStringType(size_t length)
{
  hid_t type;
  HL_SPEWDEBUG0("ENTER: HLNode_createStringType");

  type = H5Tcopy(H5T_C_S1);
  H5Tset_size(type, length);

  HL_SPEWDEBUG0("EXIT: HLNode_createStringType");
  return type;
}

/*@} End of Private functions */

/*@{ Interface functions */
HL_NodeList* newHL_NodeList(void)
{
  HL_NodeList* retv = NULL;
  int i;
  HL_SPEWDEBUG0("ENTER: newHL_NodeList");
  if (!(retv = (HL_NodeList*) malloc(sizeof(HL_NodeList)))) {
    HL_ERROR0("Failed to allocate memory for NODE");
    return NULL;
  }
  retv->filename = NULL;

  if (!(retv->nodes = (HL_Node**) malloc(sizeof(HL_Node*) * DEFAULT_SIZE_NODELIST))) {
    HL_ERROR0("Failed to allocate memory for HL_NodeList");
    free(retv);
    return NULL;
  }
  for (i = 0; i < DEFAULT_SIZE_NODELIST; i++) {
    retv->nodes[i] = NULL;
  }
  retv->nNodes = 0;
  retv->nAllocNodes = DEFAULT_SIZE_NODELIST;
  return retv;
}

void freeHL_NodeList(HL_NodeList* nodelist)
{
  int i;
  HL_SPEWDEBUG0("ENTER: freeHL_NodeList");
  if (!nodelist)
    return;

  if (nodelist->nodes) {
    for (i = 0; i < nodelist->nNodes; i++) {
      freeHL_Node(nodelist->nodes[i]);
    }
    HLHDF_FREE(nodelist->nodes);
  }
  HLHDF_FREE(nodelist->filename);
  HLHDF_FREE(nodelist);
}

int setHL_NodeListFileName(HL_NodeList* nodelist, const char* filename)
{
  int status = 0;
  char* newfilename = NULL;

  if (nodelist == NULL || filename == NULL) {
    HL_ERROR0("Inparameters NULL");
    goto fail;
  }
  if ((newfilename = strdup(filename)) == NULL) {
    HL_ERROR1("Failed to allocate memory for file %s", filename);
    goto fail;
  }
  HLHDF_FREE(nodelist->filename);
  nodelist->filename = newfilename;
  newfilename = NULL; // Hand over memory

  status = 1;
fail:
  HLHDF_FREE(newfilename);
  return status;
}

char* getHL_NodeListFileName(HL_NodeList* nodelist)
{
  char* retv = NULL;

  if (nodelist == NULL) {
    HL_ERROR0("Inparameters NULL");
    return NULL;
  }

  if (nodelist->filename != NULL) {
    if ((retv = strdup(nodelist->filename)) == NULL) {
      HL_ERROR1("Failed to allocate memory for filename: %s", nodelist->filename);
    }
  }

  return retv;
}

int getHL_NodeListNumberOfNodes(HL_NodeList* nodelist)
{
  if (nodelist == NULL) {
    HL_ERROR0("Inparameters NULL");
    return -1;
  }
  return nodelist->nNodes;
}

HL_Node* getHL_NodeListNodeByIndex(HL_NodeList* nodelist, int index)
{
  if (nodelist == NULL) {
    HL_ERROR0("Inparameters NULL");
    return NULL;
  } else if (index < 0 || index >= nodelist->nNodes) {
    HL_ERROR0("index out of range");
    return NULL;
  }
  return nodelist->nodes[index];
}

void markHL_NodeListNodes(HL_NodeList* nodelist, const HL_NodeMark mark)
{
  int i = 0;
  if (nodelist != NULL) {
    for (i = 0; i < nodelist->nNodes; i++) {
      nodelist->nodes[i]->mark = mark;
    }
  }
}

HL_Node* newHL_Node(const char* name)
{
  HL_Node* retv = NULL;
  HL_SPEWDEBUG0("ENTER: newHL_Node");
  if (!name) {
    HL_ERROR0("When creating a nodelist item, name has to be specified");
    goto fail;
  }

  if (!(retv = (HL_Node*) malloc(sizeof(HL_Node)))) {
    HL_ERROR0("Failed to allocate HL_Node");
    goto fail;
  }
  retv->type = UNDEFINED_ID;
  strcpy(retv->name, name);
  retv->ndims = 0;
  memset(retv->dims, 0, sizeof(size_t) * 4);
  retv->data = NULL;
  retv->rawdata = NULL;
  strcpy(retv->format, DATAFORMAT_UNDEFINED);
  retv->typeId = -1;
  retv->dSize = 0;
  retv->rdSize = 0;
  retv->dataType = DTYPE_UNDEFINED_ID;
  retv->hdfId = -1;
  retv->mark = NMARK_CREATED;
  retv->compoundDescription = NULL;
  retv->compression = NULL;
fail:
  return retv;
}

void freeHL_Node(HL_Node* node)
{
  HL_SPEWDEBUG0("ENTER: freeHL_Node");
  if (!node)
    return;

  if (node->typeId >= 0) {
    disableErrorReporting();
    H5Tclose(node->typeId);
    enableErrorReporting();
  }

  if (node->hdfId >= 0) {
    switch (node->type) {
    case ATTRIBUTE_ID:
      H5Aclose(node->hdfId);
      break;
    case DATASET_ID:
      H5Dclose(node->hdfId);
      break;
    case GROUP_ID:
      H5Gclose(node->hdfId);
      break;
    case TYPE_ID:
      disableErrorReporting();
      H5Tclose(node->hdfId);
      enableErrorReporting();
      break;
    case REFERENCE_ID:
      /* Don't bother nothing should be open for this */
      break;
    default:
      HL_ERROR1("Strage node type, can't close it (%ld)",(long)node->hdfId);
      break;
    }
  }

  HLHDF_FREE(node->data);
  HLHDF_FREE(node->rawdata);
  freeHL_CompoundTypeDescription(node->compoundDescription);
  freeHL_Compression(node->compression);
  HLHDF_FREE(node);
}

HL_Node* newHL_Group(const char* name)
{
  return newHL_NodeWithType(name, GROUP_ID);
}

HL_Node* newHL_Attribute(const char* name)
{
  return newHL_NodeWithType(name, ATTRIBUTE_ID);
}

HL_Node* newHL_Dataset(const char* name)
{
  return newHL_NodeWithType(name, DATASET_ID);
}

HL_Node* newHL_Datatype(const char* name)
{
  return newHL_NodeWithType(name, TYPE_ID);
}

HL_Node* newHL_Reference(const char* name)
{
  return newHL_NodeWithType(name, REFERENCE_ID);
}

HL_Node* copyHL_Node(HL_Node* node)
{
  int i, npts;
  HL_Node* retv = NULL;
  HL_SPEWDEBUG0("ENTER: copyHL_Node");
  if (!node)
    return NULL;

  retv = newHL_Node(node->name);
  retv->type = node->type;
  retv->ndims = node->ndims;
  retv->dSize = node->dSize;
  retv->rdSize = node->rdSize;
  memcpy(retv->dims, node->dims, 4* sizeof (hsize_t));
  npts=1;
  for(i=0;i<retv->ndims;i++)
    npts*=retv->dims[i];

  retv->data = (unsigned char*)malloc(npts*retv->dSize);
  memcpy(retv->data,node->data,npts*retv->dSize);

  if(node->rawdata!=NULL) {
    retv->rawdata = (unsigned char*)malloc(npts*retv->rdSize);
    memcpy(retv->rawdata,node->rawdata,npts*retv->rdSize);
  } else {
    retv->rdSize = 0;
    retv->rawdata = NULL;
  }
  strcpy(retv->format,node->format);

  if(node->typeId>=0)
    retv->typeId=H5Tcopy(node->typeId);

  retv->dataType=node->dataType;
  //fprintf(stderr, "Copying node with hdfId = %d\n",node->hdfId);
  retv->hdfId=-1; //node->hdfId;
  retv->mark=node->mark;

  retv->compoundDescription=copyHL_CompoundTypeDescription(node->compoundDescription);

  return retv;
}

/*
 * If 1 is returned, then responsibility has been taken, otherwise
 * the caller has to free both nodelist and node by him self
 */
int addHL_Node(HL_NodeList* nodelist, HL_Node* node)
{
  int newallocsize;
  int i;
  char tmpStr[256];
  char* tmpPtr;
  int treeStructureOk = 0;

  HL_SPEWDEBUG0("ENTER: addNode");

  strcpy(tmpStr, node->name);

  for (i = 0; i < nodelist->nNodes; i++) {
    if (strcmp(nodelist->nodes[i]->name, node->name) == 0) {
      HL_ERROR1("Node %s already exists",node->name);
      return 0;
    }
  }

  if (!(tmpPtr = strrchr(tmpStr, '/'))) {
    HL_ERROR1("Could not extract '/' from node name %s",node->name);
    return 0;
  } else {
    tmpPtr[0] = '\0';
    if (strcmp(tmpStr, "") != 0) {
      for (i = 0; treeStructureOk == 0 && i < nodelist->nNodes; i++) {
        if (strcmp(tmpStr, nodelist->nodes[i]->name) == 0) {
          if ((nodelist->nodes[i]->type == GROUP_ID) ||
              (nodelist->nodes[i]->type == DATASET_ID &&
                (node->type == ATTRIBUTE_ID || node->type == REFERENCE_ID)))
            treeStructureOk = 1;
        }
      }
    } else {
      treeStructureOk = 1;
    }
  }

  if (treeStructureOk == 0) {
    HL_ERROR1("Tree structure not built correct, missing group or dataset %s",tmpStr);
    return 0;
  }

  if (nodelist->nNodes < nodelist->nAllocNodes - 1) {
    nodelist->nodes[nodelist->nNodes++] = node;
    return 1;
  }

  newallocsize = nodelist->nAllocNodes + DEFAULT_SIZE_NODELIST;
  if (!(nodelist->nodes = realloc(nodelist->nodes, sizeof(HL_Node*) * newallocsize))) {
    HL_ERROR0("Serious memory error occured when reallocating Node list");
    return 0;
  }

  for (i = nodelist->nAllocNodes; i < newallocsize; i++) {
    nodelist->nodes[i] = NULL;
  }
  nodelist->nAllocNodes = newallocsize;
  nodelist->nodes[nodelist->nNodes++] = node;

  return 1;
}

HL_Node* getHL_Node(HL_NodeList* nodelist, const char* nodeName)
{
  int i;
  HL_SPEWDEBUG0("ENTER: getNode");
  if (!nodelist || !nodeName) {
    HL_ERROR0("Can't get node when either nodelist or nodeName is NULL");
    return NULL;
  }

  for (i = 0; i < nodelist->nNodes; i++) {
    if (strcmp(nodelist->nodes[i]->name, nodeName) == 0) {
      return nodelist->nodes[i];
    }
  }

  HL_ERROR1("Could not locate node '%s'",nodeName);

  return NULL;
}

int setHL_NodeScalarValue(HL_Node* node, size_t sz, unsigned char* value,
  const char* fmt, hid_t typid)
{
  HL_SPEWDEBUG0("ENTER: setHL_NodeScalarValue");
  if (!fmt) {
    HL_ERROR0("When setting a node value, fmt has to be recognized");
    return 0;
  }

  if (!(node->data = (unsigned char*) malloc(sz))) {
    HL_ERROR0("Failed to allocate memory when setting value");
    return 0;
  }
  memcpy(node->data, value, sz);
  node->dSize = sz;

  if ((strcmp(fmt, "string") == 0) && typid < 0) {
    node->typeId = HLNode_createStringType(sz);
    if (node->typeId < 0) {
      HL_ERROR0("Failed to create string type\n");
      return 0;
    }
  } else {
    strcpy(node->format, fmt);
    if (typid >= 0)
      node->typeId = typid;
  }

  node->dataType = HL_SIMPLE;

  if (node->mark != NMARK_CREATED)
    node->mark = NMARK_CHANGED;

  return 1;
}

int setHL_NodeArrayValue(HL_Node* node, size_t sz, int ndims, hsize_t* dims,
  unsigned char* value, const char* fmt, hid_t typid)
{
  int i;
  size_t npts = 0;
  HL_SPEWDEBUG0("ENTER: setHL_NodeArrayValue");
  if (!fmt) {
    HL_ERROR0("When setting a node values, fmt has to be recognized");
    return 0;
  }

  npts = 1;
  for (i = 0; i < ndims; i++) {
    node->dims[i] = dims[i];
    npts *= dims[i];
  }
  if (!(node->data = (unsigned char*) malloc(npts * sz))) {
    HL_ERROR0("Failed to allocate memory when setting value");
    return 0;
  }
  memcpy(node->data, value, npts * sz);
  node->dSize = sz;
  node->ndims = ndims;
  strcpy(node->format, fmt);

  if (typid >= 0)
    node->typeId = typid;

  node->dataType = HL_ARRAY;

  if (node->mark != NMARK_CREATED)
    node->mark = NMARK_CHANGED;

  return 1;
}

int setHL_NodeMark(HL_Node* node, const HL_NodeMark mark)
{
  if (node == NULL) {
    HL_ERROR0("Inparameters NULL");
    return 0;
  }
  node->mark = mark;
  return 1;
}

HL_CompoundTypeDescription* findHL_CompoundTypeDescription(
  HL_NodeList* nodelist, unsigned long objno0, unsigned long objno1)
{
  int i;
  HL_CompoundTypeDescription* retv = NULL;
  HL_SPEWDEBUG0("ENTER: findHL_CompoundTypeDescription");
  for (i = 0; i < nodelist->nNodes; i++) {
    if (nodelist->nodes[i]->type == TYPE_ID
        && nodelist->nodes[i]->compoundDescription) {
      if (objno0 == nodelist->nodes[i]->compoundDescription->objno[0] &&
          objno1 == nodelist->nodes[i]->compoundDescription->objno[1]) {
        retv = nodelist->nodes[i]->compoundDescription;
        goto done;
      }
    }
  }
done:
  HL_SPEWDEBUG0("EXIT: findHL_CompoundTypeDescription");
  return retv;
}


int commitHL_Datatype(HL_Node* node, hid_t testStruct_hid)
{
  HL_SPEWDEBUG0("ENTER: commitDatatype");
  node->hdfId = testStruct_hid;
  HL_SPEWDEBUG0("EXIT: commitDatatype");
  return 1;
}
