/**
 * Functions for working with HL_NodeList's.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-24
 */
#include "hlhdf.h"
#include "hlhdf_defines_private.h"
#include "hlhdf_debug.h"
#include "hlhdf_node.h"
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
      setHL_NodeMark(nodelist->nodes[i], mark);
    }
  }
}

/*
 * If 1 is returned, then responsibility has been taken, otherwise
 * the caller has to free both nodelist and node by him self
 */
int addHL_Node(HL_NodeList* nodelist, HL_Node* node)
{
  int newallocsize;
  int i;
  char* tmpStr = NULL;
  char* tmpPtr;
  int treeStructureOk = 0;
  int status = 0;
  HL_Type type;
  HL_SPEWDEBUG0("ENTER: addNode");

  if (nodelist == NULL || node == NULL) {
    HL_ERROR0("Inparameters NULL");
    return 0;
  }
  type = getHL_NodeType(node);
  tmpStr = getHL_NodeName(node);
  if (tmpStr == NULL) {
    HL_ERROR0("Failed to get node name");
    goto fail;
  }

  if (getHL_Node(nodelist, tmpStr) != NULL) {
    HL_ERROR1("Node %s already exists", tmpStr);
    goto fail;
  }

  if (!(tmpPtr = strrchr(tmpStr, '/'))) {
    HL_ERROR1("Could not extract '/' from node name %s",tmpStr);
    goto fail;
  } else {
    tmpPtr[0] = '\0';
    if (strcmp(tmpStr, "") != 0) {
      HL_Node* node = getHL_Node(nodelist, tmpStr);
      if (node != NULL) {
        HL_Type nType = getHL_NodeType(node);

        if ((nType == GROUP_ID) ||
            (nType == DATASET_ID && (type == ATTRIBUTE_ID || type == REFERENCE_ID))) {
          treeStructureOk = 1;
        }
      }
    } else {
      treeStructureOk = 1;
    }
  }

  if (treeStructureOk == 0) {
    HL_ERROR1("Tree structure not built correct, missing group or dataset %s",tmpStr);
    goto fail;
  }

  if (nodelist->nNodes < nodelist->nAllocNodes - 1) {
    nodelist->nodes[nodelist->nNodes++] = node;
  } else {
    newallocsize = nodelist->nAllocNodes + DEFAULT_SIZE_NODELIST;
    if (!(nodelist->nodes = realloc(nodelist->nodes, sizeof(HL_Node*) * newallocsize))) {
      HL_ERROR0("Serious memory error occured when reallocating Node list");
      goto fail;
    }

    for (i = nodelist->nAllocNodes; i < newallocsize; i++) {
      nodelist->nodes[i] = NULL;
    }
    nodelist->nAllocNodes = newallocsize;
    nodelist->nodes[nodelist->nNodes++] = node;
  }

  status = 1;
fail:
  HLHDF_FREE(tmpStr);
  return status;
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
    if (equalsHL_NodeName(nodelist->nodes[i], nodeName)) {
      return nodelist->nodes[i];
    }
  }

  HL_ERROR1("Could not locate node '%s'",nodeName);

  return NULL;
}

HL_CompoundTypeDescription* findHL_CompoundTypeDescription(
  HL_NodeList* nodelist, unsigned long objno0, unsigned long objno1)
{
  int i;
  HL_CompoundTypeDescription* retv = NULL;
  HL_SPEWDEBUG0("ENTER: findHL_CompoundTypeDescription");
  for (i = 0; i < nodelist->nNodes; i++) {
    if (getHL_NodeType(nodelist->nodes[i]) == TYPE_ID) {
      HL_CompoundTypeDescription* descr = getHL_NodeCompoundDescription(nodelist->nodes[i]);
      if (descr != NULL) {
        if (objno0 == descr->objno[0] && objno1 == descr->objno[1]) {
          retv = descr;
          goto done;
        }
      }
    }
  }
done:
  HL_SPEWDEBUG0("EXIT: findHL_CompoundTypeDescription");
  return retv;
}
