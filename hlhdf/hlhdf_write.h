/**
 * Functions for writing and updating HDF5 files.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-12
 */
#ifndef HLHDF_WRITE_H
#define HLHDF_WRITE_H
#include "hlhdf_types.h"

/**
 * Writes a HDF5 file from a nodelist with the specified file properties and compression level/type.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the node list to write
 * @param[in] property the file creation properties
 * @param[in] compr the wanted compression type and level
 * @return TRUE on success otherwise failure.
 */
int writeHL_NodeList(HL_NodeList* nodelist, HL_FileCreationProperty* property, HL_Compression* compr);

/**
 * Updates a HDF5 file from a nodelist.
 * @ingroup hlhdf_c_apis
 * @param[in] nodelist the node list to update
 * @param[in] compr the wanted compression type and level
 * @return TRUE on success otherwise failure.
 */
int updateHL_NodeList(HL_NodeList* nodelist, HL_Compression* compr);

#endif
