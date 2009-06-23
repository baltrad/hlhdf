/**
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-15

Encodes raw binary data in one file and an ASCII file containing
header information, into an HDF5 file.

<b>hlenc [</b>-hdv<b>] [</b>-z compression<b>]</b> -i inputprefix -o outputfile

<b>[-h]</b> Prints a help text.

<b>[-d]</b> Prints debugging information.

<b>[-v]</b> Prints the version number.

<b>[-z compression]</b> Sets the compression level, can be in the range
<b>0</b> to <b>9</b> where <b>0</b> is no compression and <b>9</b>
is the highest compression.

<b>-i inputprefix</b> Specifies the prefix for the input files, the files that will
be read are &lt;inputprefix&gt;.info and &lt;inputprefix&gt;.data.

<b>-o outputfile</b> Specifies the name of the HDF5 file to be generated. The file
with extension .info should have the following apperance:
DATATYPE: <b>[</b>ATTRIBUTE or DATASET<b>]</b>
FIELDNAME: <b>[</b>name of the field, e.g. '/attr1'<b>]</b>
DATASIZE: <b>[</b>size of the datatype in bytes<b>]</b>
DATAFORMAT: <b>[</b>string representation of the datatype, e.g. int<b>]</b>
DIMS: <b>[</b>the dimension of the data embraced by [], e.g. [10,10]<b>]</b>

The file with extension .data should contain raw binary data with native byte order.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "hlhdf.h"

static char* ProcessName;
static int hlenc_debug=0;

static void PrintHelp(int all);
static void PrintVersion(void);

/**
 * Main function
 */
int main(int argc, char** argv)
{
  extern char* optarg;
  extern int opterr;
  int c, i;
  size_t npts;
  hsize_t dims[4];
  hsize_t tmpDim;
  int ndims = 0;
  long datafield_size;
  char info_filename[512];
  char data_filename[512];
  char tmpDimsString[128];
  char col1[512], col2[512], col3[512];
  char dataType[128];
  int dataSize;
  char fieldName[512];
  char dataFormat[128];
  char dimsString[128];
  int compressLevel = 6; /*Default compression*/
  char oneliner[512];

  char* dimsptr; /*Do not delete*/
  char* datafield = NULL; /*Delete*/
  char* inputprefix = NULL; /*Delete*/
  char* outputfile = NULL; /*Delete*/
  HL_NodeList* nodelist = NULL; /*Delete*/
  HL_Node* node = NULL; /*Do not delete, unless something goes wrong when adding to nodelist*/
  FILE* info_fp = NULL; /*Fclose*/
  FILE* data_fp = NULL; /*fclose*/
  char* fileRet; /*Do not delete*/

  HL_Compression compression;

  strcpy(dataType, "");
  strcpy(fieldName, "");
  strcpy(dataFormat, "");
  strcpy(dimsString, "");
  dataSize = 0;

  initHlHdf();
  debugHlHdf(0);

  ProcessName = strdup(argv[0]);

  opterr = 0;

  while ((c = getopt(argc, argv, "hdvi:o:z:")) != EOF) {
    switch (c) {
    case 'h':
      PrintHelp(1);
      exit(0);
      break;
    case 'd':
      hlenc_debug = 1;
      debugHlHdf(2);
      break;
    case 'v':
      PrintVersion();
      exit(0);
      break;
    case 'i':
      if (optarg)
        inputprefix = strdup(optarg);
      break;
    case 'o':
      if (optarg)
        outputfile = strdup(optarg);
      break;
    case 'z':
      if (optarg)
        compressLevel = atoi(optarg);
      if (compressLevel < 0 || compressLevel > 9) {
        fprintf(stderr, "Compression must be set to between 0 and 9\n");
        goto fail;
      }
    default:
      PrintVersion();
      PrintHelp(0);
      exit(1);
      break;
    }
  }

  if (!inputprefix || !outputfile) {
    PrintHelp(0);
    exit(1);
  }

  if (!(nodelist = newHL_NodeList())) {
    fprintf(stderr, "Failed to allocate nodelist\n");
    goto fail;
  }

  sprintf(info_filename, "%s.info", inputprefix);
  sprintf(data_filename, "%s.data", inputprefix);

  if (!(info_fp = fopen(info_filename, "r"))) {
    fprintf(stderr, "Could not open info file '%s' for reading\n",
            info_filename);
    goto fail;
  }
  if (!(data_fp = fopen(data_filename, "r"))) {
    fprintf(stderr, "Could not open data file '%s' for reading\n",
            data_filename);
    goto fail;
  }

  clearerr(info_fp);
  clearerr(data_fp);

  while (!feof(info_fp)) {
    fileRet = fgets(oneliner, 512, info_fp);
    if (fileRet == NULL) {
      if (!ferror(info_fp))
        break;
      if (hlenc_debug)
        fprintf(stderr, "Failure ocurred when reading info file '%s'\n",
                info_filename);
      goto fail;
    }
    for (i = 0; i < 512; i++) {
      if (oneliner[i] == '#') {
        oneliner[i] = '\0';
      }
      if (oneliner[i] == '\0')
        break;
    }
    if (strlen(oneliner) <= 1)
      continue; /*Ignore empty lines*/

    if (sscanf(oneliner, "%[A-Z_0-9]:%[^\n]", col1, col2) != 2) {
      fprintf(stderr,
              "The line %s did not contains 2 columns, check info file '%s'\n",
              oneliner, info_filename);
    } else {
      if (strcmp(col1, "DATATYPE") == 0) {
        sscanf(col2, "%s", col3);
        if (strcmp(col3, "ATTRIBUTE") != 0 && strcmp(col3, "DATASET") != 0) {
          fprintf(stderr,
                  "DATATYPE is not of ATTRIBUTE or DATASET, it is %s\n", col3);
          goto fail;
        }
        strcpy(dataType, col3);
      } else if (strcmp(col1, "FIELDNAME") == 0) {
        char* tmpptr = strchr(col2, '/');
        if (!tmpptr) {
          fprintf(stderr,
                  "The fieldname has to begin with '/', name is '%s'.\n", col2);
          goto fail;
        }
        strcpy(fieldName, tmpptr);
      } else if (strcmp(col1, "DATASIZE") == 0) {
        dataSize = atoi(col2);
        if (dataSize <= 0) {
          fprintf(stderr, "DATASIZE is less or equal to 0\n");
          goto fail;
        }
      } else if (strcmp(col1, "DATAFORMAT") == 0) {
        sscanf(col2, "%s", col3);
        strcpy(dataFormat, col3);
      } else if (strcmp(col1, "DIMS") == 0) {
        char* tmpptr;
        if (sscanf(col2, "%[^]]", col3) != 1) {
          fprintf(stderr, "Failed to handle dimensions for str '%s'\n", col2);
          goto fail;
        }
        tmpptr = strchr(col3, '[');
        tmpptr++;
        if (!tmpptr) {
          fprintf(stderr, "Failed to handle dimensions for str '%s'\n", col2);
        }
        strcpy(dimsString, tmpptr);
      } else {
        fprintf(stderr, "UNRECKOGNIZED INFO FILE IDENTIFIER %s\n", col1);
        goto fail;
      }
    }
  }

  if (strcmp(dataType, "ATTRIBUTE") == 0) {
    node = newHL_Attribute(fieldName);
  } else {
    node = newHL_Dataset(fieldName);
  }
  if (!node) {
    fprintf(stderr, "Failed to allocate %s node for %s\n", dataType, fieldName);
    goto fail;
  }

  strcpy(tmpDimsString, dimsString);

  dimsptr = strtok(tmpDimsString, ",");
  if (!dimsptr) {
    fprintf(stderr, "Failiure, when parsing dimensions.\n");
    goto fail;
  }
  tmpDim = atoi(dimsptr);
  if (tmpDim < 0) {
    fprintf(stderr, "Could not convert %s to a int\n", dimsptr);
    goto fail;
  }
  dims[ndims++] = tmpDim;
  while ((dimsptr = strtok(NULL, ","))) {
    if ((tmpDim = atoi(dimsptr)) < 0) {
      fprintf(stderr, "Could not convert %s to a int\n", dimsptr);
      goto fail;
    }
    if (ndims > 3) {
      fprintf(
              stderr,
              "Maximum rank of specified field is 4, trying to use dims = '%s'\n",
              dimsString);
      goto fail;
    }
    dims[ndims++] = tmpDim;
  }

  npts = 1;
  if (dims[0] != 0) {
    for (i = 0; i < ndims; i++) {
      npts *= dims[i];
    }
  }

  fseek(data_fp, 0, SEEK_END);
  datafield_size = ftell(data_fp);
  if (datafield_size != dataSize * npts) {
    fprintf(stderr,
            "Data files size is %d, but according to the info file, it should be %d\n",
            (int) datafield_size, (int) (dataSize * npts));
    goto fail;
  }
  if (!(datafield = malloc(datafield_size))) {
    fprintf(stderr, "Failed to allocate enough memory for databuffer\n");
    goto fail;
  }

  rewind(data_fp);

  if (fread(datafield, 1, datafield_size, data_fp) != datafield_size) {
    fprintf(stderr, "Failed to read data\n");
    goto fail;
  }

  if (dims[0] == 0) {
    /*Add, should write a scalar value*/
    if (!setHL_NodeScalarValue(node,dataSize,(unsigned char*)datafield,dataFormat,-1)) {
      fprintf(stderr, "Failed to set scalar value\n");
      goto fail;
    }
  } else {
    /*Ahh, probably a simple value*/
    if (!setHL_NodeArrayValue(node,dataSize,ndims,dims,(unsigned char*)datafield,dataFormat,-1)) {
      fprintf(stderr, "Failed to set array value\n");
      goto fail;
    }
  }

  if (!addHL_Node(nodelist, node)) {
    fprintf(stderr, "Failed to add node (%s) to nodelist\n", node->name);
    goto fail;
  }
  node = NULL; /*nodelist has got the responsibility now*/

  if (!setHL_NodeListFileName(nodelist,outputfile)) {
    fprintf(stderr, "Failed to set the output filename %s\n", outputfile);
    goto fail;
  }

  initHL_Compression(&compression, CT_ZLIB);
  compression.level = compressLevel;

  if (!writeHL_NodeList(nodelist, NULL, &compression)) {
    fprintf(stderr, "Failed to write nodelist\n");
    goto fail;
  }

  /*Keep memory clean*/
  if (datafield)
    free(datafield);
  if (inputprefix)
    free(inputprefix);
  if (outputfile)
    free(outputfile);
  freeHL_NodeList(nodelist);
  freeHL_Node(node);
  if (info_fp)
    fclose(info_fp);
  if (data_fp)
    fclose(data_fp);

  exit(0);
  return 0;

fail:
  /*Keep memory clean*/
  if (datafield)
    free(datafield);
  if (inputprefix)
    free(inputprefix);
  if (outputfile)
    free(outputfile);
  freeHL_NodeList(nodelist);
  freeHL_Node(node);
  if (info_fp)
    fclose(info_fp);
  if (data_fp)
    fclose(data_fp);

  exit(1);
  return 1;
}

static void PrintHelp(int all)
{
   fprintf(stderr,"Usage: %s [-hdv] [-z compress level] -i <inputprefix> -o <outputfile>\n",
	   ProcessName);
   if(!all)
      return;
   fprintf(stderr,"\t[-h]    Prints this help.\n");
   fprintf(stderr,"\t[-d]    Turns on debugging.\n");
   fprintf(stderr,"\t[-v]    Prints the version.\n");
   fprintf(stderr,"\t[-z lvl]\n");
   fprintf(stderr,"\t        Sets the compression level to lvl, should be between 0-9\n");
   fprintf(stderr,"\t-i inputprefix\n");
   fprintf(stderr,"\t        Specify the prefix for the input files, the files\n");
   fprintf(stderr,"\t        that will be read are <inputprefix>.info and\n");
   fprintf(stderr,"\t        <inputprefix>.data\n");
   fprintf(stderr,"\t-o outputfile\n");
   fprintf(stderr,"\t        Specify the name of the HDF file to be generated.\n");
}

static void PrintVersion()
{
   fprintf(stderr,"HLENC: Version 0.1\n");
}
