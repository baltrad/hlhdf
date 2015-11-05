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

CFLAGS= $(OPTS) $(CCSHARED) $(DEFS) -I../../hlhdf -I../../pyhlhdf $(NUMPY_INCLUDE_DIR) \
	$(INCLUDE_PYTHON) $(HDF5_INCDIR) $(ZLIB_INCDIR) $(SZLIB_INCDIR)

LDFLAGS= -L../../hlhdf -L../../pyhlhdf $(HDF5_LIBDIR) $(ZLIB_LIBDIR) $(SZLIB_LIBDIR)

LIBRARIES= -lpyhlhdf $(LD_FORCE_SHARE) -lhlhdf -lhdf5 $(LIB_SZLIB) -lz -lm -lc

TARGET.1=_varioustestsmodule.so
TARGET.2=_rave_info_typemodule.so

SOURCES.1=varioustests.c
OBJECTS.1=$(SOURCES.1:.c=.o)

SOURCES.2=rave_info_type.c
OBJECTS.2=$(SOURCES.2:.c=.o)

all: $(TARGET.1) $(TARGET.2)

$(TARGET.1): $(OBJECTS.1) ../../hlhdf/libhlhdf.so ../../pyhlhdf/libpyhlhdf.a
	$(LDSHARED) -o $@ $(OBJECTS.1) $(LDFLAGS) $(LIBRARIES)

$(TARGET.2): $(OBJECTS.2) ../../hlhdf/libhlhdf.so ../../pyhlhdf/libpyhlhdf.a
	$(LDSHARED) -o $@ $(OBJECTS.2) $(LDFLAGS) $(LIBRARIES)

clean:
	@\rm -f *.o
	@\rm -f *~
	@\rm -f so_locations
	@\rm -f core
	@\rm -f *.pyc
	@\rm -fr TEST-unittest*.xml

distclean:
	@\rm -f *.o
	@\rm -f $(TARGET.1)
	@\rm -f $(TARGET.2)
	@\rm -f *~ core
	@\rm -f so_locations
	@\rm -f *.pyc
	@\rm -fr TEST-unittest*.xml

distribution:
	@echo "Would bring the latest revision upto date"

install:
	@echo "Nothing to install"

