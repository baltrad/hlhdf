/**
 * Functions for working with HL_Node's.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-11
 */
#ifndef HLHDF_NODE_H
#define HLHDF_NODE_H
#include "hlhdf_types.h"

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
 * Returns the node name.
 * @param[in] node the node
 * @return the node name or NULL on failure
 */
char* getHL_NodeName(HL_Node* node);

/**
 * Returns the internal data pointer for this node.
 * @param[in] node the node
 * @return the internal data (<b>Do not release and be careful so that the node does not change when holding the data pointer.</b>).
 */
const unsigned char* getHL_NodeData(HL_Node* node);

/**
 * Returns the type size for the data format.
 * @param[in] node the node
 * @return the type size for the data format
 */
size_t getHL_NodeDataSize(HL_Node* node);

/**
 * Returns the internal rawdata pointer for this node.
 * @param[in] node the node
 * @return the internal data (<b>Do not release and be careful so that the node does not change when holding the data pointer.</b>).
 */
const unsigned char* getHL_NodeRawdata(HL_Node* node);

/**
 * Returns the type size for the raw data format.
 * @param[in] node the node
 * @return the type size for the raw data format
 */
size_t getHL_NodeRawdataSize(HL_Node* node);

/**
 * Compares the nodes name with the provided name.
 * @param[in] node the node
 * @param[in] name the name to test
 * @return 1 if the name equals, otherwise 0
 */
int equalsHL_NodeName(HL_Node* node, const char* name);

/**
 * Marks a node.
 * @param[in] node - the node
 * @param[in] mark - the node mark
 */
void setHL_NodeMark(HL_Node* node, const HL_NodeMark mark);

/**
 * Returns the mark of a node.
 * @param[in] node - the node
 * @return the node mark.
 */
HL_NodeMark getHL_NodeMark(HL_Node* node);

/**
 * Sets the type on a node.
 * @param[in] node the node that should get its type set
 * @param[in] type the type the node should have
 */
void setHL_NodeType(HL_Node* node, const HL_Type type);

/**
 * Gets the type of the node
 * @param[in] node the node
 * @return the type of the node
 */
HL_Type getHL_NodeType(HL_Node* node);

/**
 * Gets the string represenation of the data format.
 * @param[in] node the node
 * @return the format specifier or the string representation for HLHDF_UNDEFINED
 */
const char* HLNode_getFormatName(HL_Node* node);

/**
 * Returns the format specifier for the node
 * @param[in] node the node
 * @return the format specifier
 */
HL_FormatSpecifier HLNode_getFormat(HL_Node* node);

/**
 * Sets the data type of the node
 * @param[in] node the node
 * @param[in] datatype the datatype
 * @return the data type of the node
 */
void setHL_NodeDataType(HL_Node* node, HL_DataType datatype);

/**
 * Returns the data type of the node
 * @param[in] node the node
 * @return the data type of the node
 */
HL_DataType getHL_NodeDataType(HL_Node* node);

/**
 * Sets the node dimensions.
 * @param[in] node the node
 * @param[in] ndims the rank
 * @param[in] dims the dimensions
 * @return 1 on success, otherwise 0
 */
int setHL_NodeDimensions(HL_Node* node, int ndims, hsize_t* dims);

/**
 * Gets the node dimensions
 * @param[in] node the node
 * @param[out] ndims the rank
 * @param[out] dims the dimensions
 */
void getHL_NodeDimensions(HL_Node* node, int* ndims, hsize_t** dims);

/**
 * Returns the rank (number of dimensions).
 * @param[in] node the node
 * @return the rank.
 */
int getHL_NodeRank(HL_Node* node);

/**
 * Returns the dimension of the specified index.
 * @param[in] node the node
 * @param[in] index the index
 * @return the size of the specified index. (If index < 0 or >= the rank then 0 is returned).
 */
hsize_t getHL_NodeDimension(HL_Node* node, int index);

/**
 * Returns the number of value points that this node has.
 * If rank == 0, then 1 is returned.
 * If rank > 0 and dims is set, then 1*dims[0]*...*dims[rank-1] is returned.
 * If rank > 0 and dims == NULL, which not should happen, but then 0 is returned
 * @param[in] node the node
 * @return the number of value points.
 */
hsize_t getHL_NodeNumberOfPoints(HL_Node* node);

/**
 * Sets the description for this node.
 * @param[in] node the node
 * @param[in] descr the compound description <b>(memory taken over so do not release it)</b>
 */
void setHL_NodeCompoundDescription(HL_Node* node, HL_CompoundTypeDescription* descr);

/**
 * Returns the compound description for the node (if any).
 * @param[in] node the node
 * @return the compound description if any, otherwise NULL. (<b>Do not release since it points to internal memory</b>
 */
HL_CompoundTypeDescription* getHL_NodeCompoundDescription(HL_Node* node);

/**
 * Returns the compression object for this node.
 * @param[in] node the node
 * @return the compression object (<b>points to internal memory so do not release</b>).
 */
HL_Compression* getHL_NodeCompression(HL_Node* node);

/**
 * Sets the compression object for this node.
 * @param[in] node the node
 * @param[in] compression the compression object (<b>HL_Node takes responsibility so do not free after call</b>).
 */
void setHL_NodeCompression(HL_Node* node, HL_Compression* compression);

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
