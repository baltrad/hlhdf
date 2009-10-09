#!/bin/sh

cd hlhdf
make
cd ../pyhl
make clean
make
make -f Makefile.rave_info_type clean
make -f Makefile.rave_info_type
cd ..

#

