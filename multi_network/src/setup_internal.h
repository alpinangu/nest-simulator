/*
 *  setup_internal.h
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

#ifndef SETUP_INTERNAL_H
#define SETUP_INTERNAL_H

//#ifdef HAVE_MPI

// C++ includes:
#include <mpi.h>
#include <string>

// Includes from libnestutil:



// Includes from nestkernel:
#include "multi_network_manager.h"

// Includes from sli:


#include "nest/multi_network/setup.h"

//#include <nest/multi_network/port.h>

#include <nest/multi_network/index_map.h>
#include <nest/multi_network/linear_index.h>
#include <nest/multi_network/cont_data.h>
#include <nest/multi_network/connector.h>
#include <nest/multi_network/temporal.h>
//#include <nest/multi_network/configuration.h>

namespace nest
{


class SetupInternal
{
public:
  SetupInternal(Setup& owner);
  ~SetupInternal(){}

  void init (int& argc, char**& argv);

  ApplicationMap* applicationMap();

  int applicationColor();

  double timebase () { return timebase_; }

  MPI_Comm communicator ();

  int leader ();

  int nProcs();


  bool config (std::string var, std::string* result);
  bool config (std::string var, int* result);
  bool config (std::string var, double* result);
  bool config (std::string var, bool* result);

  void maybePostponedSetup();

private:

  bool launchedByMusic_;
  bool postponeSetup_;
  int color_;

  MultiNetworkManager* manager_;
  Setup& setup_; 
  double timebase_;
  TemporalNegotiator* temporalNegotiator_;
  std::vector<Connection*>* connections_;


  void fullInit();

  bool launchedWithExec(std::string &result);

  
  void errorChecks();

};

} // namespace

//#endif /* HAVE_MPI */
#endif /* SETUP_H */