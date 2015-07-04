# This list is required for static linking

# ---[ Boost
find_package(Boost 1.46 REQUIRED COMPONENTS system thread)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

# ---[ Threads
find_package(Threads REQUIRED)

# ---[ Google-glog
find_package(Glog REQUIRED)
include_directories(SYSTEM ${GLOG_INCLUDE_DIRS})

# ---[ Google-gflags
find_package(GFlags REQUIRED)
include_directories(SYSTEM ${GFLAGS_INCLUDE_DIRS})

# ---[ Google-protobuf
find_package(Protobuf REQUIRED)
include_directories(SYSTEM ${PROTOBUF_INCLUDE_DIR})

# ---[ HDF5
find_package(HDF5 COMPONENTS HL REQUIRED)
include_directories(SYSTEM ${HDF5_INCLUDE_DIRS} ${HDF5_HL_INCLUDE_DIR})

# ---[ LMDB
find_package(LMDB REQUIRED)
include_directories(SYSTEM ${LMDB_INCLUDE_DIR})

# ---[ LevelDB
find_package(LevelDB REQUIRED)
include_directories(SYSTEM ${LevelDB_INCLUDE})

# ---[ Snappy
find_package(Snappy REQUIRED)
include_directories(SYSTEM ${Snappy_INCLUDE_DIR})

# ---[ ALE
find_package(ALE REQUIRED)
include_directories(SYSTEM ${ALE_INCLUDE_DIR})

# ---[ Caffe
find_package(Caffe REQUIRED)
include_directories(SYSTEM ${CAFFE_INCLUDE_DIR})

# ---[ OpenCV
find_package(OpenCV QUIET COMPONENTS core highgui imgproc imgcodecs)
if(NOT OpenCV_FOUND) # if not OpenCV 3.x, then imgcodecs are not found
  find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc)
endif()
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV found (${OpenCV_CONFIG_PATH})")

# ---[ BLAS
if(NOT APPLE)
  set(BLAS "Atlas" CACHE STRING "Selected BLAS library")
  set_property(CACHE BLAS PROPERTY STRINGS "Atlas;Open;MKL")

  if(BLAS STREQUAL "Atlas" OR BLAS STREQUAL "atlas")
    find_package(Atlas REQUIRED)
    include_directories(SYSTEM ${Atlas_INCLUDE_DIR})
  elseif(BLAS STREQUAL "Open" OR BLAS STREQUAL "open")
    find_package(OpenBLAS REQUIRED)
    include_directories(SYSTEM ${OpenBLAS_INCLUDE_DIR})
  elseif(BLAS STREQUAL "MKL" OR BLAS STREQUAL "mkl")
    find_package(MKL REQUIRED)
    include_directories(SYSTEM ${MKL_INCLUDE_DIR})
    add_definitions(-DUSE_MKL)
  endif()
elseif(APPLE)
  find_package(OpenBLAS REQUIRED)
  include_directories(SYSTEM ${OpenBLAS_INCLUDE_DIR})
endif()

# ---[ CUDA
include(cmake/Cuda.cmake)
if(NOT HAVE_CUDA)
  if(CPU_ONLY)
    message("-- CUDA is disabled. Building without it...")
  else()
    message("-- CUDA is not detected by cmake. Building without it...")
  endif()
  add_definitions(-DCPU_ONLY)
endif()

# ---[ SDL
include(cmake/SDL.cmake)

# ---[ Python
if(BUILD_python)
  if(NOT "${python_version}" VERSION_LESS "3.0.0")
    # use python3
    find_package(PythonInterp 3.0)
    find_package(PythonLibs 3.0)
    find_package(NumPy 1.7.1)
    # Find the matching boost python implementation
    set(version ${PYTHONLIBS_VERSION_STRING})
    
    STRING( REPLACE "." "" boost_py_version ${version} )
    find_package(Boost 1.46 COMPONENTS "python-py${boost_py_version}")
    set(Boost_PYTHON_FOUND ${Boost_PYTHON-PY${boost_py_version}_FOUND})
    
    while(NOT "${version}" STREQUAL "" AND NOT Boost_PYTHON_FOUND)
      STRING( REGEX REPLACE "([0-9.]+).[0-9]+" "\\1" version ${version} )
      STRING( REGEX MATCHALL "([0-9.]+).[0-9]+" has_more_version ${version} )
      if("${has_more_version}" STREQUAL "")
        break()
      endif()
      
      STRING( REPLACE "." "" boost_py_version ${version} )
      find_package(Boost 1.46 COMPONENTS "python-py${boost_py_version}")
      set(Boost_PYTHON_FOUND ${Boost_PYTHON-PY${boost_py_version}_FOUND})
    endwhile()
    if(NOT Boost_PYTHON_FOUND)
      find_package(Boost 1.46 COMPONENTS python)
    endif()
  else()
    # disable Python 3 search
    find_package(PythonInterp 2.7)
    find_package(PythonLibs 2.7)
    find_package(NumPy 1.7.1)
    find_package(Boost 1.46 COMPONENTS python)
  endif()
  if(PYTHONLIBS_FOUND AND NUMPY_FOUND AND Boost_PYTHON_FOUND)
    set(HAVE_PYTHON TRUE)
    if(BUILD_python_layer)
      add_definitions(-DWITH_PYTHON_LAYER)
      include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS} ${NUMPY_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
    endif()
  endif()
endif()
