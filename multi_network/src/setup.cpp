/*
 *  setup.cpp
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

#include "nest/multi_network/setup.h"
#include "setup_internal.h"

// C++ includes:
#include <string>


// Includes from libnestutil:

// Includes from nestkernel:




namespace nest
{

bool Setup::isInstantiated_ = false;

Setup::Setup (int& argc, char**& argv, int required, int* provided)
: argc_ (argc), argv_ (argv), setupInternal_{std::make_unique<SetupInternal>(*this, argc, argv)}
{
    int MPIInitialized;
    MPI_Initialized (&MPIInitialized);

    if (MPIInitialized || isInstantiated_)
        // TODO: error and terminate
        exit(111);
    isInstantiated_ = true;
    MPI_Init_thread (&argc, &argv, required, provided);
    //a: this might not have mpi spesific arguments
    setupInternal_->init (argc, argv);
}

Setup::~Setup() = default;

MPI_Comm
Setup::communicator ()
{
return comm;
}


bool
Setup::config (std::string var, std::string* result)
{
    return setupInternal_->config(var, result);
}

bool
Setup::config (std::string var, int* result)
{
    return setupInternal_->config(var, result);
}

  bool
  Setup::config (std::string var, double* result)
  {
    return setupInternal_->config(var, result);
  }

  bool
  Setup::config (std::string var, bool* result)
  {
    return setupInternal_->config(var, result);
  }

} // namespace nest
