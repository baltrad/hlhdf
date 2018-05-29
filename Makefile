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
# Description:	Makefile for the hlhdf distribution.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
-include ./def.mk

SHELL=/bin/sh

TEST_YES=yes

HLHDF_PREPARE_INSTALLDIR=no

ifeq ($(COMPILE_FOR_PYTHON),yes)
  SOURCE_DIRS=hlhdf pyhlhdf pyhl bin test doc doxygen examples scripts
else
  SOURCE_DIRS=hlhdf bin test doc doxygen examples scripts
endif

DOCS_DIRS=doxygen

all:
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "-------Compiling directory $$i------------"; \
		TDIR=`pwd` ; \
		cd $$i; \
		$(MAKE) || exit 255; \
		cd "$$TDIR"; \
	done;

.PHONY: doc
doc:
	@for i in $(DOCS_DIRS) ; \
	do \
		TDIR=`pwd` ; \
		cd $$i; \
		$(MAKE) doc || exit 255; \
		cd "$$TDIR"; \
	done;
	
.PHONY: clean
clean:
	@\rm -f *~ core
	@for i in $(SOURCE_DIRS) ; \
	do \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) clean || exit 255; \
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
		$(MAKE) distclean || exit 255; \
		cd "$$TDIR"; \
	done
	@\rm -f def.mk

.PHONY: distribution
distribution:
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "----Creating distribution for directory $$i-----"; \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) distribution || exit 255; \
		cd "$$TDIR"; \
	done

.PHONY: test
test:
	@chmod +x ./scripts/test_hlhdf.sh
	@./scripts/test_hlhdf.sh

.PHONY: install
install:
	@chmod +x ./scripts/prepare_installdir.sh
	@HLHDF_PREPARE_INSTALLDIR=$(HLHDF_PREPARE_INSTALLDIR) ./scripts/prepare_installdir.sh "$(DESTDIR)$(prefix)"
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "------Installing from directory $$i------"; \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) install || exit 255; \
		cd "$$TDIR"; \
	done
	@"$(HL_INSTALL)" -f -C -m644 def.mk "$(DESTDIR)$(prefix)/mkf/hldef.mk"
	@"$(HL_INSTALL)" -f -C ./scripts/install-sh.sh "$(DESTDIR)$(prefix)/bin/hlinstall.sh"

.PHONY: rollbackinstall
rollbackinstall:
	@chmod +x ./scripts/rollback_installation.sh
	@./scripts/rollback_installation.sh "$(DESTDIR)$(prefix)"
  
