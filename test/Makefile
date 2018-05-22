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
# Description:	Makefile for the tests.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2009
#
# History:	2009-06-10 Created by Anders Henja
############################################################
-include ../def.mk

SHELL=/bin/sh

TEST_YES=yes

ifeq ($(COMPILE_FOR_PYTHON),yes)
  SOURCE_DIRS=python
else
  SOURCE_DIRS=
endif

all:
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "-------Compiling directory $$i------------"; \
		TDIR=`pwd` ; \
		cd $$i; \
		$(MAKE); \
		cd "$$TDIR"; \
	done;

.PHONY: clean
clean:
	@\rm -f *~ core
	@for i in $(SOURCE_DIRS) ; \
	do \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) clean; \
		cd "$$TDIR"; \
	done;

.PHONY: distclean
distclean:
	@\rm -f *~ core config.*
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "------Distcleaning directory $$i-------"; \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) distclean; \
		cd "$$TDIR"; \
	done

.PHONY: distribution
distribution:
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "----Creating distribution for directory $$i-----"; \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) distribution; \
		cd "$$TDIR"; \
	done

.PHONY: install
install:
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "------Installing from directory $$i------"; \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) install; \
		cd "$$TDIR"; \
	done

