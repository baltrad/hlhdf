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
 * @date 2009-06-13
 */
#include <Python.h>

/**
 * You have to define HLHDF_PYMODULE_WITH_IMPORT_ARRAY to be able to force arrayobject to be
 * correctly imported.
 */
#define HLHDF_PYMODULE_WITH_IMPORT_ARRAY
#include "pyhlhdf_common.h"  /* this includes arrayobject.h */
#include "hlhdf.h"
#include "hlhdf_alloc.h"
#include "hlhdf_private.h"
#include "hlhdf_node_private.h"
#include "hlhdf_debug.h"
#include "hlhdf_defines_private.h"

/**
 * @defgroup pyhl_api PyHL Python-API Reference Manual
 *
 * The _pyhl module defines the IO access for reading/writing HDF5
 * files. The module implements two classes for building an HDF5 file
 * representation. The <b>nodelist</b> class implements a list that should
 * represent the file itself. The <b>node</b> class represents the items in the HDF5 file.
 * The <b>nodelist</b> contain several <b>node</b>'s for building the HDF5 file. You can use
 * this interface to write Python programs that interfact with HDF5
 * files.
 *
 * @todo Make the pyhl API-reference manual look like the rest of the documentation.
 */

/**
 * Error object for reporting errors to the python interpreeter
 */
static PyObject *ErrorObject;

/**
 * Sets an error type and error message, then returns NULL.
 */
#define raiseException(type,msg)\
{PyErr_SetString(type,msg);return NULL;}

/**
 * Sets an error type and error message.
 */
#define setException(type,msg)\
{PyErr_SetString(type,msg);}

/**
 * The nodelist object
 */
typedef struct {
   PyObject_HEAD /*Always have to be on top*/
   HL_NodeList* nodelist; /**< the node list */
} PyhlNodelist;

/**
 * The pyhl node object
 */
typedef struct {
   PyObject_HEAD /*Always have to be on top*/
   HL_Node* node; /**< the node */
} PyhlNode;

/**
 * The pyhl file creation property object
 */
typedef struct {
   PyObject_HEAD /*Always have to be on top*/
   HL_FileCreationProperty* props; /**< the properties */
} PyhlFileCreationProperty;

/**
 * The pyhl compression property object.
 */
typedef struct {
   PyObject_HEAD /*Always have to be on top*/
   HL_Compression* compr; /**< the compression instance */
} PyhlCompression;

/**
 * PyhlNodelist represents a HL_NodeList
 */
staticforward PyTypeObject PyhlNodelist_Type;

/**
 * PyhlNode represents a HL_Node.
 */
staticforward PyTypeObject PyhlNode_Type;

/**
 * PyhlFileCreationProperty represents a HL_FileCreationProperty.
 */
staticforward PyTypeObject PyhlFileCreationProperty_Type;

/**
 * PyhlCompression represents a HL_Compression
 */
staticforward PyTypeObject PyhlCompression_Type;

/**
 * Checks if the object is a Pyhl (nodelist) type
 */
#define PyhlNodelist_Check(op) ((op)->ob_type == &PyhlNodelist_Type)

/**
 * Checks if the object is a Pyhl node type.
 */
#define PyhlNode_Check(op) ((op)->ob_type == &PyhlNode_Type)

/**
 * Checks if the object is a Pyhl file creation property object
 */
#define PyhlFileCreationProperty_Check(op) ((op)->ob_type == &PyhlFileCreationProperty_Type)

/**
 * Checks if the object is a pyhl compression object
 */
#define PyhlCompression_Check(op) ((op)->ob_type == &PyhlCompression_Type)

/**
 * Deallocates the pyhl file list.
 * @param[in] pyhl_object the object to deallocate.
 */
static void _dealloc(PyhlNodelist* pyhl_object)
{
  /*Nothing yet*/
  if (!pyhl_object)
    return;

  HLNodeList_free(pyhl_object->nodelist);

  PyObject_Del(pyhl_object);
}

/**
 * Deallocates the pyhl node list.
 * @param[in] pyhl_node the object to deallocate.
 */
static void _dealloc_pyhlnode(PyhlNode* pyhl_node)
{
  /*Nothing yet*/
  if (!pyhl_node)
    return;

  HLNode_free(pyhl_node->node);

  PyObject_Del(pyhl_node);
}

/**
 * Deallocates the pyhl file creation property.
 * @param[in] val the object to deallocate.
 */
static void _dealloc_pyhlfilecreationproperty(PyhlFileCreationProperty* val)
{
  if (!val)
    return;
  HLFileCreationProperty_free(val->props);
  PyObject_Del(val);
}

/**
 * Deallocates the pyhl compression instance.
 * @param[in] val the object to deallocate.
 */
static void _dealloc_pyhlcompression(PyhlCompression* val)
{
  if (!val)
    return;
  HLCompression_free(val->compr);
  PyObject_Del(val);
}

/**
 * Creates a new instance of the nodelist.
 * @param[in] self this instance.
 * @param[in] args arguments for creation (NOT USED).
 * @return the object on success, otherwise NULL
 */
static PyObject* _pyhl_new_nodelist(PyObject* self, PyObject* args)
{
  PyhlNodelist* retv = NULL;
  retv = PyObject_NEW(PyhlNodelist,&PyhlNodelist_Type);
  if (!retv)
    return NULL;

  if (!(retv->nodelist = HLNodeList_new())) {
    setException(PyExc_MemoryError,"Failed to create HL NodeList\n");
    _dealloc(retv);
    retv = NULL;
  }

  return (PyObject*) retv;
}

/**
 * Creates a new node instance.
 * @param[in] self this instance.
 * @param[in] args arguments for creation. (is|O) type, nodename and eventually a compression object.
 * @return the object on success, otherwise NULL
 */
static PyObject* _pyhl_new_node(PyObject* self, PyObject* args)
{
  PyhlNode* retv = NULL;
  PyObject* comprinobj = NULL;
  PyhlCompression* compr = NULL;
  HL_Type type;
  char* nodename;

  if (!PyArg_ParseTuple(args, "is|O", &type, &nodename, &comprinobj))
    return NULL;

  switch (type) {
  case ATTRIBUTE_ID:
  case GROUP_ID:
  case DATASET_ID:
  case TYPE_ID:
  case REFERENCE_ID:
    break;
  default:
    printf("Unsupported node type %d\n", type);
    return NULL;
  }

  if (comprinobj != NULL) {
    if (!PyhlCompression_Check(comprinobj)) {
      setException(PyExc_AttributeError,"Third argument must be of type PyhlCompression");
      return NULL;
    }
    if (type != DATASET_ID) {
      setException(PyExc_AttributeError,"It is only possible to specify PyhlCompression when creating a node of type DATASET_ID");
      return NULL;
    }
    compr = (PyhlCompression*) comprinobj;
  }

  retv = PyObject_NEW(PyhlNode,&PyhlNode_Type);
  if (!retv)
    return NULL;

  switch (type) {
  case ATTRIBUTE_ID:
    retv->node = HLNode_newAttribute(nodename);
    break;
  case GROUP_ID:
    retv->node = HLNode_newGroup(nodename);
    break;
  case DATASET_ID:
    retv->node = HLNode_newDataset(nodename);
    break;
  case TYPE_ID:
    retv->node = HLNode_newDatatype(nodename);
    break;
  case REFERENCE_ID:
    retv->node = HLNode_newReference(nodename);
    break;
  default:
    retv->node = NULL;
    break;
  }

  if (!(retv->node)) {
    setException(PyExc_MemoryError,"Failed to create Node\n");
    _dealloc_pyhlnode(retv);
    retv = NULL;
  }

  if (compr != NULL && compr->compr != NULL) {
    HLNode_setCompression(retv->node, HLCompression_clone(compr->compr));
  }

  return (PyObject*) retv;
}

static PyObject* _pyhl_new_filecreationproperty(PyObject* self, PyObject* args)
{
  PyhlFileCreationProperty* retv = NULL;
  retv = PyObject_NEW(PyhlFileCreationProperty,&PyhlFileCreationProperty_Type);
  if (!retv)
    return NULL;
  if (!(retv->props = HLFileCreationProperty_new())) {
    setException(PyExc_MemoryError,"Failed to create FileCreationProperty\n");
    _dealloc_pyhlfilecreationproperty(retv);
    retv = NULL;
  }
  return (PyObject*) retv;
}

static PyObject* _pyhl_new_compression(PyObject* self, PyObject* args)
{
  PyhlCompression* retv = NULL;
  HL_CompressionType type;
  if (!PyArg_ParseTuple(args, "i", &type))
    return NULL;

  retv = PyObject_NEW(PyhlCompression,&PyhlCompression_Type);
  if (!retv)
    return NULL;
  if (!(retv->compr = HLCompression_new(type))) {
    setException(PyExc_MemoryError,"Failed to create HL_Compression instance\n");
    _dealloc_pyhlcompression(retv);
    retv = NULL;
  }
  return (PyObject*) retv;
}

static PyObject* _pyhl_read_nodelist(PyObject* self, PyObject* args)
{
  HL_NodeList* nodelist = NULL;
  PyhlNodelist* retv = NULL;
  char* filename = NULL;
  char* frompath = NULL;

  if (!PyArg_ParseTuple(args, "s|s", &filename, &frompath))
    return NULL;

  if (!frompath) {
    nodelist = HLNodeList_read(filename);
  } else {
    nodelist = HLNodeList_readFrom(filename, frompath);
  }

  if (!nodelist) {
    char errmsg[256];
    if (!frompath)
      sprintf(errmsg, "Could not read file '%s'", filename);
    else
      sprintf(errmsg, "Could not read file '%s' from '%s'", filename, frompath);
    setException(PyExc_IOError,errmsg);
    goto fail;
  }

  if (!(retv = (PyhlNodelist*) _pyhl_new_nodelist(NULL, NULL))) {
    setException(PyExc_MemoryError,"Could not allocate nodelist instance");
    goto fail;
  }

  /*Change the nodelist*/
  HLNodeList_free(retv->nodelist);
  retv->nodelist = nodelist;
  return (PyObject*) retv;
fail:
  HLNodeList_free(nodelist);
  if (retv)
    _dealloc(retv);
  return NULL;
}

static PyObject* _pyhl_is_file_hdf5(PyObject* self, PyObject* args)
{
  char* filename;
  int val;

  if (!(PyArg_ParseTuple(args, "s", &filename)))
    return NULL;

  val = HL_isHDF5File(filename);

  return PyInt_FromLong((int) val);
}

static PyObject* _pyhl_show_hdf5errors(PyObject* self, PyObject* args)
{
  int yesno;
  if (!PyArg_ParseTuple(args, "i", &yesno))
    return NULL;
  if (yesno == 0) {
    HL_disableHdf5ErrorReporting();
  } else {
    HL_enableHdf5ErrorReporting();
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* _pyhl_show_hlhdferrors(PyObject* self, PyObject* args)
{
  int yesno;
  if (!PyArg_ParseTuple(args, "i", &yesno))
    return NULL;
  if (yesno == 0) {
    HL_setDebugLevel(HLHDF_SILENT);
  } else {
    HL_setDebugLevel(HLHDF_DEBUG);
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* _pyhl_get_hdf5version(PyObject* self, PyObject* args)
{
  const char* ver = HL_getHDF5Version();
  if (ver != NULL) {
    return PyString_FromString(ver);
  } else {
    Py_RETURN_NONE;
  }
}

/* PyhlNodelist member methods */
static PyObject* _pyhl_add_node(PyhlNodelist* self, PyObject* args)
{
  PyObject* inp;
  PyhlNode* pyhlNode;
  HL_Node* aNode;

  if (!PyArg_ParseTuple(args, "O", &inp))
    return NULL;

  if (!PyhlNode_Check(inp)) {
    setException(PyExc_TypeError,"Trying to add node, which not is of PyhlNodeCore type");
    return NULL;
  }

  pyhlNode = (PyhlNode*) inp;
  aNode = pyhlNode->node;

  if (!HLNodeList_addNode(self->nodelist, aNode)) {
    setException(PyExc_IOError,"Could not add node to nodelist");
    return NULL;
  }

  pyhlNode->node = NULL;

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* _pyhl_write(PyhlNodelist* self, PyObject* args)
{
  char* filename;
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;

  int doCompress = -1;
  PyObject* props = NULL;
  HL_Compression* theCompression = NULL;

  if (!PyArg_ParseTuple(args, "s|OO", &filename, &obj1, &obj2))
    return NULL;

  if (obj1 != NULL) {
    if (PyInt_Check(obj1)) {
      doCompress = PyInt_AsLong(obj1);
    } else if (PyhlFileCreationProperty_Check(obj1)) {
      props = obj1;
    } else {
      setException(PyExc_AttributeError,"write method should be called with write(filename[,zlib compression level(int)][,file creation property])");
      return NULL;
    }
  }
  if (obj2 != NULL) {
    if (PyInt_Check(obj2)) {
      if (doCompress != -1) {
        setException(PyExc_AttributeError,"it is not meaningful or possible to call write with both second and third argument as integers, format is: "
            "write(filename[,zlib compression level(int)][,file creation property])");
        return NULL;
      }
      doCompress = PyInt_AsLong(obj2);
    } else if (PyhlFileCreationProperty_Check(obj2)) {
      if (props != NULL) {
        setException(PyExc_AttributeError,"it is not meaningful or possible to call write with both second and third argument as FileCreationProperty instances"
            ", format is: write(filename[,zlib compression level(int)][,file creation property])");
        return NULL;
      }
      props = obj2;
    } else {
      setException(PyExc_AttributeError,"write method should be called with write(filename[,zlib compression level(int)][,file creation property])");
      return NULL;
    }
  }

  if (!HLNodeList_setFileName(self->nodelist,filename)) {
    setException(PyExc_IOError, "Could not set filename for nodelist");
    return NULL;
  }

  if (doCompress != -1) {
    theCompression = HLCompression_new(CT_ZLIB);
    theCompression->level = doCompress;
  }

  if (!HLNodeList_write(self->nodelist,
                        (props != NULL) ? ((PyhlFileCreationProperty*) props)->props : NULL,
                        theCompression)) {
    setException(PyExc_IOError,"Could not write hdf file");
    if (theCompression) {
      HLCompression_free(theCompression);
    }
    return NULL;
  }

  if (theCompression) {
    HLCompression_free(theCompression);
  }

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* _pyhl_update(PyhlNodelist* self, PyObject* args)
{
  int doCompress = 6;
  HL_Compression compression;

  if (!PyArg_ParseTuple(args, "|i", &doCompress))
    return NULL;
  HLCompression_init(&compression, CT_ZLIB);
  compression.level = doCompress;
  if (!HLNodeList_update(self->nodelist, &compression)) {
    setException(PyExc_IOError,"Could not update file");
    return NULL;
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* _pyhl_get_node_names(PyhlNodelist* self, PyObject* args)
{
  PyObject* retv = NULL;
  PyObject* pyo = NULL;
  int i;
  int nNodes = 0;

  char errbuf[256];
  if (!(retv = PyDict_New())) {
    setException(PyExc_MemoryError,"Could not allocate dictionary");
    return NULL;
  }

  if ((nNodes = HLNodeList_getNumberOfNodes(self->nodelist)) < 0) {
    setException(PyExc_IOError, "Could not read number of nodes");
    goto fail;
  }

  for (i = 0; i < nNodes; i++) {
    HL_Node* node = HLNodeList_getNodeByIndex(self->nodelist, i);
    if (node == NULL) {
      setException(PyExc_IOError, "Could not fetch node");
      goto fail;
    }
    switch (HLNode_getType(node)) {
    case ATTRIBUTE_ID:
      pyo = PyInt_FromLong(ATTRIBUTE_ID);
      break;
    case DATASET_ID:
      pyo = PyInt_FromLong(DATASET_ID);
      break;
    case TYPE_ID:
      pyo = PyInt_FromLong(TYPE_ID);
      break;
    case GROUP_ID:
      pyo = PyInt_FromLong(GROUP_ID);
      break;
    case REFERENCE_ID:
      pyo = PyInt_FromLong(REFERENCE_ID);
      break;
    default:
      sprintf(errbuf, "Unknown type for node '%s': '%d'",
              HLNode_getName(node), HLNode_getType(node));
      setException(PyExc_TypeError,errbuf);
      goto fail;
    }
    if (PyDict_SetItemString(retv, HLNode_getName(node), pyo) == -1) {
      setException(PyExc_AttributeError,"Failed to set dictionary item");
      goto fail;
    }
    Py_XDECREF(pyo);
    pyo=NULL;
  }

  return retv;
fail:
  if(pyo) {
    Py_XDECREF(pyo);
  }
  if(retv) {
    Py_XDECREF(retv);
  }
  return NULL;
}

static PyObject* _pyhl_select_all(PyhlNodelist* self, PyObject* args)
{
  HLNodeList_selectAllNodes(self->nodelist);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* _pyhl_select_metadata(PyhlNodelist* self, PyObject* args)
{
  HLNodeList_selectMetadataNodes(self->nodelist);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* _pyhl_select_node(PyhlNodelist* self, PyObject* args)
{
  char* nodename;
  char errbuf[256];

  if (!PyArg_ParseTuple(args, "s", &nodename))
    return NULL;
  if (!HLNodeList_selectNode(self->nodelist, nodename)) {
    sprintf(errbuf, "Could not select node '%s'", nodename);
    setException(PyExc_AttributeError,errbuf);
    goto fail;
  }
  Py_INCREF(Py_None);
  return Py_None;
fail:
  return NULL;
}

static PyObject* _pyhl_deselect_node(PyhlNodelist* self, PyObject* args)
{
  char* nodename;
  char errbuf[256];

  if (!PyArg_ParseTuple(args, "s", &nodename))
    return NULL;
  if (!HLNodeList_deselectNode(self->nodelist, nodename)) {
    sprintf(errbuf, "Could not select node '%s'", nodename);
    setException(PyExc_AttributeError,errbuf);
    goto fail;
  }
  Py_INCREF(Py_None);
  return Py_None;
fail:
  return NULL;
}

static PyObject* _pyhl_fetch(PyhlNodelist* self, PyObject* args)
{
  if (!HLNodeList_fetchMarkedNodes(self->nodelist)) {
    setException(PyExc_IOError,"Could not fetch selected nodes");
    goto fail;
  }
  Py_INCREF(Py_None);
  return Py_None;
fail:
  return NULL;
}

static PyObject* _pyhl_fetch_node(PyhlNodelist* self, PyObject* args)
{
  char* nodename;
  char errbuf[256];
  HL_Node* node = NULL;
  PyhlNode* retv = NULL;
  PyObject* myArgs = NULL;

  if (!PyArg_ParseTuple(args, "s", &nodename))
    return NULL;

  if (!(node = HLNodeList_fetchNode(self->nodelist, nodename))) {
    sprintf(errbuf, "Could not fetch node '%s'", nodename);
    setException(PyExc_IOError,errbuf);
    goto fail;
  }

  if (!(myArgs = Py_BuildValue("(is)", HLNode_getType(node), HLNode_getName(node)))) {
    setException(PyExc_AttributeError,"Could not create argument tuple for allocating node");
    goto fail;
  }

  if (!(retv = (PyhlNode*) _pyhl_new_node((PyObject*) self, myArgs))) {
    setException(PyExc_AttributeError,"Could not create node to return");
    goto fail;
  }

  HLNode_free(retv->node);

  retv->node = HLNode_copy(node);

  Py_XDECREF(myArgs);
  return (PyObject*)retv;
fail:
  Py_XDECREF(myArgs);
  _dealloc_pyhlnode(retv);
  return NULL;
}

static PyObject* _pyhl_get_node(PyhlNodelist* self, PyObject* args)
{
  char* nodename;
  char errbuf[256];
  HL_Node* node = NULL;
  PyhlNode* retv = NULL;
  PyObject* myArgs = NULL;

  if (!PyArg_ParseTuple(args, "s", &nodename))
    return NULL;

  if (!(node = HLNodeList_getNodeByName(self->nodelist, nodename))) {
    sprintf(errbuf, "Could not get node '%s'", nodename);
    setException(PyExc_IOError,errbuf);
    goto fail;
  }

  if (!(myArgs = Py_BuildValue("(is)", HLNode_getType(node), HLNode_getName(node)))) {
    setException(PyExc_AttributeError,"Could not create argument tuple for allocating node");
    goto fail;
  }

  if (!(retv = (PyhlNode*) _pyhl_new_node((PyObject*) self, myArgs))) {
    setException(PyExc_AttributeError,"Could not create node to return");
    goto fail;
  }

  HLNode_free(retv->node);

  retv->node = HLNode_copy(node);

  Py_XDECREF(myArgs);
  return (PyObject*)retv;
fail:
  Py_XDECREF(myArgs);
  if(retv)
    _dealloc_pyhlnode(retv);
  return NULL;
}

/* PyhlNode member methods */
static PyObject* _pyhl_node_set_scalar_value(PyhlNode* self, PyObject* args)
{
  int itemSize;
  char* hltypename;
  PyObject* data;
  hid_t lhid = -1;
  hid_t tmpHid = -1;
  int chkVal = 0;

  if (!PyArg_ParseTuple(args, "iOsi", &itemSize, &data, &hltypename, &lhid))
    return NULL;

  if (!self->node) {
    setException(PyExc_AttributeError,"The responsibility of the node has been dropped, probably by doing a addNode");
    return NULL;
  }

  if (strcmp(hltypename, "string") != 0 && strcmp(hltypename, "compound") != 0
      && (tmpHid = HL_translateFormatStringToDatatype(hltypename)) == -1) {
    char errmsg[256];
    sprintf(errmsg, "No datatype called '%s'.", hltypename);
    setException(PyExc_AttributeError,errmsg);
    return NULL;
  }

  if (tmpHid != -1) {
    /* Well, ordinary attribute, just make sure the space is enough */
    size_t typeSize = H5Tget_size(tmpHid);
    chkVal = 0;
    switch (H5Tget_class(tmpHid)) {
    case H5T_INTEGER: {
      if (typeSize <= sizeof(char)) {
        char mData = (char) PyInt_AsLong(data);
        chkVal = HLNode_setScalarValue(self->node, sizeof(mData),
                                       (unsigned char*) &mData, hltypename, -1);
      } else if (typeSize <= sizeof(short)) {
        short mData = (char) PyInt_AsLong(data);
        chkVal = HLNode_setScalarValue(self->node, sizeof(mData),
                                       (unsigned char*) &mData, hltypename, -1);
      } else if (typeSize <= sizeof(int)) {
        int mData = (int) PyInt_AsLong(data);
        chkVal = HLNode_setScalarValue(self->node, sizeof(mData),
                                       (unsigned char*) &mData, hltypename, -1);
      } else if (typeSize <= sizeof(long)) {
        long mData = (long) PyInt_AsLong(data);
        chkVal = HLNode_setScalarValue(self->node, sizeof(mData),
                                       (unsigned char*) &mData, hltypename, -1);
      } else {
        long long mData = (long long) PyLong_AsLongLong(data);
        if (PyErr_Occurred()) {
          setException(PyExc_AttributeError,"Could not translate to long long, probably not supported");
          goto fail;
        }
        chkVal = HLNode_setScalarValue(self->node, sizeof(mData),
                                       (unsigned char*) &mData, hltypename, -1);
      }
      break;
    }
    case H5T_FLOAT: {
      if (typeSize <= sizeof(float)) {
        float mData = (float) PyFloat_AsDouble(data);
        chkVal = HLNode_setScalarValue(self->node, sizeof(mData),
                                       (unsigned char*) &mData, hltypename, -1);
      } else if (typeSize <= sizeof(double)) {
        double mData = (double) PyFloat_AsDouble(data);
        chkVal = HLNode_setScalarValue(self->node, sizeof(mData),
                                       (unsigned char*) &mData, hltypename, -1);
      } else {
        fprintf(stderr, "Whoaa, greater float than double not supported\n");
      }
      break;
    }
    default: {
      char errmsg[256];
      sprintf(errmsg,
              "_pyhl_node_set_scalar_value Could not handle type in switch,"
                "type=%d\n", (int) H5Tget_class(tmpHid));
      setException(PyExc_TypeError,errmsg);
      goto fail;
    }
    }
    H5Tclose(tmpHid);
    tmpHid = -1;
    if (!chkVal) {
      setException(PyExc_TypeError,"Could not set scalar value");
      goto fail;
    }
  } else if (strcmp(hltypename, "string") == 0) {
    char* mData = PyString_AsString(data);
    if (!mData || !HLNode_setScalarValue(self->node, strlen(mData) + 1,
                                         (unsigned char*) mData, hltypename, -1)) {
      setException(PyExc_ValueError,"Could not set scalar string");
      goto fail;
    }
  } else if (strcmp(hltypename, "compound") == 0) {
    char* mData = PyString_AsString(data);
    if (!mData || !HLNode_setScalarValue(self->node, itemSize, (unsigned char*) mData,
                                         hltypename, lhid)) {
      setException(PyExc_ValueError,"Could not set scalar compound value");
      goto fail;
    }
  }

  if (tmpHid >= 0)
    H5Tclose(tmpHid);

  Py_INCREF(Py_None);
  return Py_None;
fail:
  if (tmpHid >= 0)
    H5Tclose(tmpHid);

  return NULL;
}

static PyObject* _pyhl_node_set_array_value(PyhlNode* self, PyObject* args)
{
  PyObject* pydims;
  PyObject* data;
  hid_t lhid = -1;
  char* hltypename;
  int itemSize;
  int n;
  hsize_t dims[4];
  int i;
  int ndim;
  PyObject* pyo = NULL;
  size_t tmpSize;

  if (!PyArg_ParseTuple(args, "iOOsi", &itemSize, &pydims, &data, &hltypename,
                        &lhid))
    return NULL;

  if (!self->node) {
    setException(PyExc_AttributeError,"The responsibility of the node has been dropped, probably by doing a addNode");
    return NULL;
  }

  if (PyObject_Length(pydims) > 4) {
    setException(PyExc_ValueError,"Not more than 4 dimensions are allowed");
    goto fail;
  }

  if (PyArray_Check(data) && HL_isFormatSupported(hltypename)) {
    /*Ahh plain array*/
    if (PyObject_Length(pydims) != ((PyArrayObject*) data)->nd) {
      setException(PyExc_ValueError,"Rank of Array != no of dims");
      goto fail;
    }
    ndim = PyObject_Length(pydims);
    for (i = 0; i < ndim; i++) { /*Extract dimensions*/
      if (!(pyo = PySequence_GetItem(pydims, i))) {
        setException(PyExc_AttributeError,"Could not get list item");
        goto fail;
      }
      dims[i] = PyInt_AsLong(pyo);
      Py_XDECREF(pyo);
      pyo=NULL;
      if(dims[i]!=((PyArrayObject*)data)->dimensions[i]) {
        setException(PyExc_AttributeError,"Array dimensions != list dimensions");
        goto fail;
      }
    }
    if((tmpSize = HL_sizeOfFormat(hltypename))<0) {
      setException(PyExc_ValueError,"Could not determine size");
      goto fail;
    }
    if(tmpSize!=((PyArrayObject*)data)->descr->elsize) {
      setException(PyExc_ValueError,"Type sizes are different between format and array");
      goto fail;
    }
    if(!HLNode_setArrayValue(self->node,tmpSize,ndim,dims,(unsigned char*)((PyArrayObject*)data)->data,
            hltypename,-1)) {
      setException(PyExc_AttributeError,"Could not set array data");
      goto fail;
    }
  } else if(PyList_Check(data) && (strcmp(hltypename,"string")==0 || HL_isFormatSupported(hltypename))) {
    /* Okie, list of something */
    int maxstrlen=0;
    char* tmpData=NULL;
    hid_t strtype=-1;

    if(PyObject_Length(pydims)!=1) {
      setException(PyExc_ValueError,"When using lists, the only allowed rank is 1.");
      goto fail;
    }
    n=PyObject_Length(data);
    if(strcmp(hltypename,"string")==0) {
      /*Calculate the longest string*/
      for(i=0;i<n;i++) {
        char* tmpstr;
        if(!(pyo=PySequence_GetItem(data,i))) {
          setException(PyExc_AttributeError,"Could not get list item");
          goto fail;
        }
        if(!(tmpstr = PyString_AsString(pyo))) {
          setException(PyExc_AttributeError,"Could not translate data to a string");
          goto fail;
        }
        if(strlen(tmpstr)>maxstrlen) {
          maxstrlen=strlen(tmpstr);
        }
        Py_XDECREF(pyo);
        pyo=NULL;
      }
      itemSize = maxstrlen + 1;
      if(!(tmpData=HLHDF_MALLOC(itemSize * n))) {
        setException(PyExc_MemoryError,"Could not allocate memory for strings");
        goto fail;
      }
      memset(tmpData,0,itemSize*n);
      for(i=0;i<n;i++) {
        char* tmpstr;
        pyo = PySequence_GetItem(data,i);
        tmpstr = PyString_AsString(pyo);
        strncpy(&tmpData[i*itemSize],tmpstr,strlen(tmpstr));
      }
      if ((strtype = H5Tcopy(H5T_C_S1))<0) {
        setException(PyExc_MemoryError,"Could not create a string type");
        HLHDF_FREE(tmpData);
        goto fail;
      }
      H5Tset_size(strtype, itemSize);
      dims[0]=n;
      if(!(HLNode_setArrayValue(self->node,itemSize,1,dims,(unsigned char*)tmpData,hltypename,strtype))) {
        setException(PyExc_AttributeError,"Could not set array data");
        H5Tclose(strtype);
        HLHDF_FREE(tmpData);
        goto fail;
      }
    } else {
      double* doubleptr=NULL;
      float* floatptr=NULL;
      int* intptr=NULL;
      long* longptr=NULL;
      if((tmpSize = HL_sizeOfFormat(hltypename))<0) {
        setException(PyExc_ValueError,"Could not determine size");
        goto fail;
      }
      if(strcmp(hltypename,"double")==0) {
        doubleptr = HLHDF_MALLOC(sizeof(double)*n);
        tmpData = (char*)doubleptr;
      } else if(strcmp(hltypename,"float")==0) {
        floatptr = HLHDF_MALLOC(sizeof(float)*n);
        tmpData = (char*)floatptr;
      } else if(strcmp(hltypename,"int")==0) {
        intptr = HLHDF_MALLOC(sizeof(int)*n);
        tmpData = (char*)intptr;
      } else if(strcmp(hltypename,"long")==0) {
        longptr = HLHDF_MALLOC(sizeof(long)*n);
        tmpData = (char*)longptr;
      } else {
        setException(PyExc_TypeError,"Only supported types on list are double,float,int and long");
        goto fail;
      }
      if(!tmpData) {
        setException(PyExc_MemoryError,"Could not allocate memory for datatype");
        goto fail;
      }

      for(i=0;i<n;i++) {
        if(!(pyo=PySequence_GetItem(data,i))) {
          setException(PyExc_AttributeError,"Could not get list item");
          HLHDF_FREE(tmpData);
          goto fail;
        }
        if(doubleptr) {
          doubleptr[i] = (double)PyFloat_AsDouble(pyo);
        } else if(floatptr) {
          floatptr[i] = (float)PyFloat_AsDouble(pyo);
        } else if(intptr) {
          intptr[i] = (int)PyInt_AsLong(pyo);
        } else if(longptr) {
          longptr[i] = (long)PyInt_AsLong(pyo);
        } else {
          setException(PyExc_AttributeError,"Whoa, something very strange happened");
          HLHDF_FREE(tmpData);
          goto fail;
        }
        Py_XDECREF(pyo);
        pyo=NULL;
      }
      dims[0]=n;
      if(!(HLNode_setArrayValue(self->node,tmpSize,1,dims,(unsigned char*)tmpData,hltypename,-1))) {
        setException(PyExc_AttributeError,"Could not set array data");
        HLHDF_FREE(tmpData);
        goto fail;
      }
    }
    HLHDF_FREE(tmpData);
  } else if(strcmp(hltypename,"compound")==0) {
    size_t tmpLen=0;
    char* tmpData=PyString_AsString(data);
    if(!tmpData) {
      setException(PyExc_AttributeError,"Could not translate data to a string");
      goto fail;
    }
    ndim=PyObject_Length(pydims);
    for(i=0;i<ndim;i++) {
      if(!(pyo=PySequence_GetItem(pydims,i))) {
        setException(PyExc_AttributeError,"Could not get list item");
        goto fail;
      }
      dims[i] = PyInt_AsLong(pyo);
      Py_XDECREF(pyo);
      pyo=NULL;
    }
    tmpLen=1;
    for(i=0;i<ndim;i++) {
      tmpLen*=dims[i];
    }
    tmpLen*=itemSize;
    if(PyObject_Length(data)!=tmpLen) {
      setException(PyExc_AttributeError,"There is some inconsistency between data size and calculated datasize");
      goto fail;
    }
    if(!HLNode_setArrayValue(self->node,itemSize,ndim,dims,(unsigned char*)tmpData,hltypename,lhid)) {
      setException(PyExc_AttributeError,"Could not set array data");
      goto fail;
    }
  } else {
    setException(PyExc_AttributeError,"Only support arrays and compound types right now");
    goto fail;
  }

  if(pyo) {
    Py_XDECREF(pyo);
  }
  Py_INCREF(Py_None);
  return Py_None;
fail:
  if(pyo) {
    Py_XDECREF(pyo);
  }
  return NULL;
}

static PyObject* _pyhl_node_commit(PyhlNode* self, PyObject* args)
{
  hid_t dType;
  if (!PyArg_ParseTuple(args, "i", &dType))
    return NULL;
  if (HLNode_getType(self->node) != TYPE_ID) {
    setException(PyExc_AttributeError,"Trying to commit a node which not is a type node");
    return NULL;
  }
  HLNode_commitType(self->node, dType);
  Py_INCREF(Py_None);
  return Py_None;
}


static PyObject* _pyhl_node_name(PyhlNode* self, PyObject* args)
{
  return PyString_FromString(HLNode_getName(self->node));
}

static PyObject* _pyhl_node_type(PyhlNode* self, PyObject* args)
{
  return PyInt_FromLong(HLNode_getType(self->node));
}

static PyObject* _pyhl_node_dims(PyhlNode* self, PyObject* args)
{
  PyObject* retv = NULL;
  int i;
  PyObject* pyo = NULL;
  if (!(retv = PyList_New(0))) {
    return NULL;
  }

  for (i = 0; i < HLNode_getRank(self->node); i++) {
    if (!(pyo = PyInt_FromLong(HLNode_getDimension(self->node, i)))) {
      setException(PyExc_ValueError,"Could not create py integer");
      goto fail;
    }
    if (PyList_Append(retv, pyo) == -1) {
      setException(PyExc_ValueError,"Could not append list item");
      goto fail;
    }
    Py_XDECREF(pyo);
    pyo=NULL;
  }
  Py_XDECREF(pyo);
  return retv;
fail:
  Py_XDECREF(pyo);
  Py_XDECREF(retv);
  return NULL;
}

static PyObject* _pyhl_node_format(PyhlNode* self, PyObject* args)
{
  return PyString_FromString(HLNode_getFormatName(self->node));
}

static PyObject* _pyhl_node_data(PyhlNode* self, PyObject* args)
{
  PyObject* retv = NULL;
  hid_t tmpHid = -1;
  size_t typeSize;
  char errbuf[256];
  npy_intp* dims = NULL;
  int i;
  size_t npts;

  if (HLNodePrivate_getTypeId(self->node) >= 0) {
    tmpHid = H5Tcopy(HLNodePrivate_getTypeId(self->node));
  } else if (HL_isFormatSupported(HLNode_getFormatName(self->node))) {
    tmpHid = HL_translateFormatStringToDatatype(HLNode_getFormatName(self->node));
  } else {
    setException(PyExc_AttributeError,"Strange type, can't handle");
    return NULL;
  }

  typeSize = H5Tget_size(tmpHid);

  if (HLNode_getRank(self->node) == 0) { /*Scalar*/
    switch (H5Tget_class(tmpHid)) {
    case H5T_INTEGER: {
      if (typeSize <= sizeof(char)) {
        char v;
        memcpy(&v, HLNode_getData(self->node), typeSize);
        retv = PyInt_FromLong((long) v);
      } else if (typeSize <= sizeof(short)) {
        short v;
        memcpy(&v, HLNode_getData(self->node), typeSize);
        retv = PyInt_FromLong((long) v);
      } else if (typeSize <= sizeof(int)) {
        int v;
        memcpy(&v, HLNode_getData(self->node), typeSize);
        retv = PyInt_FromLong((long) v);
      } else if (typeSize <= sizeof(long)) {
        long v;
        memcpy(&v, HLNode_getData(self->node), typeSize);
        retv = PyInt_FromLong((long) v);
      } else {
        long long v;
        memcpy(&v, HLNode_getData(self->node), typeSize);
        retv = PyLong_FromLongLong(v);
      }
      break;
    }
    case H5T_FLOAT: {
      if (typeSize <= sizeof(float)) {
        float v;
        memcpy(&v, HLNode_getData(self->node), typeSize);
        retv = PyFloat_FromDouble((double) v);
      } else if (typeSize <= sizeof(double)) {
        double v;
        memcpy(&v, HLNode_getData(self->node), typeSize);
        retv = PyFloat_FromDouble(v);
      } else {
        fprintf(stderr, "Whoaa, greater float than double not supported\n");
      }
      break;
    }
    case H5T_COMPOUND: {
      retv = PyString_FromStringAndSize((char*)HLNode_getData(self->node), typeSize);
      break;
    }
    case H5T_STRING: {
      char* d = (char*)HLNode_getData(self->node);
      if (H5Tis_variable_str(tmpHid)) { /* You can't trust typeSize when variable length. The size will always be size of pointer */
        typeSize = strlen(d);
      }
      if (d[typeSize-1] == '\0') {
        retv = PyString_FromStringAndSize((char*)HLNode_getData(self->node), typeSize - 1);
      } else {
        retv = PyString_FromStringAndSize((char*)HLNode_getData(self->node), typeSize);
      }
      break;
    }
    default: {
      sprintf(errbuf, "Unrecognized class %d", (int) H5Tget_class(tmpHid));
      setException(PyExc_TypeError,errbuf);
      break;
    }
    }
  } else { /*Simple*/
    if (!(dims = HLHDF_MALLOC(sizeof(npy_intp) * HLNode_getRank(self->node)))) {
      setException(PyExc_MemoryError,"Could not allocate dims");
      goto fail;
    }
    switch (H5Tget_class(tmpHid)) {
    case H5T_INTEGER:
    case H5T_FLOAT: {
      int iformat = pyarraytypeFromHdfType(HLNode_getFormatName(self->node));
      int nbytes;
      if (iformat == -1) {
        sprintf(errbuf, "Unrecognized datatype %s", HLNode_getFormatName(self->node));
        setException(PyExc_TypeError,errbuf);
        goto fail;
      }
      for (i = 0; i < HLNode_getRank(self->node); i++)
        dims[i] = (npy_intp) HLNode_getDimension(self->node, i);

      retv = PyArray_SimpleNew(HLNode_getRank(self->node), dims, iformat);
      if (!retv) {
        setException(PyExc_MemoryError,"Could not create array");
        goto fail;
      }
      nbytes = (int)HLNode_getNumberOfPoints(self->node);
      nbytes *= ((PyArrayObject*) retv)->descr->elsize;

      memcpy(((PyArrayObject*) retv)->data, (unsigned char*) HLNode_getData(self->node),
             nbytes);
      break;
    }
    case H5T_COMPOUND: {
      npts = (size_t)HLNode_getNumberOfPoints(self->node);
      npts *= typeSize;
      retv = PyString_FromStringAndSize((char*) HLNode_getData(self->node), npts);
      break;
    }
    case H5T_STRING: {
      if (HLNode_getRank(self->node) != 1) {
        /* Don't know how to represent a multi-dim array of strings */
        npts = (size_t)HLNode_getNumberOfPoints(self->node);
        npts *= typeSize;
        retv = PyString_FromStringAndSize((char*) HLNode_getData(self->node), npts - 1);
      } else {
        const unsigned char* data = HLNode_getData(self->node);
        retv = PyList_New(0);
        for (i = 0; retv && i < HLNode_getDimension(self->node, 0); i++) {
          PyObject* pyo =
              PyString_FromStringAndSize((char*) (&data[i * typeSize]), typeSize - 1);
          if (!pyo) {
            setException(PyExc_MemoryError,"Could not create string");
            goto fail;
          }
          if (PyList_Append(retv, pyo) == -1) {
            Py_XDECREF(pyo);
            goto fail;
          }
          Py_XDECREF(pyo);
        }
      }
      break;
    }
    default: {
      sprintf(errbuf,"Unrecognized class %d",(int)H5Tget_class(tmpHid));
      setException(PyExc_TypeError,errbuf);
      break;
    }
    }
  }

  HLHDF_FREE(dims);
  HL_H5T_CLOSE(tmpHid);
  return retv;
fail:
  Py_XDECREF(retv);
  HLHDF_FREE(dims);
  HL_H5T_CLOSE(tmpHid);
  return NULL;
}

static PyObject* _pyhl_node_rawdata(PyhlNode* self, PyObject* args)
{
  PyObject* retv = NULL;
  hid_t tmpHid = -1;
  size_t typeSize;
  char errbuf[256];
  npy_intp* dims = NULL;
  int i;
  size_t npts;

  if (HLNode_getRawdata(self->node) == NULL) {
    setException(PyExc_AttributeError,"Rawdata has not been read for this node");
    return NULL;
  }

  if (HLNodePrivate_getTypeId(self->node) >= 0) {
    tmpHid = H5Tcopy(HLNodePrivate_getTypeId(self->node));
  } else if (HL_isFormatSupported(HLNode_getFormatName(self->node))) {
    tmpHid = HL_translateFormatStringToDatatype(HLNode_getFormatName(self->node));
  } else {
    setException(PyExc_AttributeError,"Strange type, can't handle");
    return NULL;
  }

  typeSize = HLNode_getRawdataSize(self->node); /* Using raw type */

  if (HLNode_getRank(self->node) == 0) { /*Scalar*/
    switch (H5Tget_class(tmpHid)) {
    case H5T_INTEGER: {
      if (typeSize <= sizeof(char)) {
        char v;
        memcpy(&v, HLNode_getRawdata(self->node), typeSize);
        retv = PyInt_FromLong((long) v);
      } else if (typeSize <= sizeof(short)) {
        short v;
        memcpy(&v, HLNode_getRawdata(self->node), typeSize);
        retv = PyInt_FromLong((long) v);
      } else if (typeSize <= sizeof(int)) {
        int v;
        memcpy(&v, HLNode_getRawdata(self->node), typeSize);
        retv = PyInt_FromLong((long) v);
      } else if (typeSize <= sizeof(long)) {
        long v;
        memcpy(&v, HLNode_getRawdata(self->node), typeSize);
        retv = PyInt_FromLong((long) v);
      } else {
        long long v;
        memcpy(&v, HLNode_getRawdata(self->node), typeSize);
        retv = PyLong_FromLongLong(v);
      }
      break;
    }
    case H5T_FLOAT: {
      if (typeSize <= sizeof(float)) {
        float v;
        memcpy(&v, HLNode_getRawdata(self->node), typeSize);
        retv = PyFloat_FromDouble((double) v);
      } else if (typeSize <= sizeof(double)) {
        double v;
        memcpy(&v, HLNode_getRawdata(self->node), typeSize);
        retv = PyFloat_FromDouble(v);
      } else {
        fprintf(stderr, "Whoaa, greater float than double not supported\n");
      }
      break;
    }
    case H5T_COMPOUND: {
      retv = PyString_FromStringAndSize((char*) HLNode_getRawdata(self->node), typeSize);
      break;
    }
    case H5T_STRING: {
      retv = PyString_FromStringAndSize((char*) HLNode_getRawdata(self->node), typeSize);
      break;
    }
    default: {
      sprintf(errbuf, "Unrecognized class %d", (int) H5Tget_class(tmpHid));
      setException(PyExc_TypeError,errbuf);
      break;
    }
    }
  } else { /*Simple*/
    if (!(dims = HLHDF_MALLOC(sizeof(npy_intp) * HLNode_getRank(self->node)))) {
      setException(PyExc_MemoryError,"Could not allocate dims");
      goto fail;
    }
    switch (H5Tget_class(tmpHid)) {
    case H5T_INTEGER:
    case H5T_FLOAT: {
      int iformat = pyarraytypeFromHdfType(HLNode_getFormatName(self->node));
      int nbytes;
      if (iformat == -1) {
        sprintf(errbuf, "Unrecognized datatype %s", HLNode_getFormatName(self->node));
        setException(PyExc_TypeError,errbuf);
        goto fail;
      }
      for (i = 0; i < HLNode_getRank(self->node); i++)
        dims[i] = (npy_intp) HLNode_getDimension(self->node,i);

      retv = PyArray_SimpleNew(HLNode_getRank(self->node), dims, iformat);
      if (!retv) {
        setException(PyExc_MemoryError,"Could not create array");
        goto fail;
      }
      nbytes = (int)HLNode_getNumberOfPoints(self->node);
      nbytes *= ((PyArrayObject*) retv)->descr->elsize;
      memcpy(((PyArrayObject*) retv)->data,
             HLNode_getRawdata(self->node), nbytes);
      break;
    }
    case H5T_COMPOUND: {
      npts = (size_t)HLNode_getNumberOfPoints(self->node);;
      npts *= typeSize;
      retv = PyString_FromStringAndSize((char*) HLNode_getRawdata(self->node), npts);
      break;
    }
    case H5T_STRING: {
      if (HLNode_getRank(self->node) != 1) {
        /* Don't know how to represent a multi-dim array of strings */
        npts = 1;
        for (i = 0; i < HLNode_getRank(self->node); i++)
          npts *= HLNode_getDimension(self->node,i);
        npts *= typeSize;
        retv = PyString_FromStringAndSize((char*) HLNode_getRawdata(self->node), npts - 1);
      } else {
        const unsigned char* data = HLNode_getRawdata(self->node);
        retv = PyList_New(0);
        for (i = 0; retv && i < HLNode_getDimension(self->node, 0); i++) {
          PyObject* pyo =
              PyString_FromStringAndSize((char*) (&data[i * typeSize]), typeSize - 1);
          if (!pyo) {
            setException(PyExc_MemoryError,"Could not create string");
            goto fail;
          }
          if (PyList_Append(retv, pyo) == -1) {
            Py_XDECREF(pyo);
            goto fail;
          }
          Py_XDECREF(pyo);
        }
      }
      break;
    }
    default: {
      sprintf(errbuf,"Unrecognized class %d",(int)H5Tget_class(tmpHid));
      setException(PyExc_TypeError,errbuf);
      break;
    }
    }
  }
  HLHDF_FREE(dims);
  if(tmpHid>=0)
    H5Tclose(tmpHid);
  return retv;
fail:
  Py_XDECREF(retv);
  HLHDF_FREE(dims);
  if(tmpHid>=0)
    H5Tclose(tmpHid);
  return NULL;
}

static PyObject* getPythonObjectFromNode(HL_CompoundTypeAttribute* descr,
  const unsigned char* data, int idx)
{
  PyObject* pyo = NULL;

  int offset = descr->offset + idx * descr->size;

  if (strcmp(descr->format, "string") == 0) {
    int len = strlen((char*) &data[offset]);
    char* dummystr = "";
    if (len > 0 && len <= descr->size) {
      pyo = PyString_FromStringAndSize((char*) &data[offset], len);
    } else if (len <= 0) {
      pyo = PyString_FromString(dummystr);
    } else if (len >= descr->size) {
      pyo = PyString_FromStringAndSize((char*) &data[offset], descr->size);
    }
  } else if (strcmp(descr->format, "schar") == 0 ||
      strcmp(descr->format, "uchar") == 0 || strcmp(descr->format, "char") == 0) {
    char val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyString_FromStringAndSize(&val, 1);
  } else if (strcmp(descr->format, "short") == 0 ||
      strcmp(descr->format, "ushort") == 0) {
    short val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyInt_FromLong((long) val);
  } else if (strcmp(descr->format, "int") == 0 ||
      strcmp(descr->format, "uint") == 0) {
    int val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyInt_FromLong((long) val);
  } else if (strcmp(descr->format, "long") == 0 ||
      strcmp(descr->format, "ulong") == 0) {
    long val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyInt_FromLong((long) val);
  } else if (strcmp(descr->format, "llong") == 0) {
    long long val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyInt_FromLong((long) val);
  } else if (strcmp(descr->format, "float") == 0) {
    float val = 0.0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyFloat_FromDouble((double) val);
  } else if (strcmp(descr->format, "double") == 0) {
    double val = 0.0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyFloat_FromDouble((double) val);
  } else if (strcmp(descr->format, "ldouble") == 0) {
    double val = 0.0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyFloat_FromDouble((double) val);
  } else if (strcmp(descr->format, "hsize") == 0) {
    hsize_t val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyInt_FromLong((long) val);
  } else if (strcmp(descr->format, "hssize") == 0) {
    hssize_t val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyInt_FromLong((long) val);
  } else if (strcmp(descr->format, "herr") == 0) {
    herr_t val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyInt_FromLong((long) val);
  } else if (strcmp(descr->format, "hbool") == 0) {
    hbool_t val = 0;
    memcpy((unsigned char*) &val, &data[offset], descr->size);
    pyo = PyInt_FromLong((long) val);
  } else {
    char errmsg[256];
    sprintf(errmsg, "Unsupported data type '%s'\n", descr->format);
    setException(PyExc_AttributeError,errmsg);
  }
  return pyo;
}

static PyObject* _pyhl_node_get_compound_data(PyhlNode* self, PyObject* args)
{
  hid_t tmpHid = -1;
  PyObject* retv = NULL;
  PyObject *pyo = NULL, *pyo2 = NULL;
  int i, j;
  const unsigned char* data;
  HL_CompoundTypeDescription* descr;

  if (HLNodePrivate_getTypeId(self->node) >= 0) {
    tmpHid = H5Tcopy(HLNodePrivate_getTypeId(self->node));
  } else if (HL_isFormatSupported(HLNode_getFormatName(self->node))) {
    tmpHid = HL_translateFormatStringToDatatype(HLNode_getFormatName(self->node));
  } else {
    setException(PyExc_AttributeError,"Strange type, can't handle");
    return NULL;
  }

  if (H5Tget_class(tmpHid) != H5T_COMPOUND) {
    setException(PyExc_AttributeError,"Only supposed to be used with composite nodes");
    goto fail;
  }

  if (HLNode_getRank(self->node) == 0 || (HLNode_getRank(self->node) == 1 && HLNode_getDimension(self->node,0) == 1)) { /*Scalar*/
    if (!HLNode_getCompoundDescription(self->node)) {
      setException(PyExc_AttributeError,"Node does not have a compound description");
      goto fail;
    }
    if (!(retv = PyDict_New())) {
      setException(PyExc_MemoryError,"Could not allocate dicionary\n");
      goto fail;
    }
    data = HLNode_getData(self->node);
    descr = HLNode_getCompoundDescription(self->node);
    for (i = 0; i < descr->nAttrs; i++) {
      pyo = NULL;
      if (descr->attrs[i]->ndims == 0 || (descr->attrs[i]->ndims == 1 && descr->attrs[i]->dims[0] == 1)) {
        if (!(pyo = getPythonObjectFromNode(descr->attrs[i], data, 0))) {
          goto fail;
        }
      } else {
        int ndim = 1;
        if (!(pyo = PyList_New(0))) {
          goto fail;
        }
        for (j = 0; j < descr->attrs[i]->ndims; j++) {
          ndim *= descr->attrs[i]->dims[j];
        }
        for (j = 0; j < ndim; j++) {
          if (!(pyo2 = getPythonObjectFromNode(descr->attrs[i], data, j))) {
            goto fail;
          }
          PyList_Append(pyo, pyo2);
          Py_XDECREF(pyo2);
          pyo2 = NULL;
        }
      }

      if(PyDict_SetItemString(retv,descr->attrs[i]->attrname,pyo)!=0) {
        goto fail;
      }
      Py_XDECREF(pyo);
      pyo = NULL;
    }
  } else {
    setException(PyExc_AttributeError,"Method does not support arrays of compound objects yet.");
    goto fail;
  }
  Py_XDECREF(pyo);
  Py_XDECREF(pyo2);
  HL_H5T_CLOSE(tmpHid);
  return retv;
fail:
  Py_XDECREF(retv);
  Py_XDECREF(pyo);
  Py_XDECREF(pyo2);
  HL_H5T_CLOSE(tmpHid);
  return NULL;
}

static PyObject* _pyhl_node_get_compound_type(PyhlNode* self, PyObject* args)
{
  PyObject* retv = NULL;

  if (HLNodePrivate_getTypeId(self->node) < 0 ||
      H5Tget_class(HLNodePrivate_getTypeId(self->node)) != H5T_COMPOUND) {
    setException(PyExc_AttributeError, "This is not a compound type");
    goto fail;
  }

  if (H5Tcommitted(HLNodePrivate_getTypeId(self->node)) > 0) {
    if (HLNode_getCompoundDescription(self->node) == NULL) {
      setException(PyExc_AttributeError,"Node does not have a compound description");
      goto fail;
    }
    retv = PyString_FromString(HLNode_getCompoundDescription(self->node)->hltypename);
  } else {
    Py_INCREF(Py_None);
    retv = Py_None; // So that we can return retv
  }

fail:
  return retv;
}

/**
 * @addtogroup pyhl_api
 * \section _pyhl_nodelist_interfaces _pyhl nodelist interfaces
\verbatim
Function: addNode(node)
  Adds the provided node to the node list
Parameters:
  node - the node to add to the node list
Returns:
  N/A.

Function: write(filename, compression=None)
Parameters:
  filename - the full path of the HDF5 file to be written
  compression - Optional compression object
Returns:
  N/A.

Function: update(compression=None)
Parameters:
  compression - Optional compression object
Returns:
  N/A.

Function: getNodeNames()
Returns:
  A list of all node names that exists in the nodelist.

Function: selectAll()
  Marks all nodes for data reading when executing fetch()
Returns:
  N/A.

Function: selectMetadata()
  Marks all nodes (except the actual dataset) for data reading when executing fetch()
Returns:
  N/A.

Function: HLNodeList_selectNode(name)
  Marks the specified node for retrival when executing fetch
Parameters:
  name - the node that should be marked for retrival
Returns:
  N/A.

Function: HLNodeList_deselectNode(name)
  Unmarks the specified node so that it is not read when executing fetch.
Parameters:
  name - the node that should be deselected
Returns:
  N/A.

Function: fetch()
  Reads the data for all nodes that has been marked for selection.
Returns:
  N/A.

Function: HLNodeList_fetchNode()
  Reads the data for the specified node and returns it.
Returns:
  The read node.

Function: getNode(name)
  Reads the data for the specified node and returns it.
Parameters:
  name - the node that should be returned. Note, that if node not has been fetched it will not contain any data.
Returns:
  The read node.

\endverbatim
*/
static struct PyMethodDef methods[] =
{
  { "addNode", (PyCFunction) _pyhl_add_node, 1 },
  { "write", (PyCFunction) _pyhl_write, 1 },
  { "update", (PyCFunction) _pyhl_update, 1 },
  { "getNodeNames", (PyCFunction) _pyhl_get_node_names, 1 },
  { "selectAll", (PyCFunction) _pyhl_select_all, 1 },
  { "selectMetadata", (PyCFunction) _pyhl_select_metadata, 1 },
  { "selectNode", (PyCFunction) _pyhl_select_node, 1 },
  { "deselectNode", (PyCFunction) _pyhl_deselect_node, 1 },
  { "fetch", (PyCFunction) _pyhl_fetch, 1 },
  { "fetchNode", (PyCFunction) _pyhl_fetch_node, 1 },
  { "getNode", (PyCFunction) _pyhl_get_node, 1 },
  { NULL, NULL } /* sentinel */
};

/**
 * @addtogroup pyhl_api
 * \section _pyhl_node_interfaces _pyhl node interfaces
\verbatim
Function: setScalarValue(itemsize, data, format, hid)
  Sets a scalar value in the node instance.
Parameters:
  itemsize - specifies the size of the value in bytes. It is not nessecary to specify unless
             a compound type is set. So, usually it is just to set it to -1.
  data - is the data to be set in the node.
  format - is the HL-HDF string representation of the datatype.
  hid - is the hid_t reference to the datatype, is not nessecary to
        specify unless a compound type is set. Usually, just set it to -1.

  NOTE: If the data to be set is of compound type, then the data should be of string type.
  NOTE: If the node is a Reference node, the data should be set as a string type, where the
        data is the name of the referenced node.

Returns:
  N/A.

Function: setArrayValue(itemsize, dims, data, format, hid)
  Sets an array value in the node instance.
Parameters:
  itemsize - specifies the size of the value in bytes. It is not nessecary to specify unless
             a compound type is set. So, usually it is just to set it to -1.
  dims - is a list of dimensions of the data.
  data - is the data to be set in the node.
  format - is the HL-HDF string representation of the datatype.
  hid - is the hid_t reference to the datatype, is not nessecary to
        specify unless a compound type is set. Usually, just set it to -1.

  NOTE: If the data to be set is of compound type, then the data should be of string type.

Returns:
  N/A.

Function: commit(datatype)
  Marks a node of type=TYPE_ID to be committed (named).
Parameters:
  datatype - is the hid_t reference to the datatype.

Returns:
  N/A.

Function: name()
  Returns the name of the node instance.
Returns:
  the name of the node.

Function: type()
  Returns the type of the node instance..
Returns:
  the type of the node.

Function: dims()
  Returns a list of the dimensions of the data.
Returns:
  the dimensions

Function: format()
  Returns the HL-HDF format specifier name
Returns:
  the format specifier

Function: data()
  Returns the data in fixed format (native).
  NOTE: If the data is of compound type, the data will be returned as a string.
Returns:
  the data in native format.

Function: rawdata()
  Returns the raw data (as read without conversion to native format).
  NOTE: If the data is of compound type, the data will be returned as a string.
Returns:
  the data in raw format.

Function: compound_data()
  Returns a dictionary with all attributes in the compund attribute, it only
  works if the node instance is a compound attribute.
Returns:
  the compound data as a dictionary

\endverbatim
 */
static struct PyMethodDef node_methods[] =
{
  { "setScalarValue", (PyCFunction) _pyhl_node_set_scalar_value, 1 },
  { "setArrayValue", (PyCFunction) _pyhl_node_set_array_value, 1 },
  { "commit", (PyCFunction) _pyhl_node_commit, 1 },

  /* Inquiry options */
  { "name", (PyCFunction) _pyhl_node_name, 1 },
  { "type", (PyCFunction) _pyhl_node_type, 1 },
  { "dims", (PyCFunction) _pyhl_node_dims, 1 },
  { "format", (PyCFunction) _pyhl_node_format, 1 },
  { "data", (PyCFunction) _pyhl_node_data, 1 },
  { "rawdata", (PyCFunction) _pyhl_node_rawdata, 1 },
  { "compound_data", (PyCFunction) _pyhl_node_get_compound_data, 1 },
  { "compound_type", (PyCFunction) _pyhl_node_get_compound_type, 1 },
  { NULL, NULL } /* sentinel */
};

/**
 * @addtogroup pyhl_api
 * \section _pyhl_filecreationproperty_interfaces _pyhl filecreationproperty interfaces
 * The filecreationproperty object does only contain various properties that is possible
 * to set. Depending on how the file should be created, there are different parameters
 * that can be used.
 *
 * \li <b>version</b>: This parameter is only for retrival, i.e it is not possible to change.
 * \li <b>userblock</b>: See the hdf5 documentation for purpose of userblock.
 * \li <b>sizes</b>: Is specified as a tuple of 2, (sizeof_addr,sizeof_size), see hdf5
 * documentation for further information.
 * \li <b>sym_k</b>: Is specified as a tuple of 2, (ik,lk), see hdf5 documentation for
 * further information.
 * \li <b>istore_k</b>: See hdf5 documentation for further information.
 * \li <b>meta_block_size</b>: This is actually a file access property but have been inserted
 * here anyway, If the value is set to 2048, then the default file access property will
 * be used. For more information about meta_block_size, see the hdf5 documentation.
 */
static struct PyMethodDef filecreationproperty_methods[] =
{
  { "version", NULL },
  { "userblock", NULL },
  { "sizes", NULL },
  { "sym_k", NULL },
  { "istore_k", NULL },
  { "meta_block_size", NULL },
  { NULL, NULL }
};

/**
 * @addtogroup pyhl_api
 * \section _pyhl_compression_interfaces _pyhl compression interfaces
 * The compression object does only contain various properties that
 * is possible to set. Depending on what compression type that was
 * specified when creating this object there are different parameters
 * that can be used.
 * If ZLIB_COMPRESSION was specified the only property that can be
 * used is: \n
 * \li <b>level</b>: Specify level of compression between 0-9 where <b>0</b> means
 * no compression and <b>9</b> means highest compression.
 *
 * If SZLIB_COMPRESSION was specified, there are two properties that can be set.
 *
 * \li <b>szlib_mask</b>: This affects how the compression should be performed and
 * can can be set up from two different sets of options.
 * <table>
 * <tr><td>H5_SZIP_CHIP_OPTION_MASK</td><td>Compresses exactly as in hardware.</td></tr>
 * <tr><td>H5_SZIP_ALLOW_K13_OPTION_MASK</td><td>Allows k split = 13 compression mode. (Default)</td></tr>
 * <tr><td>&nbsp;</td><td>&nbsp;</td></tr>
 * <tr><td>H5_SZIP_EC_OPTION_MASK</td><td>Selects entropy coding method. (Default)</td></tr>
 * <tr><td>H5_SZIP_NN_OPTION_MASK</td><td>Selects nearest neighbor coding method.</td></tr>
 * </table>
 * Where the paired options are mutual exclusive, i.e. it is possible
 * to set the szlib_mask to H5_SZIP_CHIP_OPTION_MASK|H5_SZIP_EC_OPTION_MASK
 * but not to H5_SZIP_CHIP_OPTION_MASK|H5_SZIP_ALLOW_K13_OPTION_MASK.
 *
 * \li <b>szlib_px_per_block</b>: The block size must be even, with typical values
 * being 8,10,16 and 32. The more pixel values vary, the smaller this number should be.
 */
static struct PyMethodDef compression_methods[] =
{
  { "type", NULL },
  { "level", NULL },
  { "szlib_mask", NULL },
  { "szlib_px_per_block", NULL },
  { "H5_SZIP_CHIP_OPTION_MASK", NULL },
  { "H5_SZIP_ALLOW_K13_OPTION_MASK", NULL },
  { "H5_SZIP_EC_OPTION_MASK", NULL },
  { "H5_SZIP_NN_OPTION_MASK", NULL },
  { NULL, NULL }
};

static PyObject* _getattr(PyhlNodelist* self, char* name)
{
  PyObject* res;

  if (strcmp(name, "ATTRIBUTE_ID") == 0)
    return PyInt_FromLong(ATTRIBUTE_ID);
  if (strcmp(name, "GROUP_ID") == 0)
    return PyInt_FromLong(GROUP_ID);
  if (strcmp(name, "DATASET_ID") == 0)
    return PyInt_FromLong(DATASET_ID);
  if (strcmp(name, "TYPE_ID") == 0)
    return PyInt_FromLong(TYPE_ID);
  if (strcmp(name, "REFERENCE_ID") == 0)
    return PyInt_FromLong(REFERENCE_ID);

  res = Py_FindMethod(methods, (PyObject*) self, name);
  if (res)
    return res;

  PyErr_Clear();

  PyErr_SetString(PyExc_AttributeError, name);

  return NULL;
}

static PyObject* _getattr_node(PyhlNode* self, char* name)
{
  PyObject* res;

  res = Py_FindMethod(node_methods, (PyObject*) self, name);
  if (res)
    return res;

  PyErr_Clear();

  PyErr_SetString(PyExc_AttributeError, name);

  return NULL;
}

static PyObject* _getattr_filecreationproperty(PyhlFileCreationProperty* self,
  char* name)
{
  PyObject* res;

  if (strcmp(name, "version") == 0) {
    return Py_BuildValue("(iiii)", self->props->version.super,
                         self->props->version.freelist,
                         self->props->version.stab, self->props->version.shhdr);
  } else if (strcmp(name, "userblock") == 0) {
    return PyInt_FromLong(self->props->userblock);
  } else if (strcmp(name, "sizes") == 0) {
    return Py_BuildValue("(ii)", self->props->sizes.sizeof_addr,
                         self->props->sizes.sizeof_size);
  } else if (strcmp(name, "sym_k") == 0) {
    return Py_BuildValue("(ii)", self->props->sym_k.ik, self->props->sym_k.lk);
  } else if (strcmp(name, "istore_k") == 0) {
    return PyInt_FromLong(self->props->istore_k);
  } else if (strcmp(name, "meta_block_size") == 0) {
    return PyInt_FromLong(self->props->meta_block_size);
  }

  res = Py_FindMethod(filecreationproperty_methods, (PyObject*) self, name);
  if (res)
    return res;

  PyErr_Clear();

  PyErr_SetString(PyExc_AttributeError, name);

  return NULL;
}

static int _setattr_filecreationproperty(PyhlFileCreationProperty* self,
  char* name, PyObject* val)
{
  char errmsg[256];
  if (strcmp(name, "userblock") == 0) {
    hsize_t tmpv;
    hsize_t tmpv2;
    hsize_t mask = 1;
    int found_setbit = 0;
    int number_of_shifts = sizeof(hsize_t) * 8;
    int i;
    tmpv = PyInt_AsLong(val);
    if (tmpv < 512 && tmpv != 0) {
      setException(PyExc_AttributeError,"The userblock size should be set to 0 or any power of 2 greater or equal to 512\n");
      return -1;
    }
    tmpv2 = tmpv;
    for (i = 0; i < number_of_shifts; i++) {
      if (tmpv2 & mask) {
        if (found_setbit) {
          setException(PyExc_AttributeError,"The userblock size should be set to 0 or any power of 2 greater or equal to 512\n");
          return -1;
        }
        found_setbit = 1;
      }
      tmpv2 = tmpv2 >> 1;
    }
    self->props->userblock = tmpv;
    return 0;
  } else if (strcmp(name, "sizes") == 0) {
    int sia, sis;
    if (!PyArg_ParseTuple(val, "ii", &sia, &sis)) {
      setException(PyExc_AttributeError,"sizes should be set with a tuble (sizeof_addr,sizeof_size)\n");
      return -1;
    }

    if (sia != 0 && sia != 2 && sia != 4 && sia != 8 && sia != 16) {
      setException(PyExc_AttributeError,"first argument in sizes (sizeof_addr) must be one of 0,2,4,8 or 16\n");
      return -1;
    }
    if (sis != 0 && sis != 2 && sis != 4 && sis != 8 && sis != 16) {
      setException(PyExc_AttributeError,"second argument in sizes (sizeof_size) must be one of 0,2,4,8 or 16\n");
      return -1;
    }
    self->props->sizes.sizeof_size = (size_t)sis;
    self->props->sizes.sizeof_addr = (size_t)sia;
    return 0;
  } else if (strcmp(name, "sym_k") == 0) {
    int ik, lk;
    if (!PyArg_ParseTuple(val, "ii", &ik, &lk)) {
      setException(PyExc_AttributeError,"sym_k should be set with a tuple (ik,lk)\n");
      return -1;
    }
    if (ik != 0)
      self->props->sym_k.ik = ik;
    if (lk != 0)
      self->props->sym_k.lk = lk;
    return 0;
  } else if (strcmp(name, "istore_k") == 0) {
    self->props->istore_k = PyInt_AsLong(val);
    return 0;
  } else if (strcmp(name, "meta_block_size") == 0) {
    self->props->meta_block_size = PyInt_AsLong(val);
    return 0;
  }

  sprintf(errmsg,
          "It is not possible to set '%s' in the filecreationproperty instance\n",
          name);
  setException(PyExc_AttributeError,errmsg);

  return -1;
}

static PyObject* _getattr_compression(PyhlCompression* self, char* name)
{
  PyObject* res;

  if (strcmp(name, "type") == 0) {
    return PyInt_FromLong(self->compr->type);
  } else if (strcmp(name, "level") == 0) {
    return PyInt_FromLong(self->compr->level);
  } else if (strcmp(name, "szlib_mask") == 0) {
    return PyInt_FromLong(self->compr->szlib_mask);
  } else if (strcmp(name, "szlib_px_per_block") == 0) {
    return PyInt_FromLong(self->compr->szlib_px_per_block);
  } else if (strcmp(name, "H5_SZIP_CHIP_OPTION_MASK") == 0) {
    return PyInt_FromLong(H5_SZIP_CHIP_OPTION_MASK);
  } else if (strcmp(name, "H5_SZIP_ALLOW_K13_OPTION_MASK") == 0) {
    return PyInt_FromLong(H5_SZIP_ALLOW_K13_OPTION_MASK);
  } else if (strcmp(name, "H5_SZIP_EC_OPTION_MASK") == 0) {
    return PyInt_FromLong(H5_SZIP_EC_OPTION_MASK);
  } else if (strcmp(name, "H5_SZIP_NN_OPTION_MASK") == 0) {
    return PyInt_FromLong(H5_SZIP_NN_OPTION_MASK);
  }

  res = Py_FindMethod(compression_methods, (PyObject*) self, name);
  if (res)
    return res;

  PyErr_Clear();

  PyErr_SetString(PyExc_AttributeError, name);

  return NULL;
}

static int _setattr_compression(PyhlCompression* self, char* name,
  PyObject* val)
{
  char errmsg[256];
  if (strcmp(name, "level") == 0) {
    int tmpv;
    if (self->compr->type != CT_ZLIB) {
      setException(PyExc_AttributeError,"level is only usable when compression is of type COMPRESSION_ZLIB\n");
      return -1;
    }
    tmpv = PyInt_AsLong(val);
    if (tmpv < 0 || tmpv > 9) {
      setException(PyExc_AttributeError,"level must be specified as an integer between 0 and 9\n");
      return -1;
    }
    self->compr->level = tmpv;
    return 0;
  } else if (strcmp(name, "szlib_mask") == 0) {
    unsigned int compr_mask = H5_SZIP_CHIP_OPTION_MASK | H5_SZIP_ALLOW_K13_OPTION_MASK;
    unsigned int coding_mask = H5_SZIP_EC_OPTION_MASK | H5_SZIP_NN_OPTION_MASK;
    unsigned int allmask = compr_mask | coding_mask;
    unsigned int tmpv;
    if (self->compr->type != CT_SZLIB) {
      setException(PyExc_AttributeError,"szlib_mask is only usable when compression is of type COMPRESSION_SZLIB\n");
      return -1;
    }
    /* Make sure valid values have been set for the mask*/
    tmpv = (unsigned int) PyInt_AsLong(val);
    /* Verify that both masks hasnt been set */
    if (tmpv & H5_SZIP_CHIP_OPTION_MASK && tmpv & H5_SZIP_ALLOW_K13_OPTION_MASK) {
      setException(PyExc_AttributeError,"When setting szlib_mask only one of H5_SZIP_CHIP_OPTION_MASK and H5_SZIP_ALLOW_K13_OPTION_MASK may be used\n");
      return -1;
    }
    if (tmpv & H5_SZIP_EC_OPTION_MASK && tmpv & H5_SZIP_NN_OPTION_MASK) {
      setException(PyExc_AttributeError,"When setting szlib_mask only one of H5_SZIP_EC_OPTION_MASK and H5_SZIP_NN_OPTION_MASK mayb be used\n");
      return -1;
    }
    /* Verify that at least one mask from each has been set */
    if ((tmpv & compr_mask) == 0 || (tmpv & coding_mask) == 0) {
      setException(PyExc_AttributeError,"the mask has to be set as a bitwise ORed value of H5_SZIP_CHIP_OPTION_MASK,H5_SZIP_ALLOW_K13_OPTION_MASK and H5_SZIP_ALLOW_K13_OPTION_MASK,H5_SZIP_EC_OPTION_MASK");
      return -1;
    }
    if (tmpv & (!allmask)) {
      setException(PyExc_AttributeError,"H5_SZIP_CHIP_OPTION_MASK,H5_SZIP_ALLOW_K13_OPTION_MASK,H5_SZIP_EC_OPTION_MASK"
          ",H5_SZIP_NN_OPTION_MASK are the only flags that may be used when creating the bitmask\n");
      return -1;
    }
    if (tmpv == 0) {
      setException(PyExc_AttributeError,"H5_SZIP_CHIP_OPTION_MASK,H5_SZIP_ALLOW_K13_OPTION_MASK,H5_SZIP_EC_OPTION_MASK"
          ",H5_SZIP_NN_OPTION_MASK are the only flags that may be used when creating the bitmask\n");
      return -1;
    }
    return 0;
  } else if (strcmp(name, "szlib_px_per_block") == 0) {
    int tmpv;
    if (self->compr->type != CT_SZLIB) {
      setException(PyExc_AttributeError,"szlib_px_per_block is only usable when compression is of type COMPRESSION_SZLIB\n");
      return -1;
    }
    // Make sure the value is even
    tmpv = PyInt_AsLong(val);
    if ((tmpv % 2) != 0) {
      setException(PyExc_AttributeError,"szlib_px_per_block must be specified in even values, typically 8, 10, 16, and 32");
      return -1;
    }
    self->compr->szlib_px_per_block = tmpv;
    Py_INCREF(Py_None);
    return 0;
  }

  sprintf(errmsg,
          "It is not possible to set '%s' in the compression instance\n", name);
  setException(PyExc_AttributeError,errmsg);
  return -1;
}

statichere PyTypeObject PyhlNodelist_Type =
{
  PyObject_HEAD_INIT(NULL)0, /*ob_size*/
  "PyhlCore", /*tp_name*/
  sizeof(PyhlNodelist), /*tp_size*/
  0, /*tp_itemsize*/
  /* methods */
  (destructor)_dealloc, /*tp_dealloc*/
  0, /*tp_print*/
  (getattrfunc)_getattr, /*tp_getattr*/
  0, /*tp_setattr*/
  0, /*tp_compare*/
  0, /*tp_repr*/
  0, /*tp_as_number */
  0,
  0, /*tp_as_mapping */
  0 /*tp_hash*/
};

statichere PyTypeObject PyhlNode_Type =
{
  PyObject_HEAD_INIT(NULL)0, /*ob_size*/
  "PyhlNodeCore", /*tp_name*/
  sizeof(PyhlNode), /*tp_size*/
  0, /*tp_itemsize*/
  /* methods */
  (destructor)_dealloc_pyhlnode,/*tp_dealloc*/
  0, /*tp_print*/
  (getattrfunc)_getattr_node, /*tp_getattr*/
  0, /*tp_setattr*/
  0, /*tp_compare*/
  0, /*tp_repr*/
  0, /*tp_as_number */
  0,
  0, /*tp_as_mapping */
  0 /*tp_hash*/
};

statichere PyTypeObject PyhlFileCreationProperty_Type =
{
  PyObject_HEAD_INIT(NULL)0, /*ob_size*/
  "PyhlFileCreationProperty", /*tp_name*/
  sizeof(PyhlFileCreationProperty), /*tp_size*/
  0, /*tp_itemsize*/
  /* methods */
  (destructor)_dealloc_pyhlfilecreationproperty,/*tp_dealloc*/
  0, /*tp_print*/
  (getattrfunc)_getattr_filecreationproperty, /*tp_getattr*/
  (setattrfunc)_setattr_filecreationproperty, /*tp_setattr*/
  0, /*tp_compare*/
  0, /*tp_repr*/
  0, /*tp_as_number */
  0,
  0, /*tp_as_mapping */
  0 /*tp_hash*/
};

statichere PyTypeObject PyhlCompression_Type =
{
  PyObject_HEAD_INIT(NULL)0, /*ob_size*/
  "PyhlCompression", /*tp_name*/
  sizeof(PyhlCompression), /*tp_size*/
  0, /*tp_itemsize*/
  /* methods */
  (destructor)_dealloc_pyhlcompression,/*tp_dealloc*/
  0, /*tp_print*/
  (getattrfunc)_getattr_compression, /*tp_getattr*/
  (setattrfunc)_setattr_compression, /*tp_setattr*/
  0, /*tp_compare*/
  0, /*tp_repr*/
  0, /*tp_as_number */
  0,
  0, /*tp_as_mapping */
  0 /*tp_hash*/
};

/**
 * @addtogroup pyhl_api
 * \section _pyhl_interfaces _pyhl interfaces
\verbatim
Function: nodelist()
Returns:
  a new instance of the "nodelist" class.

Function: node(type, nodename, compress=None)
Parameters:
  type     - an integer that can be one of
    ATTRIBUTE_ID, DATASET_ID, GROUP_ID, TYPE_ID or REFERENCE_ID
  nodename - the fully qualified name of this node, eg /a/b/c
  compress - the compression object
Returns:
  a new instance of the "node" class.

Function: filecreationproperty()
Returns:
  a new instance of the "filecreationproperty" class.

Function: compression()
Returns:
  a new instance of the "compression" class.

Function: read_nodelist(filename, frompath=".")
Reads the hdf5 file named filename. If frompath is specified
the node structure is read from that path and downwards in the
hierarchy.
Returns:
  the read nodelist.

Function: is_file_hdf5(filename)
Returns 1 or 0 depending on if the specified filename is a HDF5
file or not.
Returns:
  1 if the specified filename is a HDF5 file, otherwise 0.

Function: show_hdf5errors(enable)
Turns HDF5 error reporting on or off. If enable == 1, then
HDF5 error reporting is turned on. Otherwise it is turned off.
Returns:
  N/A.

Function: show_hlhdferrors(enable)
Turns HL-HDF error reporting on or off. If enable == 1, then
HL-HDF error reporting is turned on (in debugging mode). Otherwise it is turned off.
Returns:
  N/A.
\endverbatim
*/

static PyMethodDef functions[] = {
  {"nodelist", (PyCFunction)_pyhl_new_nodelist, 1},
  {"node", (PyCFunction)_pyhl_new_node, 1},
  {"filecreationproperty",(PyCFunction)_pyhl_new_filecreationproperty,1},
  {"compression",(PyCFunction)_pyhl_new_compression,1},
  {"read_nodelist",(PyCFunction)_pyhl_read_nodelist,1},
  {"is_file_hdf5",(PyCFunction)_pyhl_is_file_hdf5,1},
  {"show_hdf5errors",(PyCFunction)_pyhl_show_hdf5errors,1},
  {"show_hlhdferrors",(PyCFunction)_pyhl_show_hlhdferrors,1},
  {"get_hdf5version", (PyCFunction)_pyhl_get_hdf5version,1},
  {NULL,NULL} /*Sentinel*/
};

/**
 * Initializes PyHL.
 */
void init_pyhl(void)
{
  PyObject *m, *d;
  PyObject *tmp;
  PyhlNodelist_Type.ob_type = &PyType_Type;
  PyhlNode_Type.ob_type = &PyType_Type;

  m = Py_InitModule("_pyhl",functions);
  d = PyModule_GetDict(m);

  ErrorObject = PyString_FromString("_pyhl.error");
  if (ErrorObject == NULL || PyDict_SetItemString(d, "error", ErrorObject) != 0) {
    Py_FatalError("Can't define _pyhl.error");
  }

  tmp = PyInt_FromLong(ATTRIBUTE_ID);
  PyDict_SetItemString(d, "ATTRIBUTE_ID", tmp);
  Py_XDECREF(tmp);

  tmp = PyInt_FromLong(GROUP_ID);
  PyDict_SetItemString(d,"GROUP_ID",tmp);
  Py_XDECREF(tmp);

  tmp = PyInt_FromLong(DATASET_ID);
  PyDict_SetItemString(d,"DATASET_ID",tmp);
  Py_XDECREF(tmp);

  tmp = PyInt_FromLong(TYPE_ID);
  PyDict_SetItemString(d,"TYPE_ID",tmp);
  Py_XDECREF(tmp);

  tmp = PyInt_FromLong(REFERENCE_ID);
  PyDict_SetItemString(d,"REFERENCE_ID",tmp);
  Py_XDECREF(tmp);

  tmp = PyInt_FromLong(CT_ZLIB);
  PyDict_SetItemString(d,"COMPRESSION_ZLIB",tmp);
  Py_XDECREF(tmp);

  tmp = PyInt_FromLong(CT_SZLIB);
  PyDict_SetItemString(d,"COMPRESSION_SZLIB",tmp);
  Py_XDECREF(tmp);

  import_array(); /*To make sure I get access to Numeric*/
  /*Always have to do this*/
  HL_init();
  /* We really should not have debug mode set to 2 */
  HL_setDebugMode(0);
}
