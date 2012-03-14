# Try to find the Kipi Plugins library and defines the following variables
# KIPI_PLUGINS_FOUND
# KIPI_PLUGINS_INCLUDE_DIR
# KIPI_PLUGINS_LIBRARIES
#
# WARNING: Do not use it, this is just a temporary hack until KTp library can be
#          considered stable or until Kipi Plugin library is installed

# Copyright (c) 2012, Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (NOT IS_KIPI_PLUGINS_INTERNAL_MODULE)
   message (FATAL_ERROR "This is just a temporary hack until KTp library can be considered stable
                         or until Kipi Plugin library is installed. Do not use FindKipiPlugins.cmake")
endif (NOT IS_KIPI_PLUGINS_INTERNAL_MODULE)

set (KIPI_PLUGINS_FIND_REQUIRED ${KipiPlugins_FIND_REQUIRED})
if (KIPI_PLUGINS_INCLUDE_DIR AND KIPI_PLUGINS_LIBRARIES)
  # Already in cache, be silent
  set(KIPI_PLUGINS_FIND_QUIETLY TRUE)
endif (KIPI_PLUGINS_INCLUDE_DIR AND KIPI_PLUGINS_LIBRARIES)

set (KIPI_PLUGINS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libkipiplugins;${CMAKE_SOURCE_DIR}/libkipiplugins/tools;${CMAKE_SOURCE_DIR}/libkipiplugins/widgets")

find_library(KIPI_PLUGINS_LIBRARIES NAMES kipiplugins)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(KipiPlugins DEFAULT_MSG
                                  KIPI_PLUGINS_LIBRARIES
                                  KIPI_PLUGINS_INCLUDE_DIR)

set(AREA_CODE_LOADING "51001")
add_definitions(-DAREA_CODE_LOADING=${AREA_CODE_LOADING})

mark_as_advanced(KIPI_PLUGINS_INCLUDE_DIR KIPI_PLUGINS_LIBRARIES)
