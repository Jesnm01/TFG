# Install script for directory: C:/TFG/Github/code/openbabel-openbabel-3-1-1/data

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/data" TYPE FILE FILES
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/atomization-energies.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/atomtyp.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/babel_povray3.inc"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/bondtyp.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/eem.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/eem2015ba.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/eem2015bm.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/eem2015bn.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/eem2015ha.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/eem2015hm.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/eem2015hn.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/eqeqIonizations.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/gaff.dat"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/gaff.prm"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/ghemical.prm"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/logp.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/MACCS.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmff94.ff"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmff94s.ff"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffang.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffbndk.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffbond.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffchg.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffdef.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffdfsb.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffoop.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffpbci.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffprop.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffstbn.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmfftor.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffvdw.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffs_oop.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mmffs_tor.par"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mpC.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/mr.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/patterns.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/phmodel.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/plugindefines.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/psa.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/qeq.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/resdata.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/rigid-fragments.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/rigid-fragments-index.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/ring-fragments.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/ringtyp.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/SMARTS_InteLigand.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/space-groups.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/superatom.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/svgformat.script"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/templates.sdf"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/torlib.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/torsion.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/types.txt"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/data/UFF.prm"
    )
endif()

