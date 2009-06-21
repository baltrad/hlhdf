/**
 * Private generic functions used within HLHDF that should not be exposed to the end-user.
 *
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-11
 */
#ifndef HLHDF_PRIVATE_H
#define HLHDF_PRIVATE_H
#include "hlhdf.h"

/**
 * Opens a HDF5 file by specifying the file and an option mode.
 * @param[in] filename the filename
 * @param[in] how how the file should be opened, <b>'r'</b>, <b>'w'</b> or <b>'rw'</b>
 * @return the file identifier or -1 on failure.
 */
hid_t openHlHdfFile(const char* filename,const char* how);

/**
 * Creates a HDF5 file. If the filename already exists this file will be truncated.
 * @param[in] filename the name of the file to create
 * @param[in] property The properties for trimming the filesize and structure. (May be NULL)
 * @return the file identifier or -1 on failure.
 */
hid_t createHlHdfFile(const char* filename, HL_FileCreationProperty* property);

/**
 * Translates a HDF5 type identifier into a native type identifier. This identifier
 * is used within the HLHDF library.
 * @param[in] type the type that should be translated
 * @return a reference to the native type identifier, <0 on failure. Remember to call H5Tclose on the returned type.
 */
hid_t getFixedType(hid_t type);

/**
 * Returns a native data type from the format specifier.
 * @param[in] dataType Format specifier. See @ref ValidFormatSpecifiers "here" for valid format specifiers.
 * @return the reference to the type or <0 on failure.
 */
hid_t translateCharToDatatype(const char* dataType);

/**
 * Returns the type as represented by HDF5. For example 'H5T_STD_I8BE'.
 * @param[in] type the type identifier
 * @return an allocated string with the name or NULL upon failure.
 */
char* getTypeNameString(hid_t type);

/**
 * Returns the format specifier as defined by HLHDF.
 * @param[in] type the type identifier, must be a native type.
 * @return the format specifier string or NULL on failure. See @ref ValidFormatSpecifiers "here" for valid values.
 */
char* getFormatNameString(hid_t type);

/**
 * Returns the String Pad name as represented by HDF5
 * @param[in] type the type identifier
 * @return the String Pad name as defined in HDF5.
 */
char* getStringPadName(hid_t type);

/**
 * Returns the CSET name as represented in HDF5.
 * @param[in] type the type identifier
 * @return the CSET name as represented in HDF5.
 */
char* getStringCsetName(hid_t type);

/**
 * Returns the CTYPE name as represented in HDF5.
 * @param[in] type the type identifier
 * @return the CTYPE name as represented in HDF5.
 */
char* getStringCtypeName(hid_t type);


/**
 * Extracts the parent and child name from the node name.
 * Example:
 * '/group1/group2/group3' will be splitted into '/group1/group2' and 'group3'.
 * @param[in] node the node containing the name that should be splitted
 * @param[out] parent the parent path
 * @param[out] child this nodes name.
 * @return 1 on success, otherwise 0.
 */
int extractParentChildName(HL_Node* node, char* parent, char* child);

#endif /* HLHDF_PRIVATE_H_ */
