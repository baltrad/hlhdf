/**
 * Type definitions that are used in HLHDF.
 *
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-10
 */
#ifndef HLHDF_TYPES_H
#define HLHDF_TYPES_H

#include <hdf5.h>

/**
 * Defines what type of compression that should be used
 * @ingroup hlhdf_c_apis
 *  */
typedef enum HL_CompressionType {
  CT_NONE=0, /**< No compression */
  CT_ZLIB,   /**< ZLIB compression */
  CT_SZLIB   /**< SZLIB compression */
} HL_CompressionType;

/**
 * See hdf5 documentation for H5Pget_version for purpose
 * @ingroup hlhdf_c_apis
 */
typedef struct {
   unsigned super; /**< Super block version number */
   unsigned freelist; /**< Global freelist version number */
   unsigned stab; /**< Symbol table version number */
   unsigned shhdr; /**< Shared object header version number */
} HL_PropertyVersion;

/**
 * See hdf5 documentation for H5Pset_sizes and H5Pget_sizes for purpose
 * @ingroup hlhdf_c_apis
 */
typedef struct {
   size_t sizeof_addr; /**< Size of an object offset in bytes */
   size_t sizeof_size; /**< Size of an object length in bytes */
} HL_PropertySize;

/**
 * See hdf5 documentation for H5Pset_sym_k and H5Pget_sym_k for purpose
 * @ingroup hlhdf_c_apis
 */
typedef struct {
   unsigned ik; /**< Symbol table tree rank. */
   unsigned lk; /**< Symbol table node size. */
} HL_PropertySymK;

/**
 * Properties that can be finely tuned when creating a HDF5 file.
 * @ingroup hlhdf_c_apis
 */
typedef struct {
  /**
   * See @ref #HL_PropertyVersion
   */
  HL_PropertyVersion version;

  /**
   * See hdf5 documentation for H5Pset_userblock and H5Pget_userblock for purpose
   */
  hsize_t userblock;

  /**
   * See @ref #HL_PropertySize
   */
  HL_PropertySize sizes;

  /**
   * See @ref #HL_PropertySymK
   */
  HL_PropertySymK sym_k;

  /**
   * See hdf5 documentation for H5Pset_istore_k and H5Pget_istore_k for purpose
   */
  unsigned istore_k;

  /** This is actually in the File access properties but atm it feels like overkill
   * to provide the user with functionality to fine tune all these variables
   * since its only the meta_block_size we have found any use for.
   * If the value of meta_block_size is 2048, then we are using default value
   * and the default FILE_ACCESS_PROPERTY will be used.
   */
  hsize_t meta_block_size;

} HL_FileCreationProperty;

/**
 * Compression properties.
 * @ingroup hlhdf_c_apis
 */
typedef struct {
   /**
    * The wanted compression type.
    * If @ref HL_CompressionType#CT_ZLIB is specified, then level needs to be set.
    * if @ref HL_CompressionType#CT_SZLIB is specified, then szlib_mask and szlib_px_per_block needs to be set.
    */
   HL_CompressionType type;

   /**
    * The compression level when using ZLIB compression,
    * compression is indicated by values between 1-9, if set
    * to 0 this will not be seen as compression */
   int level;

   /**
    * Mask when using szlib compression, the mask can be set up from two
    * different sets of options.
    * <table>
    * <tr><td>H5_SZIP_CHIP_OPTION_MASK</td><td>Compresses exactly as in hardware.</td></tr>
    * <tr><td>H5_SZIP_ALLOW_K13_OPTION_MASK</td><td>Allows k split = 13 compression mode. (Default)</td></tr>
    * <tr><td>&nbsp;</td><td>&nbsp;</td></tr>
    * <tr><td>H5_SZIP_EC_OPTION_MASK</td><td>Selects entropy coding method. (Default)</td></tr>
    * <tr><td>H5_SZIP_NN_OPTION_MASK</td><td>Selects nearest neighbor coding method.</td></tr>
    * </table>
    * Where the paired options are mutual exclusive, i.e. it is possible
    * to set the szlib_mask to H5_SZIP_CHIP_OPTION_MASK|H5_SZIP_EC_OPTION_MASK
    * but not to H5_SZIP_CHIP_OPTION_MASK|H5_SZIP_ALLOW_K13_OPTION_MASK
    */
   unsigned int szlib_mask;

   /**
    * The block size must be be even, with typical values being 8, 10, 16, and 32.
    * The more pixel values vary, the smaller this number should be.
    */
   unsigned int szlib_px_per_block;
} HL_Compression;

/**
 * This is an enumeration variable designed to identify the type of a given node.
 * @ingroup hlhdf_c_apis
 */
typedef enum HL_Type {
  UNDEFINED_ID=-1, /**< An undefined type */
  ATTRIBUTE_ID=0,  /**< Attribute type (corresponds to H5A) */
  GROUP_ID,        /**< Group type (corresponds to H5G) */
  DATASET_ID,      /**< Data set type (corresponds to H5D) */
  TYPE_ID,         /**< Type type (corresponds to H5T) */
  REFERENCE_ID     /**< Reference type (corresponds to H5R) */
} HL_Type;

/**
 * This is an enumeration variable designed to identify the type of data in a given node.
 * When new nodes are initiated, they are initialized with DTYPE_UNDEFINED_ID.
 * @ingroup hlhdf_c_apis
 */
typedef enum HL_DataType {
  DTYPE_UNDEFINED_ID=-1, /**< Undefined data type */
  HL_SIMPLE=0,           /**< If the value is a scalar */
  HL_ARRAY               /**< If the value is of array type */
} HL_DataType;

/**
 * This is an enumeration variable designed to keep track of the status of a given node.
 * @ingroup hlhdf_c_apis
 */
typedef enum HL_NodeMark {
  NMARK_ORIGINAL=0, /**< Nothing has been done on the node, e.g. it has been read but nothing else */
  NMARK_CREATED,    /**< If a node has been created but not been written */
  NMARK_CHANGED,    /**< If a nodes value has been changed and needs to be written */
  NMARK_SELECT      /**< The node has been marked for fetching but the read has not been performed yet. */
} HL_NodeMark;

/**
 * This type is designed to describe an individual node with a complicated structure, ie.
 * one which consists of more than atomic data types. It contains all the information required
 * to interpret the contents of the node.
 * @ingroup hlhdf_c_apis
 */
typedef struct {
   char attrname[256]; /**< name of the attribute */
   size_t offset;      /**< offset in the structure, use HOFFSET in HDF5 */
   size_t size;        /**< size of the data field */
   char format[256];   /**< format specifier, @ref ValidFormatSpecifiers "format specifier"*/
   int ndims;          /**< number of dimensions */
   size_t dims[4];     /**< dimensions, max 4 */
} HL_CompoundTypeAttribute;

/**
 * This type is a list of <b>HL_CompoundTypeAttribute</b>s. The reason why it's
 * called "Description" is that it acts more like meta data than actual
 * data.
 * @ingroup hlhdf_c_apis
 */
typedef struct {
   char hltypename[256];    /**< this types name if any */
   unsigned long objno[2];  /**< the unique identifier for this type */
   size_t size;             /**< the size of this type */
   int nAttrs;              /**< the number of attributes defining this type */
   int nAllocAttrs;         /**< the number of allocated attributes */
   HL_CompoundTypeAttribute** attrs; /**< points at the different attributes that defines this type, max index is always nAttrs-1 */
} HL_CompoundTypeDescription;

/**
 * Each entry and type in a HDF5 file is represented by a HL_Node.
 * @ingroup hlhdf_c_apis
 */
typedef struct HL_Node {
   HL_Type type;               /**< The type of this node */
   char name[256];             /**< The name of this node */
   int ndims;                  /**< Number of dimensions if this node is represented by a HL_Type#ATTRIBUTE_ID or HL_Type#TYPE_ID*/
   hsize_t dims[4];            /**< The dimension size */
   unsigned char* data;        /**< The data in fixed-type format */
   unsigned char* rawdata;     /**< Unconverted data, exactly as read from the file */
   char format[64];            /**< @ref ValidFormatSpecifiers "Format specifier" */
   hid_t typeId;               /**< HDF5 type identifier */
   size_t dSize;               /**< Size for data (fixed type) */
   size_t rdSize;              /**< Size for rawdata */
   HL_DataType dataType;       /**< Type of data */
   hid_t hdfId;                /**< The hdf id that this node represents (used internally)*/
   HL_NodeMark mark;           /**< Current state of this node */
   HL_CompoundTypeDescription* compoundDescription; /**< The compound type description if this is a TYPE node*/
   HL_Compression* compression; /**< Compression settings for this node */
} HL_Node;

/**
 * Represents a HDF5 file.
 * @ingroup hlhdf_c_apis
 */
typedef struct _HL_NodeList HL_NodeList;

#endif
