#include <hdf5.h>
#include <string.h>
#include <alloca.h>

/*#define PAD_WITH_NULLPAD 1*/

static int addAttribute(hid_t gid, const char* name, const char* value, int sz, H5T_str_t paddingType)
{
  hid_t aid = -1;
  hid_t attr_id = -1;
  herr_t status = -1;
  hid_t type_id = H5Tcopy (H5T_C_S1);

  if (H5Tset_size (type_id, sz) < 0) {
    fprintf(stderr, "Could not set size of type\n");
    goto fail;
  }

  H5Tset_strpad(type_id, paddingType);

  if ((aid = H5Screate(H5S_SCALAR)) < 0) {
    fprintf(stderr, "Failed to create scalar data space\n");
    goto fail;
  }
  if ((attr_id = H5Acreate(gid, name, type_id, aid, H5P_DEFAULT, H5P_DEFAULT)) < 0) {
    fprintf(stderr, "Failed to create scalar attribute\n");
    goto fail;
  }

  if (sz == -1) {
    if (H5Awrite(attr_id, type_id, &value) < 0) {
      fprintf(stderr, "Failed to write scalar data to file\n");
      goto fail;
    }
  } else {
    if (H5Awrite(attr_id, type_id, value) < 0) {
      fprintf(stderr, "Failed to write scalar data to file\n");
      goto fail;
    }
  }
  status = 0;
fail:
  if (aid >= 0) H5Sclose(aid);
  if (attr_id >= 0) H5Aclose(attr_id);
  if (type_id >= 0) H5Tclose(type_id);
  return status;
}

int main(int argc, char** argv) {
  hid_t file_id = -1;
  hid_t gid = -1;
  char* nullpaddedStr = alloca(sizeof(char)*27);
  const char* nullterminatedStr = "this is a null terminated string";
  const char* spacepaddedStr = "this is a spacepadded string";
  const char* variableLengthStr = "this is a variable length string";

  memset(nullpaddedStr, 0, sizeof(char)*27);
  memcpy(nullpaddedStr, "this is a nullpadded string", strlen("this is a nullpadded string"));

  file_id = H5Fcreate("strings_fixture.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  if (file_id < 0) {
    fprintf(stderr, "Could not create file\n");
    goto done;
  }
  if ((gid = H5Gopen(file_id, ".", H5P_DEFAULT)) < 0) {
    fprintf(stderr, "Failed to open root group\n");
    goto done;
  }

  addAttribute(gid, "nullterminated", nullterminatedStr, strlen(nullterminatedStr)+1, H5T_STR_NULLTERM);
  addAttribute(gid, "nullpadded", nullpaddedStr, 27, H5T_STR_NULLPAD);
  addAttribute(gid, "spacepadded", spacepaddedStr, 28, H5T_STR_SPACEPAD);
  addAttribute(gid, "erroneously_nullterminated", nullterminatedStr, strlen(nullterminatedStr), H5T_STR_NULLTERM);
  addAttribute(gid, "variable", variableLengthStr, -1, H5T_STR_NULLTERM);
  H5Fflush(file_id, H5F_SCOPE_LOCAL);
done:
  if (file_id >= 0) H5Fclose(file_id);
  if (gid >= 0) H5Gclose(gid);
  return 0;
}
