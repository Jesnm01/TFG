#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "openbabel" for configuration "Release"
set_property(TARGET openbabel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(openbabel PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/bin/openbabel-3.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/openbabel-3.dll"
  )

list(APPEND _cmake_import_check_targets openbabel )
list(APPEND _cmake_import_check_files_for_openbabel "${_IMPORT_PREFIX}/bin/openbabel-3.lib" "${_IMPORT_PREFIX}/bin/openbabel-3.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
