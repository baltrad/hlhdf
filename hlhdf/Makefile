############################################################
# Description:	Makefile for building the hlhdf library.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
include ../def.mk

CFLAGS= $(OPTS) $(DEFS) $(INCLUDE_PYTHON) $(HDF5_INCDIR) $(ZLIB_INCDIR) 

TARGET=libhlhdf.so
TARGET.2=libhlhdf.a
SOURCES=hlhdf.c hlhdf_node.c hlhdf_nodelist.c hlhdf_compound.c hlhdf_compound_utils.c hlhdf_read.c hlhdf_write.c hlhdf_debug.c
INSTALL_HEADERS=hlhdf.h hlhdf_types.h hlhdf_node.h hlhdf_nodelist.h hlhdf_compound.h hlhdf_compound_utils.h hlhdf_read.h hlhdf_write.h hlhdf_debug.h

OBJS=$(SOURCES:.c=.o)

all: $(TARGET) $(TARGET.2)

$(TARGET): $(OBJS)
	$(LDSHARED) -o $@ $(OBJS)

$(TARGET.2): $(OBJS)
	$(AR) cr $@ $(OBJS) 

clean:
	@\rm -f *.o
	@\rm -f *~

distclean:
	@\rm -f *.o
	@\rm -f $(TARGET)
	@\rm -f $(TARGET.2)	
	@\rm -f *~ core

distribution:
	@echo "Would bring the latest revision upto date"

install:
	@$(HL_INSTALL) -f -o -C $(TARGET) $(prefix)/lib/$(TARGET)
	@$(HL_INSTALL) -f -o -C $(TARGET.2) $(prefix)/lib/$(TARGET.2)
	@for i in $(INSTALL_HEADERS) ; \
	do \
		$(HL_INSTALL) -f -o -m644 -C $$i $(prefix)/include/$$i; \
	done
