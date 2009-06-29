/**
 * Useful macros that are used internally in HLHDF.
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-10
 */
#ifndef HLHDF_DEFINES_PRIVATE_H
#define HLHDF_DEFINES_PRIVATE_H
#include "hdf5.h"

/**
 * @brief Closes a H5 group identifier.
 */
#define HL_H5G_CLOSE(x) if (x>=0) {H5Gclose(x); x=-1;}

/**
 * @brief Closes a H5 data set identifier.
 */
#define HL_H5D_CLOSE(x) if (x>=0) {H5Dclose(x); x=-1;}

/**
 * @brief Closes a group, dataset or named datatype.
 */
#define HL_H5O_CLOSE(x) if (x>=0) {H5Oclose(x); x=-1;}

/**
 * @brief Closes a H5 attribute identifier.
 */
#define HL_H5A_CLOSE(x) if (x>=0) {H5Aclose(x); x=-1;}

/**
 * @brief Closes a H5 file identifier.
 */
#define HL_H5F_CLOSE(x) if (x>=0) {H5Fclose(x); x=-1;}

/**
 * @brief Closes a H5 type identifier
 */
#define HL_H5T_CLOSE(x) if (x>=0) {H5Tclose(x); x=-1;}

/**
 * @brief Closes a H5 space identifier
 */
#define HL_H5S_CLOSE(x) if (x>=0) {H5Sclose(x); x=-1;}

/**
 * @brief Closes a H5 property identifier
 */
#define HL_H5P_CLOSE(x) if (x>=0) {H5Pclose(x); x=-1;}

/**
 * @brief Used to align the offsets when creating a compound type.
 */
#define HLHDF_ALIGN(A,Z)  ((((A)+(Z)-1)/(Z))*(Z))

/**
 * Default number of nodes in a HDF5 file
 */
#define DEFAULT_SIZE_NODELIST 20


#endif
