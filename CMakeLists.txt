cmake_minimum_required(VERSION 3.1.0)

project(HBVisionGraphEditor)

if(MSVC)
  set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(3P_LIBRARY_OUTPUT_PATH         ${CMAKE_BINARY_DIR}/3rdparty/lib${LIB_SUFFIX})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenCV REQUIRED)

# build options
option(ENABLE_SOLUTION_FOLDERS "Solution folder in Visual Studio or in other IDEs" MSVC_IDE)

if(ENABLE_SOLUTION_FOLDERS)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include)

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_lower)
if(cmake_build_type_lower MATCHES "debug")
    # debug defines
endif()

add_definitions(-std=c++11)

add_subdirectory(src)

if (NOT NO_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Tests disabled")
endif()
