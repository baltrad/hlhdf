/**
 * Functions for working with HL_Node's and HL_NodeList's.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-11
 */
#ifndef HLHDF_NODE_H
#define HLHDF_NODE_H
#include "hlhdf_types.h"

/**
 * Creates a new HL_NodeList instance
 * @ingroup hlhdf_c_apis
 * @return the allocated node list on success, otherwise NULL.
 */
HL_NodeList* newHL_NodeList(void);

/**
 * Releasing all resources associated with this node list including the node list itself.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the list that should be released.
 */
void freeHL_NodeList(HL_NodeList* nodelist);

/**
 * Sets the filename in the HL_NodeList instance
 * @param[in] nodelist - the nodelist
 * @param[in] filename - the filename that should be used
 * @return 1 on success, otherwise 0
 */
int setHL_NodeListFileName(HL_NodeList* nodelist, const char* filename);

/**
 * Returns the filename of this nodelist.
 * @param[in] nodelist - the nodelist
 * @return the filename for this nodelist or NULL if no filename is set or failed to allocate memory.
 */
char* getHL_NodeListFileName(HL_NodeList* nodelist);

/**
 * Returns the number of nodes that exists in the provided nodelist.
 * @param[in] nodelist - the node list
 * @return the number of nodes or a negative value on failure.
 */
int getHL_NodeListNumberOfNodes(HL_NodeList* nodelist);

/**
 * Returns the node at the specified index.
 * @param[in] nodelist - the node list
 * @param[in] index - the index of the node
 * @return the node if it exists, otherwise NULL. <b>Do not free since it is an internal pointer</b>
 */
HL_Node* getHL_NodeListNodeByIndex(HL_NodeList* nodelist, int index);

/**
 * Marks all nodes in the nodelist with the provided mark.
 * @param[in] nodelist - the nodelist to be updated.
 * @param[in] mark - the mark each node should have.
 */
void markHL_NodeListNodes(HL_NodeList* nodelist, const HL_NodeMark mark);

/**
 * Creates a new HL_Node instance.
 * @ingroup hlhdf_c_apis
 * @param[in] name the name of this node, should be fully qualified name. I.e. /a/b/c
 * @return the allocated node on success, otherwise NULL.
 */
HL_Node* newHL_Node(const char* name);

/**
 * Deallocates the provided node.
 * @ingroup hlhdf_c_apis
 * @param[in] node the node that should be deallocated
 */
void freeHL_Node(HL_Node* node);

/**
 * Creates an empty group HL_Node of type @ref HL_Type::GROUP_ID.
 * @ingroup hlhdf_c_apis
 * @param[in] name the fully qualified name
 * @return The allocated node on success, otherwise NULL.
 */
HL_Node* newHL_Group(const char* name);

/**
 * Creates an empty attribute HL_Node of type @ref HL_Type::ATTRIBUTE_ID.
 * @ingroup hlhdf_c_apis
 * @param[in] name the fully qualified name
 * @return The allocated node on success, otherwise NULL.
 */
HL_Node* newHL_Attribute(const char* name);

/**
 * Creates an empty data set HL_Node of type @ref HL_Type::DATASET_ID.
 * @ingroup hlhdf_c_apis
 * @param[in] name the fully qualified name
 * @return The allocated node on success, otherwise NULL.
 */
HL_Node* newHL_Dataset(const char* name);

/**
 * Creates an empty data type HL_Node of type @ref HL_Type::TYPE_ID.
 * @ingroup hlhdf_c_apis
 * @param[in] name the fully qualified name
 * @return The allocated node on success, otherwise NULL.
 */
HL_Node* newHL_Datatype(const char* name);

/**
 * Creates an empty reference HL_Node of type @ref HL_Type::REFERENCE_ID.
 * @ingroup hlhdf_c_apis
 * @param[in] name the fully qualified name
 * @return The allocated node on success, otherwise NULL.
 */
HL_Node* newHL_Reference(const char* name);

/**
 * Copies an HL_Node.
 * @ingroup hlhdf_c_apis
 * @param[in] node the node that should be copied
 * @return the allocated node on success, otherwise NULL.
 */
HL_Node* copyHL_Node(HL_Node* node);

/**
 * Adds a node to the nodelist.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the nodelist that should get a node added
 * @param[in] node the node that should be added to the node list
 * @return 1 On success, otherwise 0
 */
int addHL_Node(HL_NodeList* nodelist, HL_Node* node);

/**
 * Locates a node called nodeName in the nodelist and returns a pointer
 * to this node. I.e. Do not delete it!
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the nodelist that should be searched in
 * @param[in] nodeName the name of the node that should be located
 * @return the node if it could be found, otherwise NULL.
 */
HL_Node* getHL_Node(HL_NodeList* nodelist,const char* nodeName);

/**
 * Sets a scalar value in the specified node.
 * @ingroup hlhdf_c_apis
 * @param[in] node the node that should get its value set.
 * @param[in] sz the size of the data
 * @param[in] value the data
 * @param[in] fmt the format specifier, @see ref ValidFormatSpecifiers "here" for valid formats.
 * @param[in] typid if a custom made type should be used for writing the data, otherwise use -1.
 * @return 1 if everything was ok, otherwise 0
 */
int setHL_NodeScalarValue(HL_Node* node,size_t sz,unsigned char* value,const char* fmt,hid_t typid);

/**
 * Sets an array value in the specified node.
 * @ingroup hlhdf_c_apis
 * @param[in] node the node that should get its value set
 * @param[in] sz the size of the type
 * @param[in] ndims the rank
 * @param[in] dims the dimension
 * @param[in] value the data buffer
 * @param[in] fmt the format specifier, @see ref ValidFormatSpecifiers "here" for valid formats.
 * @param[in] typid if a custom made type should be used for writing the data, otherwise use -1.
 * @return 1 if everything was ok, otherwise 0
 */
int setHL_NodeArrayValue(HL_Node* node,size_t sz,int ndims,hsize_t* dims,unsigned char* value,
      const char* fmt,hid_t typid);

/**
 * Marks a node.
 * @param[in] node - the node to be marked
 * @param[in] mark - the node mark
 * @return 1 on success, otherwise 0.
 */
int setHL_NodeMark(HL_Node* node, const HL_NodeMark mark);

/**
 * Searches the nodelist for any type node, that has got the same object id as objno0 and objno1.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the nodelist that should be searched
 * @param[in] objno0 identifier 0
 * @param[in] objno1 identifier 1
 * @return The compound type description if found, otherwise NULL.
 */
HL_CompoundTypeDescription* findHL_CompoundTypeDescription(HL_NodeList* nodelist,
                  unsigned long objno0,
                  unsigned long objno1);

/**
 * Will mark the node to be committed, only applicable on type nodes.
 * @ingroup hlhdf_c_apis
 * @param[in] node the node that should be commited
 * @param[in] typeid the HDF5 type identifier
 * @return 1 on success, otherwise 0
 * @deprecated this function will be removed sooner or later.
 */
int commitHL_Datatype(HL_Node* node,hid_t typeid);

#endif
