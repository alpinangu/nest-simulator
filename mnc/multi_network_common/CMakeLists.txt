# multi_network_common/CMakeLists.txt
#
# This file is part of NEST.
#
# Copyright (C) 2004 The NEST Initiative
#
# NEST is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# NEST is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with NEST.  If not, see <http://www.gnu.org/licenses/>.


add_library(multi_network_common STATIC
  ${PROJECT_SOURCE_DIR}/mnc/multi_network_common/connectivity.cpp
  ${PROJECT_SOURCE_DIR}/mnc/multi_network_common/application_map.cpp
  ${PROJECT_SOURCE_DIR}/mnc/multi_network_common/ioutils.cpp
  ${PROJECT_SOURCE_DIR}/mnc/multi_network_common/error.cpp
)


set_target_properties(multi_network_common PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(multi_network_common
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/mnc
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/multi_network>
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/mnc/multi_network_common/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/multi_network/multi_network_common
  FILES_MATCHING PATTERN "*.h"
)