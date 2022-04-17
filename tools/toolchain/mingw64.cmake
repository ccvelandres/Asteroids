# Toolchain for building for Windows in Linux Host

# Dependencies
# mingw-w64

# 32-bit -> i686-w64-mingw32
# 64-bit -> x86_64-w64-mingw32

set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

# set compilers
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

# set target environment on host build system
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
set(CMAKE_LIBRARY_PATH /usr/${TOOLCHAIN_PREFIX})
set(CMAKE_PREFIX_PATH /usr/${TOOLCHAIN_PREFIX})

# adjust behaviour of find commands
# look for header, libs in target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)