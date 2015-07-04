

SET(CAFFE_INCLUDE_SEARCH_PATHS
  /usr/include
  /usr/include/caffe
  /usr/local/include
  /usr/local/include/caffe
  /opt/Caffe/include
  $ENV{HOME}/caffe/include
  $ENV{CAFFE_HOME}
  $ENV{CAFFE_HOME}/include
)

SET(CAFFE_LIB_SEARCH_PATHS
        /lib/
        /lib/caffe
        /lib64/
        /usr/lib
        /usr/lib/caffe
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        $ENV{CAFFE_HOME}
        $ENV{CAFFE_HOME}/lib
        $ENV{CAFFE_HOME}/build/lib
        $ENV{HOME}/caffe
        $ENV{HOME}/caffe/lib
        $ENV{HOME}/caffe/build/lib
 )

FIND_PATH(CAFFE_INCLUDE_DIR NAMES caffe/caffe.hpp PATHS ${CAFFE_INCLUDE_SEARCH_PATHS})
FIND_LIBRARY(CAFFE_LIB NAMES caffe PATHS ${CAFFE_LIB_SEARCH_PATHS})

SET(CAFFE_FOUND ON)

#    Check include files
IF(NOT CAFFE_INCLUDE_DIR)
    SET(CAFFE_FOUND OFF)
    MESSAGE(STATUS "Could not find CAFFE include. Turning CAFFE_FOUND off")
ENDIF()

#    Check libraries
IF(NOT CAFFE_LIB)
    SET(CAFFE_FOUND OFF)
    MESSAGE(STATUS "Could not find CAFFE lib. Turning CAFFE_FOUND off")
ENDIF()

IF (CAFFE_FOUND)
  IF (NOT CAFFE_FIND_QUIETLY)
    MESSAGE(STATUS "Found CAFFE libraries: ${CAFFE_LIB}")
    MESSAGE(STATUS "Found CAFFE include: ${CAFFE_INCLUDE_DIR}")
  ENDIF (NOT CAFFE_FIND_QUIETLY)
ELSE (CAFFE_FOUND)
  IF (CAFFE_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find CAFFE")
  ENDIF (CAFFE_FIND_REQUIRED)
ENDIF (CAFFE_FOUND)

MARK_AS_ADVANCED(
    CAFFE_INCLUDE_DIR
    CAFFE_LIB
    CAFFE
)

