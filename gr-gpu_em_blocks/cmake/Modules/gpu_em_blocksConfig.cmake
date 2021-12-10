INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_GPU_EM_BLOCKS gpu_em_blocks)

FIND_PATH(
    GPU_EM_BLOCKS_INCLUDE_DIRS
    NAMES gpu_em_blocks/api.h
    HINTS $ENV{GPU_EM_BLOCKS_DIR}/include
        ${PC_GPU_EM_BLOCKS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GPU_EM_BLOCKS_LIBRARIES
    NAMES gnuradio-gpu_em_blocks
    HINTS $ENV{GPU_EM_BLOCKS_DIR}/lib
        ${PC_GPU_EM_BLOCKS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gpu_em_blocksTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GPU_EM_BLOCKS DEFAULT_MSG GPU_EM_BLOCKS_LIBRARIES GPU_EM_BLOCKS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GPU_EM_BLOCKS_LIBRARIES GPU_EM_BLOCKS_INCLUDE_DIRS)
