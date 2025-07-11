# multi_network_manager/CMakeLists.txt
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


set(SRC  
    src/array_data.cpp nest/multi_network/array_data.h
    src/array_data.cpp nest/multi_network/array_data.h
    src/BIFO.cpp nest/multi_network/BIFO.h
    src/clock.cpp nest/multi_network/clock.h
    src/collector.cpp nest/multi_network/collector.h
    src/communication.cpp nest/multi_network/communication.h
    src/config.cpp nest/multi_network/config.h
    src/connection.cpp        nest/multi_network/connection.h
    src/connector.cpp         nest/multi_network/connector.h
    src/cont_data.cpp         nest/multi_network/cont_data.h
    src/data_map.cpp          nest/multi_network/data_map.h
    src/debug.cpp             nest/multi_network/debug.h
    src/distributor.cpp       nest/multi_network/distributor.h
    src/event_router.cpp      nest/multi_network/event_router.h
    src/event_routing_map.cpp      nest/multi_network/event_routing_map.h
    src/event.cpp             nest/multi_network/event.h
    src/FIBO.cpp              nest/multi_network/FIBO.h
    src/index_map_factory.cpp nest/multi_network/index_map_factory.h
    src/index_map.cpp         nest/multi_network/index_map.h
    src/interval_table.cpp    nest/multi_network/interval_table.h
    src/interval_tree.cpp     nest/multi_network/interval_tree.h
    src/interval.cpp          nest/multi_network/interval.h
    src/linear_index.cpp      nest/multi_network/linear_index.h
    src/message.cpp           nest/multi_network/message.h
    src/mpi_utils.cpp         nest/multi_network/mpi_utils.h
    src/ordered_ilist.cpp     nest/multi_network/ordered_ilist.h
    src/parse.cpp             nest/multi_network/parse.h
    src/port.cpp              nest/multi_network/port.h
    src/sampler.cpp           nest/multi_network/sampler.h
    src/setup.cpp             nest/multi_network/setup.h
    src/spatial.cpp           nest/multi_network/spatial.h
    src/subconnector.cpp      nest/multi_network/subconnector.h
    src/temporal.cpp          nest/multi_network/temporal.h
    )

add_library(multi_network SHARED ${SRC})

target_include_directories(multi_network
  PRIVATE
    ${PROJECT_SOURCE_DIR}/nestkernel
    ${PROJECT_SOURCE_DIR}/libnestutil
    ${PROJECT_BINARY_DIR}/libnestutil
    ${PROJECT_SOURCE_DIR}/sli

  # << PUBLIC headers live in multi_network/include >>
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)



    target_link_libraries(multi_network
    PRIVATE nestutil)


    install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
      DESTINATION include
      FILES_MATCHING PATTERN "*.h"
)
