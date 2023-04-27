# Install script for directory: C:/TFG/Github/code/openbabel-openbabel-3-1-1/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/openbabel")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openbabel3/openbabel" TYPE FILE FILES "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/chemdrawcdx.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openbabel3/openbabel" TYPE FILE FILES
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/alias.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/atom.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/base.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/bitvec.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/bond.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/bondtyper.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/builder.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/canon.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/chains.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/chargemodel.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/conformersearch.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/data.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/data_utilities.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/descriptor.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/distgeom.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/dlhandler.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/elements.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/fingerprint.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/forcefield.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/format.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/generic.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/graphsym.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/grid.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/griddata.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/groupcontrib.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/inchiformat.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/internalcoord.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/isomorphism.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/json.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/kekulize.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/kinetics.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/lineend.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/locale.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/matrix.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/mcdlutil.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/mol.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/molchrg.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/obconversion.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/oberror.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/obfunctions.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/obiter.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/obmolecformat.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/obutil.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/op.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/optransform.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/parsmart.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/patty.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/phmodel.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/plugin.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/pointgroup.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/query.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/reaction.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/reactionfacade.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/residue.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/ring.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/rotamer.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/rotor.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/shared_ptr.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/spectrophore.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/tautomer.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/text.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/tokenst.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/typer.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/xml.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openbabel3/openbabel/math" TYPE FILE FILES
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/math/align.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/math/erf.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/math/matrix3x3.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/math/spacegroup.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/math/transform3d.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/math/vector3.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openbabel3/openbabel/stereo" TYPE FILE FILES
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/stereo/bindings.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/stereo/cistrans.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/stereo/squareplanar.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/stereo/stereo.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/stereo/tetrahedral.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/stereo/tetranonplanar.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/stereo/tetraplanar.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openbabel3/openbabel/depict" TYPE FILE FILES
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/depict/asciipainter.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/depict/cairopainter.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/depict/commandpainter.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/depict/depict.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/depict/painter.h"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/include/openbabel/depict/svgpainter.h"
    )
endif()

