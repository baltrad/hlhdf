############################################################
# Description:	Makefile for building the pyhl module.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
include ../def.mk

CFLAGS= $(OPTS) $(DEFS) -I../hlhdf -I../pyhlhdf $(NUMPY_INCLUDE_DIR) \
	$(INCLUDE_PYTHON) \
	$(HDF5_INCDIR) $(ZLIB_INCDIR) $(SZLIB_INCDIR)

ifeq ($(GOT_SZ_COMPRESS),yes)
  LIB_SZLIB=-lsz
else
  LIB_SZLIB=
endif


LDFLAGS= -L../hlhdf -L../pyhlhdf $(HDF5_LIBDIR) $(ZLIB_LIBDIR) $(SZLIB_LIBDIR)

LIBRARIES= -lpyhlhdf $(LD_FORCE_SHARE) -lhlhdf -lhdf5 $(LIB_SZLIB) -lz -lm -lc

TARGET=_pyhlmodule.so

SOURCES=pyhl.c
OBJECTS=$(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) ../hlhdf/libhlhdf.so ../pyhlhdf/libpyhlhdf.a
	$(LDSHARED) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBRARIES)

clean:
	@\rm -f *.o
	@\rm -f *~
	@\rm -f so_locations
	@\rm -f core

distclean:
	@\rm -f *.o
	@\rm -f $(TARGET)
	@\rm -f *~ core
	@\rm -f so_locations

distribution:
	@echo "Would bring the latest revision upto date"

install:
	@"$(HL_INSTALL)" -f -o -C $(TARGET) "$(prefix)/lib/$(TARGET)"
	@echo "$(prefix)/lib" > hlhdf.tmp
	@"$(HL_INSTALL)" -f -o -C -m644 hlhdf.tmp "$(prefix)/hlhdf.pth"
	@\rm -f hlhdf.tmp

