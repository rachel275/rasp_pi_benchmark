cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(vulkanMatrixMultiplication)

include_directories(base)
# include_directories(external)

OPTION(USE_HEADLESS "Build the project using headless extension swapchain" ON)

set(RESOURCE_INSTALL_DIR "" CACHE PATH "Path to install resources to (leave empty for running uninstalled)")

if (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
	message(STATUS "Using module to find Vulkan")
	find_package(Vulkan)
endif()

IF (NOT Vulkan_FOUND)
	find_library(Vulkan_LIBRARY NAMES vulkan HINTS "$ENV{VULKAN_SDK}/lib" "${CMAKE_SOURCE_DIR}/libs/vulkan" REQUIRED)
	IF (Vulkan_LIBRARY)
		set(Vulkan_FOUND ON)
		MESSAGE("Using bundled Vulkan library version")
	ENDIF()
ENDIF()

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
ENDIF()

find_package(Threads REQUIRED)
IF(USE_HEADLESS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_HEADLESS_EXT")
ENDIF()

# Set preprocessor defines
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE *.cpp )

# shader path
if(RESOURCE_INSTALL_DIR)
	add_definitions(-DVK_EXAMPLE_SHADERS_DIR=\"${RESOURCE_INSTALL_DIR}/shaders/\")
	install(DIRECTORY shaders/ DESTINATION ${RESOURCE_INSTALL_DIR}/shaders/)
else()
	if(NOT USE_RELATIVE_ASSET_PATH)
		add_definitions(-DVK_EXAMPLE_SHADERS_DIR=\"${CMAKE_SOURCE_DIR}/shaders/\")
	endif()
endif()


link_libraries(${Vulkan_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

add_subdirectory(base)
add_subdirectory(src)
