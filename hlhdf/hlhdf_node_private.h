/**
 * Private functions for working with HL_Node's.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-24
 */
#ifndef HLHDF_NODE_PRIVATE_H
#define HLHDF_NODE_PRIVATE_H

/**
 * Sets data and datasize in the node. When this function has been called,
 * responsibility for the data has been taken over so do not release that memory.
 * @param[in] node the node (MAY NOT BE NULL)
 * @param[in] datasize the size of the data type as get by H5Tget_size.
 * @param[in] data the data (<b>responsibility taken over so do not release after call</b>).
 */
void HLNodePrivate_setData(HL_Node* node, size_t datasize, unsigned char* data);

/**
 * Sets rawdata and rawdatasize in the node. When this function has been called,
 * responsibility for the data has been taken over so do not release that memory.
 * @param[in] node the node (MAY NOT BE NULL)
 * @param[in] datasize the size of the data type as get by H5Tget_size.
 * @param[in] data the data (<b>responsibility taken over so do not release after call</b>).
 */
void HLNodePrivate_setRawdata(HL_Node* node, size_t datasize, unsigned char* data);

/**
 * Copies the typeid and sets it in the node and also atempts to derive
 * the format name.
 * @param[in] node the node
 * @param[in] typeId the type identifier
 * @return 1 on success, otherwise 0.
 */
int HLNodePrivate_setTypeIdAndDeriveFormat(HL_Node* node, hid_t typeId);

/**
 * Returns the internal name pointer in the HL_Node. <b>DO NOT FREE</b>.
 * @param[in] node the node
 * @return the internal pointer to the name.
 */
char* HLNodePrivate_getName(HL_Node* node);

/**
 * Sets the HDF identifier.
 * @param[in] node the node
 * @param[in] hdfid the hdf id. (<b>The responsibility is taken over so do not delete</b>)
 */
void HLNodePrivate_setHdfID(HL_Node* node, hid_t hdfid);

/**
 * Returns the HDF identifier.
 * @param[in] node the node
 * @return the hdf identifier (<b>Do not close</b>).
 */
hid_t HLNodePrivate_getHdfID(HL_Node* node);

/**
 * Returns an internal pointer to the dimensions.
 * @param[in] node the node
 * @return the internal dimension pointer (<b>Do not free and be careful when using it</b>).
 */
const hsize_t* HLNodePrivate_getDims(HL_Node* node);

/**
 * Returns the internal type id.
 * @param[in] node the  node
 * @return the internal type identifier (<b>Do not close</b>).
 */
hid_t HLNodePrivate_getTypeId(HL_Node* node);

#endif /* HLHDF_NODE_PRIVATE_H */
