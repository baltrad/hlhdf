/**
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-15
 *
 * Utility functions for working with python objects.
 */
#ifndef PYHLHDF_COMMON_H
#define PYHLHDF_COMMON_H

#include <Python.h>
#include <hlhdf.h>
#include "hlhdf_arrayobject_wrap.h"

/**
 * Retrives a float from a dictionary, python equivalent would be val = dict[name]
 * @param[in] name	The name of the attribute
 * @param[out] val	The assigned float
 * @param[in] dict	The Python dictionary
 * @return 1 on success, otherwise 0
 */
int getFloatFromDictionary(char* name, float* val, PyObject* dict);

/**
 * Retrives a double from a dictionary, python equivalent would be val = dict[name]
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned double
 * @param[in] dict  The Python dictionary
 * @return 1 on success, otherwise 0
 */
int getDoubleFromDictionary(char* name, double* val, PyObject* dict);

/**
 * Retrives a integer from a dictionary, python equivalent would be val = dict[name]
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned integer
 * @param[in] dict  The Python dictionary
 * @return 1 on success, otherwise 0
 */
int getIntegerFromDictionary(char* name, int* val, PyObject* dict);

/**
 * Retrives a unsigned long from a dictionary, python equivalent would be val = dict[name]
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned unsigned long
 * @param[in] dict  The Python dictionary
 * @return 1 on success, otherwise 0
 */
int getUnsignedLongFromDictionary(char* name, unsigned long* val, PyObject* dict);

/**
 * Retrives a long from a dictionary, python equivalent would be val = dict[name]
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned long
 * @param[in] dict  The Python dictionary
 * @return 1 on success, otherwise 0
 */
int getLongFromDictionary(char* name, long* val, PyObject* dict);

/**
 * Retrives a float from an object, python equivalent would be val = dict.name
 * @param[in] name	The name of the attribute
 * @param[out] val	The assigned float
 * @param[in] dict	The Python object
 * @return 1 on success, otherwise 0
 */
int getFloatFromObject(char* name, float* val, PyObject* dict);

/**
 * Retrives a double from an object, python equivalent would be val = dict.name
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned double
 * @param[in] dict  The Python object
 * @return 1 on success, otherwise 0
 */
int getDoubleFromObject(char* name, double* val, PyObject* dict);

/**
 * Retrives a integer from an object, python equivalent would be val = dict.name
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned integer
 * @param[in] dict  The Python object
 * @return 1 on success, otherwise 0
 */
int getIntegerFromObject(char* name, int* val, PyObject* dict);

/**
 * Retrives a long from an object, python equivalent would be val = dict.name
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned long
 * @param[in] dict  The Python object
 * @return 1 on success, otherwise 0
 */
int getLongFromObject(char* name, long* val, PyObject* dict);

/**
 * Retrives a unsigned long from an object, python equivalent would be val = dict.name
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned unsigned long
 * @param[in] dict  The Python object
 * @return 1 on success, otherwise 0
 */
int getUnsignedLongFromObject(char* name, unsigned long* val, PyObject* dict);

/**
 * Retrives a short from an object, python equivalent would be val = dict.name
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned short
 * @param[in] dict  The Python object
 * @return 1 on success, otherwise 0
 */
int getShortFromObject(char* name, short* val, PyObject* dict);

/**
 * Retrives a unsigned char from an object, python equivalent would be val = dict.name
 * @param[in] name  The name of the attribute
 * @param[out] val The assigned unsigned char
 * @param[in] dict  The Python object
 * @return 1 on success, otherwise 0
 */
int getByteFromObject(char* name, unsigned char* val, PyObject* dict);

/**
 * Retrives a string from a name in an object, python equivalent would be	val = dict.name
 * @param[in] name The name of the attribute
 * @param[in] dict The Python dictionary
 * @return the string on success, otherwise NULL
 */
char* getStringFromObject(char* name, PyObject* dict);

/**
 * Retrives a integer from a index in a tuple, python equivalent would be	val = tuple[idx]
 * @param[in] idx	The index of the value
 * @param[out] val	The assigned integer
 * @param[in] tuple  The python tuple
 * @return 1 on success, otherwise 0
 */
int getIdxIntegerFromTuple(int idx, int* val, PyObject* tuple);

/**
 * Retrives a double from a index in a tuple, python equivalent would be val = tuple[idx]
 * @param[in] idx The index of the value
 * @param[out] val  The assigned double
 * @param[in] tuple  The python tuple
 * @return 1 on success, otherwise 0
 */
int getIdxDoubleFromTuple(int idx, double* val, PyObject* tuple);

/**
 * Retrives a string from a dictionary, python equivalent would be val = dict[name].
 * @param[in] name  The name of the attribute
 * @param[in] dict  The Python dictionary
 * @return the string on success, otherwise NULL
 */
char* getStringFromDictionary(char* name, PyObject* dict);

/**
 * Sets an integer in a dictionary, the python equivalent would be:	info[keyname]=in_value
 * @param[in] info	The Python dictionary
 * @param[in] keyname	The name of the attribute
 * @param[in] in_value the integer value
 * @return 1 on success, otherwise 0
 */
int setMappingInteger(PyObject* info, char* keyname, int in_value);

/**
 * Sets an double in a dictionary, the python equivalent would be: info[keyname]=in_value
 * @param[in] info  The Python dictionary
 * @param[in] keyname The name of the attribute
 * @param[in] in_value the double value
 * @return 1 on success, otherwise 0
 */
int setMappingDouble(PyObject* info, char* keyname, double in_value);

/**
 * Sets an float in a dictionary, the python equivalent would be: info[keyname]=in_value
 * @param[in] info  The Python dictionary
 * @param[in] keyname The name of the attribute
 * @param[in] in_value the float value
 * @return 1 on success, otherwise 0
 */
int setMappingFloat(PyObject* info, char* keyname, float in_value);

/**
 * Sets an string in a dictionary, the python equivalent would be: info[keyname]=in_value
 * @param[in] info  The Python dictionary
 * @param[in] keyname The name of the attribute
 * @param[in] in_string the string value
 * @return 1 on success, otherwise 0
 */
int setMappingString(PyObject* info,char* keyname,char* in_string);

/**
 * Sets a string in a dictionary, the python equivalent would be:	info[keyname]=in_string[0:len]
 * @param[in] info	The Python dictionary
 * @param[in] keyname	The name of the attribute
 * @param[in] in_string the string
 * @param[in] len	The number of characters that should be taken from the string
 * @return 1 on success, otherwise 0
 */
int setMappingString_Length(PyObject* info,char* keyname,char* in_string,int len);

/**
 * Sets an integer in a object, the python equivalent would be:	info.keyname=in_value
 * @param[in] info	The Python object
 * @param[in] keyname	The name of the attribute
 * @param[in] in_value the integer value
 * @return 1 on success, otherwise 0
 */
int setObjectInteger(PyObject* info, char* keyname, int in_value);

/**
 * Sets an long in a object, the python equivalent would be: info.keyname=in_value
 * @param[in] info  The Python object
 * @param[in] keyname The name of the attribute
 * @param[in] in_value the long value
 * @return 1 on success, otherwise 0
 */
int setObjectLong(PyObject* info, char* keyname, long in_value);

/**
 * Sets an unsigned long in a object, the python equivalent would be: info.keyname=in_value
 * @param[in] info  The Python object
 * @param[in] keyname The name of the attribute
 * @param[in] in_value the unsigned long value
 * @return 1 on success, otherwise 0
 */
int setObjectUnsignedLong(PyObject* info, char* keyname, unsigned long in_value);

/**
 * Sets an double in a object, the python equivalent would be: info.keyname=in_value
 * @param[in] info  The Python object
 * @param[in] keyname The name of the attribute
 * @param[in] in_value the double value
 * @return 1 on success, otherwise 0
 */
int setObjectDouble(PyObject* info, char* keyname, double in_value);

/**
 * Sets an float in a object, the python equivalent would be: info.keyname=in_value
 * @param[in] info  The Python object
 * @param[in] keyname The name of the attribute
 * @param[in] in_value the float value
 * @return 1 on success, otherwise 0
 */
int setObjectFloat(PyObject* info, char* keyname, float in_value);

/**
 * Sets an string in a object, the python equivalent would be: info.keyname=in_value
 * @param[in] info  The Python object
 * @param[in] keyname The name of the attribute
 * @param[in] in_string the string value
 * @return 1 on success, otherwise 0
 */
int setObjectString(PyObject* info,char* keyname,char* in_string);

/**
 * Sets a string in an object, the python equivalent would be:	info.keyname=in_string[0:len]
 * @param[in] info	The Python object
 * @param[in] keyname	The name of the attribute
 * @param[in] in_string the string
 * @param[in] len	The number of characters that should be taken from the string
 * @return 1 on success, otherwise 0
 */
int setObjectString_Length(PyObject* info,char* keyname,char* in_string,int len);

/**
 * Translates a PyArray type to a hlhdf known	format.
 * @param[in] type	The PyArray type
 * @return The hlhdf representation if ok, NULL otherwise.
 */
char* translatePyFormatToHlHdf(char type);

/**
 * Translates an hlhdf represented type into a PyArray known format
 * @param[in] format	The hlhdf representation
 * @return The PyArray type if ok, -1 otherwise
 */
int pyarraytypeFromHdfType(const char* format);

/**
 * Creates a new 1-dimensional PyArrayObject
 * @param[in] nl	The dimension
 * @param[in] format	The hlhdf representation of the array to be	created.
 * @return The PyArray if ok, NULL otherwise
 */
PyArrayObject* new1d_ArrayObject(int nl, const char* format);

/**
 * Creates a new 2-dimensional PyArrayObject
 * @param[in] xsize	The x-dimension
 * @param[in] ysize	The y-dimension
 * @param[in] format The hlhdf representation of the array to be created.
 * @return The PyArray if ok, NULL otherwise
 */
PyArrayObject* new2d_ArrayObject(int xsize,int ysize, const char* format);

#endif
