############################################################
# Description:	Makefile for building the pyhlhdf library.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
include ../def.mk

CFLAGS = $(OPTS) $(CCSHARED) $(DEFS) $(NUMPY_INCLUDE_DIR) $(INCLUDE_PYTHON) $(HDF5_INCDIR) $(ZLIB_INCDIR) -I../hlhdf

# I gotta learn how to use expanded variables sometime.
#TEMP= $(CFLAGS)
#CFLAGS := $(CFLAGS) -I../hlhdf

TARGET=libpyhlhdf.a
SOURCES=pyhlhdf_common.c
INSTALL_HEADERS=pyhlhdf_common.h hlhdf_arrayobject_wrap.h

OBJS=$(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cr $(TARGET) $(OBJS)

clean:
	@\rm -f *.o
	@\rm -f *~
	@\rm -f core

distclean:
	@\rm -f *.o
	@\rm -f $(TARGET)
	@\rm -f *~ core

distribution:
	@echo "Would bring the latest revision upto date"

install:
	@"$(HL_INSTALL)" -f -o -C $(TARGET) "$(prefix)/lib/$(TARGET)"
	@for i in $(INSTALL_HEADERS) ; \
	do \
		"$(HL_INSTALL)" -f -o -m644 -C $$i "$(prefix)/include/$$i"; \
	done	
#@echo "Would install this package"
