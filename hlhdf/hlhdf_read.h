/**
 * Functions that should be used when reading a HDF5 file.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-11
 */
#ifndef HLHDF_READ_H
#define HLHDF_READ_H
#include "hlhdf_types.h"

/**
 * Reads an HDF5 file with name filename from the group fromPath and downwards.
 * This function will not fetch the actual data but will only read the structure.
 * Use selectAll/selectAllNodes and fetchMarkedNodes to retrieve the data.
 * @ingroup hlhdf_c_apis
 * @param[in] filename the name of the HDF5 file
 * @param[in] fromPath the path from where the file should be read.
 * @return the read data structure on success, otherwise NULL.
 */
HL_NodeList* readHL_NodeListFrom(const char* filename, const char* fromPath);

/**
 * Reads an HDF5 file with name filename from the root group ("/") and downwards.
 * This function will not fetch the actual data but will only read the structure.
 * Use selectAll/selectAllNodes and fetchMarkedNodes to retrieve the data.
 * @ingroup hlhdf_c_apis
 * @param[in] filename the name of the HDF5 file
 * @return the read data structure on success, otherwise NULL.
 */
HL_NodeList* readHL_NodeList(const char* filename);

/**
 * Selects the node named 'name' from which to fetch data.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the nodelist where the node named name should be marked for select
 * @param[in] name the fully qualified name of the node that should be selected.
 * @return: 1 on success, otherwise 0
 */
int selectNode(HL_NodeList* nodelist, const char* name);

/**
 * Marks all nodes in the nodelist for retrival.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the node list
 * @return 1 on success, otherwise 0
 */
int selectAllNodes(HL_NodeList* nodelist);

/**
 * Selects all metadata nodes in the nodelist to be fetched, ie. dataset attributes but no dataset arrays.
 * <b>VOLATILE: Do not attempt to access dataset arrays after calling this.</b>
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the node list
 * @return 1 on success, otherwise 0
 */
int selectMetadataNodes(HL_NodeList* nodelist);

/**
 * De-selects the node named 'name' to be retrived when fetching data from the nodelist file.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the node list
 * @param[in] name the name that should be deselected
 * @return 1 on success, otherwise 0.
 */
int deselectNode(HL_NodeList* nodelist, const char* name);

/**
 * Fills all nodes (marked as select) with data.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the node list
 * @return 1 on success, otherwise 0
 */
int fetchMarkedNodes(HL_NodeList* nodelist);

/**
 * Behaves as a combination of selectNode()/fetch()/getNode().
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the node list
 * @param[in] name the name of the node that should be fetched.
 * @return the found node or NULL on failure.
 */
HL_Node* fetchNode(HL_NodeList* nodelist, const char* name);

#endif
