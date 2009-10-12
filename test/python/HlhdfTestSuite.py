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

##
# Suite containing all testfiles for the python package.
#
import unittest
from HlhdfReadTest import *
from HlhdfWriteTest import *
from HlhdfUpdateTest import *
from HlhdfNodeTest import *
from HlhdfPyhlhdfCommonTest import *
from HlhdfFileCreationPropertyTest import *

if __name__ == '__main__':
  unittest.main()
