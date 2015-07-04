

SET(ALE_INCLUDE_SEARCH_PATHS
  /usr/include
  /usr/include/ale
  /usr/local/include
  /usr/local/include/ale
  /opt/ALE/include
  $ENV{HOME}/ale/src
  $ENV{ALE_HOME}
  $ENV{ALE_HOME}/src
)

SET(ALE_LIB_SEARCH_PATHS
        /lib/
        /lib/ale
        /lib64/
        /usr/lib
        /usr/lib/ale
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        $ENV{ALE_HOME}
        $ENV{ALE_HOME}/lib
        $ENV{HOME}/ale
 )

FIND_PATH(ALE_INCLUDE_DIR NAMES ale_interface.hpp PATHS ${ALE_INCLUDE_SEARCH_PATHS})
FIND_LIBRARY(ALE_LIB NAMES ale PATHS ${ALE_LIB_SEARCH_PATHS})

SET(ALE_FOUND ON)

#    Check include files
IF(NOT ALE_INCLUDE_DIR)
    SET(ALE_FOUND OFF)
    MESSAGE(STATUS "Could not find ALE include. Turning ALE_FOUND off")
ENDIF()

#    Check libraries
IF(NOT ALE_LIB)
    SET(ALE_FOUND OFF)
    MESSAGE(STATUS "Could not find ALE lib. Turning ALE_FOUND off")
ENDIF()

IF (ALE_FOUND)
  IF (NOT ALE_FIND_QUIETLY)
    MESSAGE(STATUS "Found ALE libraries: ${ALE_LIB}")
    MESSAGE(STATUS "Found ALE include: ${ALE_INCLUDE_DIR}")
  ENDIF (NOT ALE_FIND_QUIETLY)
ELSE (ALE_FOUND)
  IF (ALE_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find ALE")
  ENDIF (ALE_FIND_REQUIRED)
ENDIF (ALE_FOUND)

MARK_AS_ADVANCED(
    ALE_INCLUDE_DIR
    ALE_LIB
    ALE
)

