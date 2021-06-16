project(pcgcpp)

cmake_minimum_required(VERSION 3.7)

option(BUILD_SHARED_LIBS "Build shared instead of static library" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_PKGCONFIG "Build in PKGCONFIG mode" OFF)

set(BUILD_DEFINES "" CACHE STRING "Build defines")

if(MSVC)
	option(STATIC_CRT "Use static CRT libraries" OFF)

	# Rewrite command line flags to use /MT if necessary
	if(STATIC_CRT)
		foreach(flag_var
				CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
				CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${flag_var} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
			endif(${flag_var} MATCHES "/MD")
		endforeach(flag_var)
	endif()
endif()

# Pre-defines standard install locations on *nix systems.
include(GNUInstallDirs)
mark_as_advanced(CLEAR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)

include_directories(include)
set(HEADERS include/pcg_extras.hpp include/pcg_random.hpp)
set(SOURCES ${HEADERS} pcgcpp_impl.cpp)

if(DEFINED BUILD_DEFINES)
	foreach(DEFINE ${BUILD_DEFINES})
		add_definitions("-D" ${DEFINE})
	endforeach()
endif()

if(BUILD_SHARED_LIBS)
	add_library(pcgcpp SHARED ${SOURCES})
else()
	add_library(pcgcpp STATIC ${SOURCES})
endif()

# Enable C++11 long long for compilers that are capable of it
if(NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} STRLESS 3.1 AND ";${CMAKE_CXX_COMPILE_FEATURES};" MATCHES ";cxx_long_long_type;")
	target_compile_features(pcgcpp PUBLIC cxx_long_long_type)
endif()

set_target_properties(pcgcpp PROPERTIES VERSION 5b5cac8)
get_target_property(PCGCPP_VERSION_STRING pcgcpp VERSION)

if(BUILD_PKGCONFIG)
	# Install library into its own directory under LIBDIR
	set(INSTALL_SUFFIX /pcgcpp-${PCGCPP_VERSION_STRING})
endif()

install(TARGETS pcgcpp EXPORT pcgcpp-config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}${INSTALL_SUFFIX})
install(EXPORT pcgcpp-config DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pcgcpp)

if(BUILD_PKGCONFIG)
	configure_file(scripts/pcgcpp.pc.in ${PROJECT_BINARY_DIR}/pcgcpp.pc @ONLY)
	install(FILES ${PROJECT_BINARY_DIR}/pcgcpp.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
endif()

if(BUILD_TESTS)
	file(GLOB TEST_SOURCES tests/*.cpp)
	file(GLOB FUZZ_SOURCES tests/fuzz_*.cpp)
	list(REMOVE_ITEM TEST_SOURCES ${FUZZ_SOURCES})

	add_executable(check ${TEST_SOURCES})
	target_link_libraries(check pcgcpp)
	add_custom_command(TARGET check POST_BUILD COMMAND check WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
