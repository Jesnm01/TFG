# Install script for directory: C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obabel.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obchiral.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obconformer.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obdistgen.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obenergy.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obfit.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obgen.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obgrep.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obgui.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obminimize.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obprobe.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obprop.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obrms.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obrotamer.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obrotate.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obspectrophore.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obsym.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obtautomer.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/obthermo.1"
    "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/roundtrip.1"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/openbabel/3.1.0" TYPE FILE FILES "C:/TFG/Github/code/openbabel-openbabel-3-1-1/doc/splash.png")
endif()

