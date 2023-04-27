#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "openbabel" for configuration "Debug"
set_property(TARGET openbabel APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(openbabel PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/bin/openbabel-3.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/openbabel-3.dll"
  )

list(APPEND _cmake_import_check_targets openbabel )
list(APPEND _cmake_import_check_files_for_openbabel "${_IMPORT_PREFIX}/bin/openbabel-3.lib" "${_IMPORT_PREFIX}/bin/openbabel-3.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
