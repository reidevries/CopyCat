cmake_minimum_required(VERSION 3.17)
cmake_policy(VERSION 3.17)
project(copycat VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(CAT_VERBOSITY_STRING "Critical Errors" CACHE STRING "Only critical errors are logged")
set_property(CACHE CAT_VERBOSITY_STRING PROPERTY STRINGS "Critical Errors" "General Errors" "General Messages" "Extra Messages" "Debug")
string(COMPARE EQUAL "${CAT_VERBOSITY_STRING}" "Critical Errors" result)
if (result)
	set(CAT_VERBOSITY 0)
endif()
string(COMPARE EQUAL "${CAT_VERBOSITY_STRING}" "General Errors" result)
if (result)
	set(CAT_VERBOSITY 1)
endif()
string(COMPARE EQUAL "${CAT_VERBOSITY_STRING}" "General Messages" result)
if (result)
	set(CAT_VERBOSITY 2)
endif()
string(COMPARE EQUAL "${CAT_VERBOSITY_STRING}" "Extra Messages" result)
if (result)
	set(CAT_VERBOSITY 3)
endif()
string(COMPARE EQUAL "${CAT_VERBOSITY_STRING}" "Debug" result)
if (result)
	set(CAT_VERBOSITY 4)
endif()

configure_file(src/catconf.h.in catconf.h)

find_package(raylib 3.0 QUIET) # Requires at least version 3.0
find_package(EnTT 2.7.3) #Requires at least version 2.7.3

option(CAT_REBUILD_CUSTOM_RAYLIB "Rebuild raylib from raylib source directory, for messing around with edits" OFF)

# This part downloads raylib and builds it if it's not installed on your system
# If there's none, or if you want to set fetch and build raylib
if ((NOT raylib_FOUND OR CAT_REBUILD_CUSTOM_RAYLIB) AND USE_LOCAL_RAYLIB)
  include(FetchContent)

  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/master.tar.gz
  )

  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
  
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

    # build raylib
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  elseif(CAT_REBUILD_CUSTOM_RAYLIB)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

if (NOT EnTT_FOUND)
  include(FetchContent)

  FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG cpp14
  )

  FetchContent_GetProperties(EnTT)
  if (NOT EnTT_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(EnTT)
    add_subdirectory(${entt_SOURCE_DIR} ${entt_BINARY_DIR})
  endif()
endif()

option(CAT_USE_SANITIZER_ASAN "Use some sanitizer compile flags and set raylib to compile with ASAN" ON)
SET(GCC_EXTRA_COMPILE_FLAGS "-fsanitize=leak -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined")
if (USE_SANITIZER_ASAN)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_EXTRA_COMPILE_FLAGS}")
endif()

add_executable(${PROJECT_NAME} "")
add_subdirectory(src)

#set(raylib_VERBOSE 1)
if(USE_LOCAL_RAYLIB)
	target_link_libraries(${PROJECT_NAME} raylib_static)
else()
	target_link_libraries(${PROJECT_NAME} raylib)
endif()
target_link_libraries(${PROJECT_NAME} EnTT)

target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
# That's it! You should have an example executable that you can run. Have fun!
