# FindMusic.cmake
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

# - Find MUSIC header and library
#
# This module defines
#  MUSIC_FOUND, if false, do not try to use MUSIC.
#  MUSIC_INCLUDE_DIRS, where to find music.hh.
#  MUSIC_LIBRARIES, the libraries to link against to use MUSIC.
#  MUSIC_EXECUTABLE, the music executable.
#  MUSIC_VERSION, the library version
#
# As a hint allows MUSIC_ROOT_DIR.

find_path( MUSIC_INCLUDE_DIR
    NAMES nest/multi_network.h
    HINTS ${Music_ROOT}/include/multi_network
    )
find_library( MUSIC_LIBRARY
    NAMES multi_network
    HINTS ${Music_ROOT}/lib/nest
    )

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Music
  FOUND_VAR
    MUSIC_FOUND
  REQUIRED_VARS
    MUSIC_LIBRARY
    MUSIC_INCLUDE_DIR
    )

if ( MUSIC_FOUND )
  set( MUSIC_LIBRARIES "${MUSIC_LIBRARY}" )
  set( MUSIC_INCLUDE_DIRS "${MUSIC_INCLUDE_DIR}" )
endif ()

mark_as_advanced( MUSIC_ROOT_DIR MUSIC_INCLUDE_DIR MUSIC_LIBRARY )
