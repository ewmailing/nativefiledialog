CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(NativeFileDialog)

INCLUDE(CMakeDependentOption)
INCLUDE(CheckFunctionExists)
#SET(CPACK_SET_DESTDIR TRUE)
#INCLUDE(CPack)

OPTION(NFD_BUILD_TESTS "Build tests" OFF)

IF(APPLE)
	IF(IOS)
		OPTION(WANTS_BUILD_SHARED_LIBRARY "Build dynamic library" OFF)
	ELSE()
		OPTION(WANTS_BUILD_SHARED_LIBRARY "Build dynamic library" ON)
		CMAKE_DEPENDENT_OPTION(WANTS_BUILD_FRAMEWORK "Set to ON to build framework instead of dylib. Only valid if BUILD_SHARED_LIBRARY is ON an is OS X." ON "WANTS_BUILD_SHARED_LIBRARY" ON)
	ENDIF()
ELSE()
	OPTION(WANTS_BUILD_SHARED_LIBRARY "Build dynamic library" ON)
ENDIF()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/" ${CMAKE_MODULE_PATH})



IF(WANTS_BUILD_SHARED_LIBRARY)
# RPATH stuff. This MUST be set before any targets that use it are declared.
IF("${CMAKE_SYSTEM}" MATCHES "Linux" AND NOT ANDROID)
	# RPATH $ORIGIN is only for Linux and Solaris
	# FIXME: match for Solaris too

	# use, i.e. don't skip the full RPATH for the build tree
	#	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

	# when building, don't use the install RPATH already
	# (but later on when installing)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

	#SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

	# add the automatically determined parts of the RPATH
	# which point to directories outside the build tree to the install RPATH
	#	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


	# the RPATH to be used when installing, but only if it's not a system directory
	#	LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
	#	IF("${isSystemDir}" STREQUAL "-1")
	#   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

	#    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:\\$$ORIGIN")
	#	ENDIF("${isSystemDir}" STREQUAL "-1")
	SET(CMAKE_INSTALL_RPATH "\$ORIGIN")
	#    SET(CMAKE_INSTALL_RPATH "/usr/lib${CMAKE_INSTALL_RPATH}:\\\$ORIGIN")
ELSEIF(APPLE)
	set(CMAKE_MACOSX_RPATH 1)
	# use, i.e. don't skip the full RPATH for the build tree
	#	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

	# when building, don't use the install RPATH already
	# (but later on when installing)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

	#SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

	# add the automatically determined parts of the RPATH
	# which point to directories outside the build tree to the install RPATH
	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)	
ENDIF()
ENDIF(WANTS_BUILD_SHARED_LIBRARY)

SET(NFD_C_FLAGS "")

IF(WANTS_BUILD_SHARED_LIBRARY)
	IF("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
		SET(NFD_C_FLAGS "${NFD_C_FLAGS} -fvisibility=hidden")
	ELSEIF("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
		SET(NFD_C_FLAGS "${NFD_C_FLAGS} -fvisibility=hidden")
	ENDIF()
	
	SET(NFD_C_FLAGS "${NFD_C_FLAGS} -DBUILD_NFD_AS_DLL")

ENDIF(WANTS_BUILD_SHARED_LIBRARY)

IF(APPLE)

	IF(WANTS_BUILD_FRAMEWORK)
#		SET(CMAKE_FRAMEWORK_INSTALL_DIR "/Library/Frameworks" CACHE STRING "Directory to install frameworks to.")
#		SET(LUA_FRAMEWORK_NAME "${LUA_LIBRARY_NAME}.framework")
#		SET(LUA_FRAMEWORK_VERSION_NUMBER "5.3")
#		SET(LUA_FRAMEWORK_VERSIONED_EXECUTABLE_DIR "Versions/${LUA_FRAMEWORK_VERSION_NUMBER}/MacOS")
#		SET(LUA_FRAMEWORK_VERSIONED_LIB_DIR "Versions/${LUA_FRAMEWORK_VERSION_NUMBER}/lib")
#		SET(LUA_FRAMEWORK_CURRENT_EXECUTABLE_DIR "Versions/Current/MacOS")

		# For Apple install_name, is it better to detect if Xcode vs Makefile?
		# Xcode default=1, Makefile=0? Or detect if Framework vs. dylib,
		# Framework=1, dylib=0?
		#OPTION(CMAKE_BUILD_WITH_INSTALL_RPATH "Set to YES to set the rpath or install_name on build instead of install." ON)
	ENDIF(WANTS_BUILD_FRAMEWORK)
ENDIF(APPLE)


SET(PUBLIC_HEADERS
	${CMAKE_SOURCE_DIR}/src/include/nfd.h
)

SET(SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/common.h
	${CMAKE_SOURCE_DIR}/src/nfd_common.c
	${CMAKE_SOURCE_DIR}/src/nfd_common.h
)

# Experimental and not finished. Need to make an option.
# Also need to do NDEBUG for release because standalone uses assert.h
# ADD_DEFINITIONS(-DNFD_COMPILE_AS_STANDALONE)

SET(ADDITIONAL_LIBRARIES "")


IF(APPLE)
	IF(IOS)
	ELSE()
		LIST(APPEND SOURCE_FILES
			${CMAKE_SOURCE_DIR}/src/nfd_cocoa.m
		)
		FIND_LIBRARY(APPLE_COCOA_LIBRARY Cocoa)
		LIST(APPEND ADDITIONAL_LIBRARIES 
			${APPLE_COCOA_LIBRARY}
		)
	ENDIF()
ELSEIF(WIN32)
		LIST(APPEND SOURCE_FILES
			${CMAKE_SOURCE_DIR}/src/nfd_win.cpp
		)

		FIND_LIBRARY(WINDOWS_COMCTL_LIBRARY comtl32)
		LIST(APPEND ADDITIONAL_LIBRARIES 
			${WINDOWS_COMCTL_LIBRARY}
		)

ELSEIF(ANDROID)
ELSEIF(UNIX)
	LIST(APPEND SOURCE_FILES
		${CMAKE_SOURCE_DIR}/src/nfd_gtk.c
	)

	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	# Setup CMake to use GTK+, tell the compiler where to look for headers
	# and to the linker where to look for libraries
	include_directories(${GTK3_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})
	# Add other flags to the compiler
	#add_definitions(${GTK3_CFLAGS_OTHER})
	LIST(APPEND NFD_C_FLAGS ${GTK3_CFLAGS_OTHER})

	LIST(APPEND ADDITIONAL_LIBRARIES 
		${GTK3_LIBRARIES}
	)


ELSE()

ENDIF()

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/src/include
	${CMAKE_SOURCE_DIR}/src
)

IF(WANTS_BUILD_SHARED_LIBRARY)
	ADD_LIBRARY(nfd SHARED 
		${SOURCE_FILES}
		${PUBLIC_HEADERS}
)
ELSE()
	ADD_LIBRARY(nfd STATIC 
		${SOURCE_FILES}
		${PUBLIC_HEADERS}
	)
ENDIF()

TARGET_LINK_LIBRARIES(nfd
	${ADDITIONAL_LIBRARIES}
)

IF(APPLE)
	INCLUDE(XcodeDefaults)
	BLURRR_CONFIGURE_XCODE_DEFAULTS(nfd)
ENDIF()



# Set Dynamic Library and Framework properties
IF(WANTS_BUILD_SHARED_LIBRARY AND WANTS_BUILD_FRAMEWORK)

	SET_TARGET_PROPERTIES(nfd PROPERTIES
		FRAMEWORK TRUE
#		FRAMEWORK_VERSION "${LUA_FRAMEWORK_VERSION_NUMBER}"
		#  PRIVATE_HEADER "fooPrivate.h;fooBoth.h"
		PUBLIC_HEADER "${PUBLIC_HEADERS}"
#		RESOURCE "${RESOURCE_FILES}"
		# INSTALL_NAME_DIR ${CMAKE_FRAMEWORK_INSTALL_NAME_DIR}
		# BUILD_WITH_INSTALL_RPATH ${CMAKE_BUILD_WITH_INSTALL_RPATH} 
#		VERSION ${LUA_VERSION}
#		SOVERSION ${LUA_COMPATIBILITY_VERSION}
		COMPILE_FLAGS "${NFD_C_FLAGS}"
	)
	
	# I moved the INSTALL_NAME_DIR to use SET_PROPERTY instead because
	# SET_TARGET_PROPERTIES will fail if the variable is empty.
	#	SET_PROPERTY(TARGET lua_library_dynamic PROPERTY INSTALL_NAME_DIR ${CMAKE_FRAMEWORK_INSTALL_NAME_DIR})
	#	SET_PROPERTY(TARGET lua_library_dynamic PROPERTY BUILD_WITH_INSTALL_RPATH NO)
	# If the user deletes the install_name path, use the gcc default
	# and disable the option completely. But CMake by default places
	# the name of the library in the install_name if 
	# BUILD_WITH_INSTALL_RPATH is ON. So to avoid this, I need to 
	# disable the switch.
#	IF(CMAKE_FRAMEWORK_INSTALL_NAME_DIR)
		#		MESSAGE("got dir ${CMAKE_FRAMEWORK_INSTALL_NAME_DIR}")
#		SET_PROPERTY(TARGET lua_library_dynamic PROPERTY BUILD_WITH_INSTALL_RPATH ${CMAKE_BUILD_WITH_INSTALL_RPATH})

#	ELSE(CMAKE_FRAMEWORK_INSTALL_NAME_DIR)
		#		MESSAGE("no dir ${CMAKE_FRAMEWORK_INSTALL_NAME_DIR}")
#		SET_PROPERTY(TARGET lua_library_dynamic PROPERTY BUILD_WITH_INSTALL_RPATH NO)

#	ENDIF(CMAKE_FRAMEWORK_INSTALL_NAME_DIR)



	# Short Version is the "marketing version". It is the version
	# the user sees in an information panel.
#	SET(MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${LUA_MAJOR_VERSION}.${LUA_MINOR_VERSION}.${LUA_PATCH_VERSION}")
	# Bundle version is the version the OS looks at.
#	SET(MACOSX_FRAMEWORK_BUNDLE_VERSION "${LUA_MAJOR_VERSION}.${LUA_MINOR_VERSION}.${LUA_PATCH_VERSION}")
	SET(MACOSX_FRAMEWORK_IDENTIFIER "com.frogtoss.nfd")
	#	SET(MACOSX_BUNDLE_BUNDLE_NAME "lua" )

ELSEIF(WANTS_BUILD_SHARED_LIBRARY)
	IF(APPLE)
		SET_TARGET_PROPERTIES(nfd PROPERTIES
			FRAMEWORK FALSE
			# INSTALL_NAME_DIR ${CMAKE_DYLIB_INSTALL_NAME_DIR}
			# BUILD_WITH_INSTALL_RPATH ${CMAKE_BUILD_WITH_INSTALL_RPATH} 
#			VERSION ${LUA_VERSION}
#			SOVERSION ${LUA_COMPATIBILITY_VERSION}
			COMPILE_FLAGS "${NFD_C_FLAGS}"
		)
		# I moved the INSTALL_NAME_DIR to use SET_PROPERTY instead because
		# SET_TARGET_PROPERTIES will fail if the variable is empty.
#		SET_PROPERTY(TARGET lua_library_dynamic PROPERTY INSTALL_NAME_DIR ${CMAKE_DYLIB_INSTALL_NAME_DIR})
#		SET_PROPERTY(TARGET lua_library_dynamic PROPERTY BUILD_WITH_INSTALL_RPATH NO)
		# If the user deletes the install_name path, use the gcc default
		# and disable the option completely. But CMake by default places
		# the name of the library in the install_name if 
		# BUILD_WITH_INSTALL_RPATH is ON. So to avoid this, I need to 
		# disable the switch.
#		IF(CMAKE_DYLIB_INSTALL_NAME_DIR)
#			SET_PROPERTY(TARGET lua_library_dynamic PROPERTY BUILD_WITH_INSTALL_RPATH ${CMAKE_BUILD_WITH_INSTALL_RPATH})
#		ELSE(CMAKE_DYLIB_INSTALL_NAME_DIR)
#			SET_PROPERTY(TARGET lua_library_dynamic PROPERTY BUILD_WITH_INSTALL_RPATH OFF)
#		ENDIF(CMAKE_DYLIB_INSTALL_NAME_DIR)



	ELSEIF(UNIX)
		IF(NOT ANDROID)
			SET_TARGET_PROPERTIES(nfd PROPERTIES
				INSTALL_RPATH ${CMAKE_INSTALL_RPATH}
				BUILD_WITH_INSTALL_RPATH ${CMAKE_BUILD_WITH_INSTALL_RPATH} 
#				VERSION ${LUA_VERSION}
#				SOVERSION ${LUA_SOVERSION}
				COMPILE_FLAGS "${NFD_C_FLAGS}"
			)
		ENDIF(NOT ANDROID)
	
	ELSEIF(WIN32)
		SET_TARGET_PROPERTIES(nfd PROPERTIES
#			VERSION ${LUA_VERSION}
#			SOVERSION ${LUA_COMPATIBILITY_VERSION}
			COMPILE_FLAGS "${NFD_C_FLAGS}"
		)

	ELSE(APPLE)
		SET_TARGET_PROPERTIES(nfd PROPERTIES
#			VERSION ${LUA_VERSION}
#			SOVERSION ${LUA_COMPATIBILITY_VERSION}
			COMPILE_FLAGS "${NFD_C_FLAGS}"
		)

	ENDIF(APPLE)


ENDIF(WANTS_BUILD_SHARED_LIBRARY AND WANTS_BUILD_FRAMEWORK)

IF(NOT WANTS_BUILD_FRAMEWORK)
	INSTALL(TARGETS
		nfd
		RUNTIME DESTINATION lib		
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		PUBLIC_HEADER DESTINATION include
	)
	# Hmmm...install TARGETS is not installing headers
	INSTALL(FILES ${PUBLIC_HEADERS} DESTINATION include)
ELSE()
	INSTALL(TARGETS
		nfd
		FRAMEWORK DESTINATION Frameworks
	)


ENDIF()

# Decided to put everything in this CMakeLists.txt to make keeping in sync easier since this isn't official.
#ADD_SUBDIRECTORY(test)
IF(NFD_BUILD_TESTS)

	ADD_EXECUTABLE(test_opendialog MACOSX_BUNDLE WIN32
		${CMAKE_SOURCE_DIR}/test/test_opendialog.c)
	TARGET_LINK_LIBRARIES(test_opendialog nfd)

	ADD_EXECUTABLE(test_opendialogmultiple MACOSX_BUNDLE WIN32
		${CMAKE_SOURCE_DIR}/test/test_opendialogmultiple.c)
	TARGET_LINK_LIBRARIES(test_opendialogmultiple nfd)

	ADD_EXECUTABLE(test_savedialog MACOSX_BUNDLE WIN32
		${CMAKE_SOURCE_DIR}/test/test_savedialog.c)
	TARGET_LINK_LIBRARIES(test_savedialog nfd)

ENDIF()

