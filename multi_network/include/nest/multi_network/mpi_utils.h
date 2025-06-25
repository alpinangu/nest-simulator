/*
 *  mpi_utils.h
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef MPI_UTILS_H
#define MPI_UTILS_H
#include "nest/multi_network/config.h"

#include <mpi.h>

namespace nest
{
  bool mpi_is_initialized ();
  int mpi_get_rank (MPI_Comm comm);
  int mpi_get_comm_size (MPI_Comm comm);
  int mpi_get_group_size (MPI_Group group);
  int mpi_get_type_size (MPI_Datatype type);
  MPI_Group mpi_get_group (MPI_Comm comm);
} // namespace

//#endif /* HAVE_MPI */
#endif /* MPI_UTILS_H */