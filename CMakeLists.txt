cmake_minimum_required(VERSION 3.5)
project(libsmt)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -std=c++11")


# Add path for custom modules
list (APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


find_package(Gmp REQUIRED)
if (GMP_FOUND)
  include_directories (${GMP_INCLUDE_DIR})
  include_directories (${GMPXX_INCLUDE_DIR})
else()
  set(GMP_LIB "")
  set(GMPXX_LIB "")
endif()



##### z3
# We use a modified version of z3. Please pass the path of z3 explicitly.
set(Z3_INCLUDES "" CACHE FILEPATH "Path to your z3 includes")
include_directories(${Z3_INCLUDES}/src/api/c++)
include_directories(${Z3_INCLUDES}/src/api)

if(APPLE)
    set(Z3_LIB ${Z3_INCLUDES}/build/libz3.dylib)
endif()

if(UNIX AND NOT APPLE)
    set(Z3_LIB ${Z3_INCLUDES}/build/libz3.so)
endif()

set(SOURCE_FILES
        utils/timer.cpp
        utils/base64.cpp
        utils/string2int.cpp
        
        opt/opt_util.cpp
        opt/opt.cpp
        
        main.cpp
        )

add_executable(libsmt ${SOURCE_FILES})
target_link_libraries(libsmt
        ${Z3_LIB}
        )
        
       


