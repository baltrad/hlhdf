############################################################
# Description: Makefile for building the python tests.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2009
#
# History:	2009-06-15 Created by Anders Henja
############################################################
include ../../def.mk

ifeq ($(GOT_SZ_COMPRESS),yes)
  LIB_SZLIB=-lsz
else
  LIB_SZLIB=
endif

CFLAGS= $(OPTS) $(DEFS) -I../../hlhdf -I../../pyhlhdf $(NUMPY_INCLUDE_DIR) \
	$(INCLUDE_PYTHON) $(HDF5_INCDIR) $(ZLIB_INCDIR) $(SZLIB_INCDIR)

LDFLAGS= -L../../hlhdf -L../../pyhlhdf $(HDF5_LIBDIR) $(ZLIB_LIBDIR) $(SZLIB_LIBDIR)

LIBRARIES= -lpyhlhdf -lhlhdf $(LD_FORCE_SHARE) -lhdf5 $(LIB_SZLIB) -lz -lm -lc

TARGET.1=_varioustestsmodule.so
TARGET.2=_rave_info_typemodule.so

SOURCES.1=varioustests.c
OBJECTS.1=$(SOURCES.1:.c=.o)

SOURCES.2=rave_info_type.c
OBJECTS.2=$(SOURCES.2:.c=.o)

all: $(TARGET.1) $(TARGET.2)

$(TARGET.1): $(OBJECTS.1) ../../hlhdf/libhlhdf.a ../../pyhlhdf/libpyhlhdf.a
	$(LDSHARED) -o $@ $(OBJECTS.1) $(LDFLAGS) $(LIBRARIES)

$(TARGET.2): $(OBJECTS.2) ../../hlhdf/libhlhdf.a ../../pyhlhdf/libpyhlhdf.a
	$(LDSHARED) -o $@ $(OBJECTS.2) $(LDFLAGS) $(LIBRARIES)

clean:
	@\rm -f *.o
	@\rm -f *~
	@\rm -f so_locations
	@\rm -f core
	@\rm -f *.pyc

distclean:
	@\rm -f *.o
	@\rm -f $(TARGET.1)
	@\rm -f $(TARGET.2)
	@\rm -f *~ core
	@\rm -f so_locations
	@\rm -f *.pyc

distribution:
	@echo "Would bring the latest revision upto date"

install:
	@echo "Nothing to install"

