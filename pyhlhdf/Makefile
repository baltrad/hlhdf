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

.PHONY: clean
clean:
	@\rm -f *.o
	@\rm -f *~
	@\rm -f core

.PHONY: distclean
distclean:
	@\rm -f *.o
	@\rm -f $(TARGET)
	@\rm -f *~ core

.PHONY: distribution
distribution:
	@echo "Would bring the latest revision upto date"

.PHONY: install
install:
	@"$(HL_INSTALL)" -f -o -C $(TARGET) "$(prefix)/lib/$(TARGET)"
	@for i in $(INSTALL_HEADERS) ; \
	do \
		"$(HL_INSTALL)" -f -o -m644 -C $$i "$(prefix)/include/$$i"; \
	done	
