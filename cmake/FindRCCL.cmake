# Try to find RCCL (ROCm Communication Collectives Library)
#
# The following variables are optionally searched for defaults
#  RCCL_ROOT_DIR: Base directory where all RCCL components are found
#  RCCL_INCLUDE_DIR: Directory where RCCL header is found
#  RCCL_LIB_DIR: Directory where RCCL library is found
#
# The following are set after configuration is done:
#  RCCL_FOUND
#  RCCL_INCLUDE_DIRS
#  RCCL_LIBRARIES
#
# The path hints include ROCM_PATH (or /opt/rocm) as this is the default
# location of the ROCm stack which RCCL is part of.

set(RCCL_ROOT_DIR $ENV{RCCL_ROOT_DIR} CACHE PATH "Folder contains AMD RCCL")

set(_ROCM_PATHS
  ${ROCM_PATH}
  $ENV{ROCM_PATH}
  /opt/rocm)

if(DEFINED ENV{USE_STATIC_RCCL} AND NOT "$ENV{USE_STATIC_RCCL}" STREQUAL "")
  message(STATUS "USE_STATIC_RCCL detected. Linking against static RCCL library")
  set(_use_static_rccl ON)
  set(RCCL_LIBNAME "librccl_static.a")
else()
  set(_use_static_rccl OFF)
  set(RCCL_LIBNAME "rccl")
endif()

find_path(RCCL_INCLUDE_DIR
  NAMES rccl/rccl.h
  HINTS
  ${RCCL_INCLUDE_DIR}
  ${RCCL_ROOT_DIR}
  ${RCCL_ROOT_DIR}/include
  ${_ROCM_PATHS}
  ${_ROCM_PATHS}/include)

find_library(RCCL_LIBRARY
  NAMES ${RCCL_LIBNAME}
  HINTS
  ${RCCL_LIB_DIR}
  ${RCCL_ROOT_DIR}
  ${RCCL_ROOT_DIR}/lib
  ${RCCL_ROOT_DIR}/lib/x86_64-linux-gnu
  ${RCCL_ROOT_DIR}/lib64
  ${_ROCM_PATHS}
  ${_ROCM_PATHS}/lib
  ${_ROCM_PATHS}/lib64)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RCCL DEFAULT_MSG RCCL_INCLUDE_DIR RCCL_LIBRARY)

if (RCCL_FOUND)
  set(RCCL_HEADER_FILE "${RCCL_INCLUDE_DIR}/rccl/rccl.h")
  message(STATUS "Determining RCCL version from the header file: ${RCCL_HEADER_FILE}")
  file (STRINGS ${RCCL_HEADER_FILE} RCCL_MAJOR_VERSION_DEFINED
        REGEX "^[ \t]*#define[ \t]+NCCL_MAJOR[ \t]+[0-9]+.*$" LIMIT_COUNT 1)
  if (RCCL_MAJOR_VERSION_DEFINED)
    string (REGEX REPLACE "^[ \t]*#define[ \t]+NCCL_MAJOR[ \t]+" ""
            RCCL_MAJOR_VERSION ${RCCL_MAJOR_VERSION_DEFINED})
    message(STATUS "RCCL_MAJOR_VERSION: ${RCCL_MAJOR_VERSION}")
  endif()
  set(RCCL_INCLUDE_DIRS ${RCCL_INCLUDE_DIR})
  set(RCCL_LIBRARIES ${RCCL_LIBRARY})
  message(STATUS "Found RCCL (include: ${RCCL_INCLUDE_DIRS}, library: ${RCCL_LIBRARIES})")
  mark_as_advanced(RCCL_ROOT_DIR RCCL_INCLUDE_DIRS RCCL_LIBRARIES)

  if(NOT TARGET RCCL::rccl AND RCCL_FOUND)
    if(_use_static_rccl)
      add_library(RCCL::rccl STATIC IMPORTED)
    else()
      add_library(RCCL::rccl SHARED IMPORTED)
    endif()
    set_target_properties(RCCL::rccl PROPERTIES
      IMPORTED_LOCATION ${RCCL_LIBRARIES}
      INTERFACE_INCLUDE_DIRECTORIES ${RCCL_INCLUDE_DIRS}
    )
  endif()
endif()
