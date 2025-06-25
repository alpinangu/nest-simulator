/*
 *  setup.h
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

#ifndef SETUP_H
#define SETUP_H

//#ifdef HAVE_MPI

// C++ includes:
#include <mpi.h>
#include <string>
#include <vector>

// Includes from libnestutil:



// Includes from nestkernel:



#include "nest/multi_network/config.h"

#define MUSIC_DEFAULT_TIMEBASE 1e-9
namespace nest
{

class SetupInternal;

class Setup
{
public:
  Setup(int& argc, char**& argv, int required, int* provided);
  ~Setup();

  MPI_Comm communicator();

  bool config (std::string var, std::string* result);
  bool config (std::string var, int* result);
  bool config (std::string var, double* result);
  bool config (std::string var, bool* result);

  //ContInputPort* publishContInput (std::string identifier);

  
private:
  int& argc_;
  char**& argv_;

  MPI_Comm comm;

  static bool isInstantiated_;

  friend class SetupInternal; //We need accsess to these private variables from the internal class
  std::unique_ptr<SetupInternal> setupInternal_;
};


} // namespace

//#endif /* HAVE_MPI */
#endif /* SETUP_H */