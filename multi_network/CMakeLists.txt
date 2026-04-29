# multi_network/CMakeLists.txt
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


set(multi_network_sources
  src/array_data.cpp
  src/connection.cpp
  src/event_routing_map.cpp
  src/linear_index.cpp
  src/sampler.cpp
  src/subconnector.cpp
  src/BIFO.cpp
  src/connector.cpp
  src/FIBO.cpp
  src/mpi_utils.cpp
  src/scheduler_agent.cpp
  src/temporal.cpp
  src/clock.cpp
  src/distributor.cpp
  src/index_map_factory.cpp
  src/multibuffer.cpp
  src/scheduler.cpp
  src/collector.cpp
  src/index_map.cpp
  src/parse.cpp
  src/setup.cpp
  src/configuration.cpp
  src/event_router.cpp
  src/port.cpp
  src/spatial.cpp
  src/runtime.cpp
  src/permutation_index.cpp
  src/predict_rank.cpp
  src/mpidep.cpp
  src/ContInPort.cpp
  src/EventOutPort.cpp
  src/encoder_runner.cpp
  )

add_library(multi_network SHARED ${multi_network_sources})

target_link_libraries(multi_network
  PUBLIC  multi_network_common
  PRIVATE ${MPI_CXX_LIBRARIES}
)

target_include_directories(multi_network
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/multi_network>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/nest
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/multi_network
  FILES_MATCHING PATTERN "*.h"
)

install(TARGETS multi_network
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/nest
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/nest
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
