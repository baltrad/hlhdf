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
# Description:	Makefile for building the pyhl module.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
-include ../def.mk

CFLAGS= $(OPTS) $(CCSHARED) $(DEFS) -I../hlhdf -I../pyhlhdf $(NUMPY_INCLUDE_DIR) \
	$(INCLUDE_PYTHON) \
	$(HDF5_INCDIR) $(ZLIB_INCDIR) $(SZLIB_INCDIR)

ifeq ($(GOT_SZ_COMPRESS),yes)
  LIB_SZLIB=-lsz
else
  LIB_SZLIB=
endif


LDFLAGS= -L../hlhdf -L../pyhlhdf $(HDF5_LIBDIR) $(ZLIB_LIBDIR) $(SZLIB_LIBDIR)

LIBRARIES= -lpyhlhdf $(LD_FORCE_SHARE) -lhlhdf -lhdf5 $(LIB_SZLIB) -lz -lm -lc

TARGET=_pyhl.so

SOURCES=pyhl.c
OBJECTS=$(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) ../hlhdf/libhlhdf.so ../pyhlhdf/libpyhlhdf.a
	$(LDSHARED) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBRARIES)

.PHONY: clean
clean:
	@\rm -f *.o
	@\rm -f *~
	@\rm -f so_locations
	@\rm -f core

.PHONY: distclean
distclean:
	@\rm -f *.o
	@\rm -f $(TARGET)
	@\rm -f *~ core
	@\rm -f so_locations

.PHONY: distribution
distribution:
	@echo "Would bring the latest revision upto date"

.PHONY: install
install:
	@"$(HL_INSTALL)" -f -C $(TARGET) "$(DESTDIR)$(prefix)/lib/$(TARGET)"
	@echo "$(prefix)/lib" > hlhdf.tmp
	@"$(HL_INSTALL)" -f -C -m644 hlhdf.tmp "$(DESTDIR)$(prefix)/hlhdf.pth"
	@\rm -f hlhdf.tmp

