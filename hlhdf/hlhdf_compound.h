/**
 * Functions for working with compound types.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-11
 */
#ifndef HLHDF_COMPOUND_H
#define HLHDF_COMPOUND_H
#include "hlhdf_types.h"

/**
 * Creates a compound type description list.
 * @ingroup hlhdf_c_apis
 * @return the compound type descriptor on success, otherwise NULL.
 */
HL_CompoundTypeDescription* newHL_CompoundTypeDescription(void);

/**
 * Frees the compound type, including all members.
 * @ingroup hlhdf_c_apis
 * @param[in] typelist the descriptor that should be deleted.
 */
void freeHL_CompoundTypeDescription(HL_CompoundTypeDescription* typelist);

/**
 * Creates a compound attribute member node.
 * @ingroup hlhdf_c_apis
 * @param[in] attrname The name of this attribute
 * @param[in] offset the offset in the structure for this member (See HOFFSET in HDF5 documentation)
 * @param[in] format @ref ValidFormatSpecifiers "Format specifier".
 * @param[in] size The size of this data type
 * @param[in] ndims The number of dimensions
 * @param[in] dims The dimensions
 * @return The compound member on success, otherwise NULL
 */
HL_CompoundTypeAttribute* newHL_CompoundTypeAttribute(char* attrname,
                  size_t offset,
                  const char* format,
                  size_t size,
                  int ndims,
                  size_t* dims);

/**
 * Frees one compound type attribute.
 * @ingroup hlhdf_c_apis
 * @param[in] attr the attribute that should be deallocated.
 */
void freeHL_CompoundTypeAttribute(HL_CompoundTypeAttribute* attr);

/**
 * Adds a compound type attribute to the type description.
 * @ingroup hlhdf_c_apis
 * @param[in] typelist the descriptor that should get the new attribute
 * @param[in] attribute the attribute that should be added
 * @return 0 on failure, 1 if success
 */
int addHL_CompoundTypeAttribute(HL_CompoundTypeDescription* typelist,
           HL_CompoundTypeAttribute* attribute);

/**
 * Copies the compound type descriptor.
 * @ingroup hlhdf_c_apis
 * @param[in] descr the descriptor that should be copied.
 * @return The type description on success, otherwise NULL
 */
HL_CompoundTypeDescription* copyHL_CompoundTypeDescription(HL_CompoundTypeDescription* descr);

#endif
