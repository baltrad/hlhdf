############################################################
# Description:	Makefile for the hlhdf distribution.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
include ./def.mk

SHELL=/bin/sh

TEST_YES=yes

ifeq ($(COMPILE_FOR_PYTHON),yes)
  SOURCE_DIRS=hlhdf pyhlhdf pyhl bin test doc doxygen examples
else
  SOURCE_DIRS=hlhdf bin test doc doxygen examples
endif

DOCS_DIRS=doxygen

all:
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "-------Compiling directory $$i------------"; \
		TDIR=`pwd` ; \
		cd $$i; \
		$(MAKE); \
		cd $$TDIR; \
	done;

docs:
	@for i in $(DOCS_DIRS) ; \
	do \
		TDIR=`pwd` ; \
		cd $$i; \
		$(MAKE) docs; \
		cd $$TDIR; \
	done;

clean:
	@\rm -f *~ core
	@for i in $(SOURCE_DIRS) ; \
	do \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) clean; \
		cd $$TDIR; \
	done;

distclean:
	@\rm -f *~ core config.*
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "------Distcleaning directory $$i-------"; \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) distclean; \
		cd $$TDIR; \
	done
	@\rm -f def.mk
	
distribution:
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "----Creating distribution for directory $$i-----"; \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) distribution; \
		cd $$TDIR; \
	done

check:
	@chmod +x ./tools/test_hlhdf.sh
	@./tools/test_hlhdf.sh

install:
	@for i in $(SOURCE_DIRS) ; \
	do \
		echo "------Installing from directory $$i------"; \
		TDIR=`pwd`; \
		cd $$i; \
		$(MAKE) install; \
		cd $$TDIR; \
	done
	@$(HL_INSTALL) -f -o -C -m644 def.mk $(prefix)/mkf/hldef.mk
	@$(HL_INSTALL) -f -o -C ./tools/install-sh.sh $(prefix)/bin/hlinstall.sh

