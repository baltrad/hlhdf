############################################################
# Description:	Makefile for generating the three binaries
#		hlenc, hldec and hllist.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
include ../def.mk

CFLAGS = $(OPTS) $(DEFS) $(HDF5_INCDIR) $(ZLIB_INCDIR) $(SZLIB_INCDIR) -I../hlhdf
LDFLAGS=  -L. -L../hlhdf $(HDF5_LIBDIR) $(ZLIB_LIBDIR) $(SZLIB_LIBDIR)

ifeq ($(GOT_SZ_COMPRESS),yes)
  LIB_SZLIB=-lsz
else
  LIB_SZLIB=
endif

LIBRARIES= -lhlhdf $(LD_FORCE_STATIC) -lhdf5 -lz $(LIB_SZLIB) $(LD_FORCE_SHARE) -lm

TARGET_HLDEC=hldec
SOURCES_HLDEC=hldec.c
OBJECTS_HLDEC=$(SOURCES_HLDEC:.c=.o)

TARGET_HLENC=hlenc
SOURCES_HLENC=hlenc.c
OBJECTS_HLENC=$(SOURCES_HLENC:.c=.o)

TARGET_HLLIST=hllist
SOURCES_HLLIST=hllist.c
OBJECTS_HLLIST=$(SOURCES_HLLIST:.c=.o)

all: $(TARGET_HLDEC) $(TARGET_HLENC) $(TARGET_HLLIST)

$(TARGET_HLDEC): $(OBJECTS_HLDEC) ../hlhdf/libhlhdf.a
	$(CC) -o $@ $(LDFLAGS) $(OBJECTS_HLDEC) $(LIBRARIES)

$(TARGET_HLENC): $(OBJECTS_HLENC) ../hlhdf/libhlhdf.a
	$(CC) -o $@ $(LDFLAGS) $(OBJECTS_HLENC) $(LIBRARIES)

$(TARGET_HLLIST): $(OBJECTS_HLLIST) ../hlhdf/libhlhdf.a
	$(CC) -o $@ $(LDFLAGS) $(OBJECTS_HLLIST) $(LIBRARIES)

clean:
	@\rm -f *.o
	@\rm -f *~
	@\rm -f so_locations

distclean:
	@\rm -f *.o
	@\rm -f $(TARGET_HLDEC)
	@\rm -f $(TARGET_HLENC)
	@\rm -f $(TARGET_HLLIST)
	@\rm -f *~ core
	@\rm -f so_locations

distribution:
	@echo "Would bring the latest revision upto date"

check:
	@echo "Nothing to test"

install:
	@$(HL_INSTALL) -f -o -C $(TARGET_HLDEC) $(prefix)/bin/$(TARGET_HLDEC)
	@$(HL_INSTALL) -f -o -C $(TARGET_HLENC) $(prefix)/bin/$(TARGET_HLENC)
	@$(HL_INSTALL) -f -o -C $(TARGET_HLLIST) $(prefix)/bin/$(TARGET_HLLIST)
