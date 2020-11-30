###########################################################################
# Copyright (C) 2009 Swedish Meteorological and Hydrological Institute, SMHI,
#
# This file is part of HLHDF.
#
# HLHDF is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# HLHDF is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with HLHDF.  If not, see <http://www.gnu.org/licenses/>.
###########################################################################

############################################################
# Description:	Makefile for building the hlhdf library.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
-include ../def.mk

CFLAGS= $(OPTS) $(CCSHARED) $(DEFS) $(INCLUDE_PYTHON) $(HDF5_INCDIR) $(ZLIB_INCDIR) -DDEBUG_HLHDF

TARGET=libhlhdf.so
TARGET.2=libhlhdf.a
SOURCES=hlhdf.c hlhdf_node.c hlhdf_nodelist.c hlhdf_compound.c hlhdf_compound_utils.c hlhdf_read.c hlhdf_write.c hlhdf_debug.c hlhdf_alloc.c
INSTALL_HEADERS=hlhdf.h hlhdf_types.h hlhdf_node.h hlhdf_nodelist.h hlhdf_compound.h hlhdf_compound_utils.h hlhdf_read.h hlhdf_write.h hlhdf_debug.h hlhdf_alloc.h

OBJS=$(SOURCES:.c=.o)

all: $(TARGET) $(TARGET.2)

$(TARGET): $(OBJS)
	$(LDSHARED) -o $@ $(OBJS) $(HDF5_LIBDIR) -lhdf5

$(TARGET.2): $(OBJS)
	$(AR) cr $@ $(OBJS) 

.PHONY: clean
clean:
	@\rm -f *.o
	@\rm -f *~

.PHONY: distclean
distclean:
	@\rm -f *.o
	@\rm -f $(TARGET)
	@\rm -f $(TARGET.2)	
	@\rm -f *~ core

.PHONY: distribution
distribution:
	@echo "Would bring the latest revision upto date"

.PHONY: install
install:
	@"$(HL_INSTALL)" -f -C $(TARGET) "$(DESTDIR)$(prefix)/lib/$(TARGET)"
	@"$(HL_INSTALL)" -f -C $(TARGET.2) "$(DESTDIR)$(prefix)/lib/$(TARGET.2)"
	@echo "Installing header files"
	@for i in $(INSTALL_HEADERS) ; \
	do \
		"$(HL_INSTALL)" -f -m644 -C $$i "$(DESTDIR)$(prefix)/include/$$i"; \
	done

