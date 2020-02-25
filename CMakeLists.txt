# Project <proj> main cmake file

cmake_minimum_required(VERSION 3.2)

project(jwein_blog VERSION 0.0.1)

# Option to install targets to env location
if(DEFINED ENV{JWEIN_BLOG})
     message("-- Will install JWEIN_BLOG binaries and libraries to location: " $ENV{JWEIN_BLOG})
	 set(CMAKE_INSTALL_PREFIX $ENV{JWEIN_BLOG})
else(DEFINED ENV{JWEIN_BLOG})
     message("-- Will install JWEIN_BLOG binaries and libraries to location: " ${CMAKE_CURRENT_BINARY_DIR})
	 set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif(DEFINED ENV{JWEIN_BLOG})


# Disable some windows specific warnings for using more modern c functions
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

# Options
option(WITH_testing "Allows tests to be run" ON)

if(WITH_testing MATCHES ON)
   enable_testing()
endif(WITH_testing MATCHES ON)


