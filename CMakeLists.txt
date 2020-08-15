cmake_minimum_required(VERSION 3.17)
cmake_policy(VERSION 3.17)
project(copycat VERSION 0.1)

set(OUTPUT_EXT "")
if (${PLATFORM} MATCHES "Web")
	set(PLATFORM_WEB ON)
	set(OUTPUT_EXT ".html")
	string(APPEND PROJECT_NAME ${OUTPUT_EXT})
endif()

add_executable(${PROJECT_NAME} "")
add_subdirectory(src)

target_compile_features(${PROJECT_NAME}
    PRIVATE
    cxx_std_17
)
set(CMAKE_CXX_STANDARD 17) 		#needs C++17 for EnTTz

#Give strings to the different levels of verbosity in my own debug output
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

#set location of web libraylib.bc
set(WEB_BC_LOCATION "${CMAKE_SOURCE_DIR}/third_party/raylib-build-emscripten/src/libraylib.bc" CACHE STRING "location of raylib .bc file for emscripten")
set(WEB_SHELL_LOCATION "${CMAKE_SOURCE_DIR}/src/shell.html" CACHE STRING "location of shell html file for emscripten")
set(WEB_FONT_DIR "${CMAKE_SOURCE_DIR}/font" CACHE STRING "dir for fonts")
set(WEB_AUDIO_DIR "${CMAKE_SOURCE_DIR}/audio" CACHE STRING "dir for audio")
set(WEB_LEVEL_DIR "${CMAKE_SOURCE_DIR}/level" CACHE STRING "dir for levels")
set(WEB_SPRITE_DIR "${CMAKE_SOURCE_DIR}/sprite" CACHE STRING "dir for sprites")

#Use strings to customize configuration file
configure_file(src/CatConf.hpp.in CatConf.hpp)

#find, optionally download, and (re-)build raylib
find_package(raylib 3.0 QUIET) # Requires at least version 3.0
option(CAT_REBUILD_CUSTOM_RAYLIB
	"Rebuild raylib from raylib source directory, for edits/testing"
	OFF
)
option(USE_LOCAL_RAYLIB
	"Use the raylib in third_party folder, not the dynamic library or internet download"
	ON
)
# This part downloads raylib and builds it if it's not installed on your system
# If there's none, or if you want to set fetch and build raylib
if (USE_LOCAL_RAYLIB)
	if(${PLATFORM} MATCHES "Desktop")
		add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/raylib-src" "${CMAKE_SOURCE_DIR}/third_party/raylib-build")
	elseif(${PLATFORM} MATCHES "Web")
		#add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/raylib-src" "${CMAKE_SOURCE_DIR}/third_party/raylib-build-emscripten")
	endif()
else()
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

#Use ASAN sanitization flags
if (USE_SANITIZER_ASAN)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined")
endif()

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
	target_compile_options(${PROJECT_NAME}
		PRIVATE
		-Wall -Wextra -Wpedantic
	)
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC
	"${CMAKE_SOURCE_DIR}/third_party"
	"${CMAKE_SOURCE_DIR}/third_party/raylib-build/src"
	"${PROJECT_BINARY_DIR}"
)

#set(raylib_VERBOSE 1)
if (${PLATFORM} MATCHES "Web")
	message(STATUS "building for web with emscripten")
	target_link_libraries(${PROJECT_NAME} ${WEB_BC_LOCATION})
	target_link_libraries(${PROJECT_NAME} -lglfw)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -s LLD_REPORT_UNDEFINED -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file ${WEB_SHELL_LOCATION}")
	string(REPLACE "-rdynamic" "" CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS})
	
	# For generating .data files for web
	if(EXISTS ${WEB_FONT_DIR})
	# The local resources path needs to be mapped to /resources virtual path
		string(APPEND WEB_FONT_DIR "@font")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file-folder ${WEB_FONT_DIR}")
	endif()
	if(EXISTS ${WEB_AUDIO_DIR})
		string(APPEND WEB_FONT_DIR "@audio")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file ${WEB_AUDIO_DIR}")
	endif()
	if(EXISTS ${WEB_LEVEL_DIR})
		string(APPEND WEB_FONT_DIR "@level")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file ${WEB_LEVEL_DIR}")
	endif()
	if(EXISTS ${WEB_SPRITE_DIR})
		string(APPEND WEB_FONT_DIR "@sprite")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file ${WEB_SPRITE_DIR}")
	endif()
else()
	if(USE_LOCAL_RAYLIB)
		target_link_libraries(${PROJECT_NAME} raylib_static)
	else()
		target_link_libraries(${PROJECT_NAME} raylib)
	endif()
endif()
