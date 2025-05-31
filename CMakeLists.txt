cmake_minimum_required(VERSION 3.31)
project(Analog_Clock LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static")
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif ()

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
	message("downloading raylib from git.")
	include(FetchContent)
	FetchContent_Declare(
			raylib
			DOWNLOAD_EXTRACT_TIMESTAMP OFF
			URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
	)
	FetchContent_GetProperties(raylib)
	if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
		set(FETCHCONTENT_QUIET NO)
		FetchContent_MakeAvailable(raylib)
		set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
	endif ()
endif ()

# Project
add_executable(${PROJECT_NAME} src/main.cpp
		src/Application.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if (WIN32)
	set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY WIN32_EXECUTABLE TRUE)

	target_sources(${PROJECT_NAME} PRIVATE res/app.rc)
elseif (APPLE)
	set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY MACOSX_BUNDLE TRUE)

	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit")
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
elseif (UNIX AND NOT APPLE)
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

	target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
	# Uncomment if using Wayland
	# target_link_libraries(${PROJECT_NAME} PRIVATE ${Wayland_LIBRARIES})
endif ()

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/res")
set(DEST_DIR "${CMAKE_BINARY_DIR}")
file(COPY ${SOURCE_DIR} DESTINATION ${DEST_DIR})

if (${PLATFORM} STREQUAL "Web")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Os -Wall -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s GL_ENABLE_GET_PROC_ADDRESS=1 -s EXPORTED_RUNTIME_METHODS=['HEAPF32'] --preload-file res")
	set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif ()
