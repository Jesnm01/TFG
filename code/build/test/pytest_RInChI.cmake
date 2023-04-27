
  SET(ENV{PYTHONPATH} C:/TFG/Github/code/openbabel-openbabel-3-1-1/scripts/python:C:/TFG/Github/code/build/lib)
  SET(ENV{LD_LIBRARY_PATH} C:/TFG/Github/code/openbabel-openbabel-3-1-1/scripts/python:C:/TFG/Github/code/build/lib:$ENV{LD_LIBRARY_PATH})
  SET(ENV{BABEL_LIBDIR} C:/TFG/Github/code/build/lib/)
  SET(ENV{BABEL_DATADIR} C:/TFG/Github/code/openbabel-openbabel-3-1-1/data)
  MESSAGE("C:/TFG/Github/code/openbabel-openbabel-3-1-1/scripts/python:C:/TFG/Github/code/build/lib")
  EXECUTE_PROCESS(
  	COMMAND C:/Users/Jesús/AppData/Local/Programs/Python/Python39/python.exe C:/TFG/Github/code/openbabel-openbabel-3-1-1/test/testRInChI.py 
  	#WORKING_DIRECTORY @LIBRARY_OUTPUT_PATH@
  	RESULT_VARIABLE import_res
  	OUTPUT_VARIABLE import_output
  	ERROR_VARIABLE  import_output
  )
  
  # Pass the output back to ctest
  IF(import_output)
    MESSAGE(${import_output})
  ENDIF(import_output)
  IF(import_res)
    MESSAGE(SEND_ERROR ${import_res})
  ENDIF(import_res)
