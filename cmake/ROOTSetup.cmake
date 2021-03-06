# Copyright 2018 L. Pickering, P Stowell, R. Terri, C. Wilkinson, C. Wret

################################################################################
#    This file is part of NUISANCE.
#
#    NUISANCE is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    NUISANCE is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with NUISANCE.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

if (NOT DEFINED ENV{ROOTSYS})
  cmessage (FATAL_ERROR "$ROOTSYS is not defined, please set up ROOT first.")
else()
  cmessage(STATUS "Using ROOT installed at $ENV{ROOTSYS}")
  set(CMAKE_ROOTSYS $ENV{ROOTSYS})
endif()

# Get cflags from ROOT
execute_process (COMMAND root-config
  --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS_RAW OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REPLACE " " ";" ROOT_CXX_FLAGS "${ROOT_CXX_FLAGS_RAW}")
# Get libdir from ROOT
execute_process (COMMAND root-config
  --libdir OUTPUT_VARIABLE ROOT_LIBDIR OUTPUT_STRIP_TRAILING_WHITESPACE)
# Get version from ROOT
execute_process (COMMAND root-config
  --version OUTPUT_VARIABLE ROOT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
# Get features from ROOT
execute_process (COMMAND root-config
  --features OUTPUT_VARIABLE ROOT_FEATURES OUTPUT_STRIP_TRAILING_WHITESPACE)

LIST(APPEND EXTRA_LINK_DIRS ${ROOT_LIBDIR})

LIST(APPEND ROOT_LIBS
  Core
  Cint
  RIO
  XMLIO
  Net
  Hist
  Graf
  Graf3d
  Gpad
  Tree
  Rint
  Postscript
  Matrix
  Physics
  MathCore
  Thread
  EG
  Geom
  GenVector)

string(REGEX MATCH "^6.*" ROOTVERSIXMATCH ${ROOT_VERSION})
if(ROOTVERSIXMATCH)
    LIST(REMOVE_ITEM ROOT_LIBS Cint)
  set(USE_ROOT6 True)
else()
    set(USE_ROOT6 False)
endif()

if("${ROOT_FEATURES}" MATCHES "opengl")
  cmessage(STATUS "ROOT built with OpenGL support")
  LIST(APPEND ROOT_LIBS RGL)
endif()

if(DEFINED NEED_ROOTPYTHIA6 AND NEED_ROOTPYTHIA6)
  ###############################  pythia6  ######################################
  if("$ENV{PYTHIA6} " STREQUAL " ")
      cmessage(FATAL_ERROR "Environment variable PYTHIA6 is not defined.")
  else()
    SET(PYTHIA6 $ENV{PYTHIA6})
  endif()
  LIST(APPEND EXTRA_LINK_DIRS ${PYTHIA6})
  LIST(APPEND ROOT_LIBS EGPythia6 Pythia6)
endif()

cmessage ( STATUS "[ROOT]: root-config --version: ${ROOT_VERSION} ")
cmessage ( STATUS "[ROOT]: root-config --cflags : ${ROOT_CXX_FLAGS} ")
cmessage ( STATUS "[ROOT]: root-config --libdir : ${ROOT_LIBDIR} ")
cmessage ( STATUS "[ROOT]: root libs            : ${ROOT_LIBS} ")

LIST(APPEND EXTRA_CXX_FLAGS ${ROOT_CXX_FLAGS})
LIST(APPEND EXTRA_LIBS ${ROOT_LIBS})
