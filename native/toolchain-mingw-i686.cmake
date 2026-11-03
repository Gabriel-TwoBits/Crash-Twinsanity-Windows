# 32 bit Windows from Linux (native/WINDOWS32.md): clang for i686-w64-mingw32 with the MinGW-w64 headers, runtime and libstdc++
# of the distribution's cross compiler (Debian: g++-mingw-w64-i686-posix), linked by lld. The compilers are given by the build
# script (native/tools/build_windows32.py): CMAKE_C_COMPILER and CMAKE_CXX_COMPILER, a clang of 21 or later
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86)
set(CMAKE_C_COMPILER_TARGET i686-w64-mingw32)
set(CMAKE_CXX_COMPILER_TARGET i686-w64-mingw32)
set(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
# libstdc++'s threads are winpthreads' (the posix flavour of the cross compiler), which clang's MinGW driver doesn't add
set(CMAKE_C_STANDARD_LIBRARIES "-lpthread" CACHE STRING "")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lpthread" CACHE STRING "")

set(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
