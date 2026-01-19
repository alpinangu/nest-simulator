/*
 *  error.cpp
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

//#include "nest/multi_network/config.h"

#include <mpi.h>

//#include "nest/multi_network/mpi_utils.h"
#include <multi_network_common/error.h>

#include <iostream>
#include <stdlib.h>

namespace nest {

  void
  error ()
  {
    MPI_Abort (MPI_COMM_WORLD, 1);
    abort();

  }


  void
  hang ()
  {
    while (true)
      ;
  }
  
  
  void
  error (std::string msg)
  {
    std::cerr << "Error in MUSIC library: " << msg << std::endl;
    error ();
  }


  void
  error (std::ostringstream& ostr)
  {
    error (ostr.str ());
  }


  static int
  getRank ()
  {
    int isInitialized;
    MPI_Initialized (&isInitialized);
    if (isInitialized)
    {
      int rank;
      MPI_Comm_rank (MPI_COMM_WORLD, &rank);
      return rank;
    }
    return -1;
  }

  
  void
  error0 (std::string msg)
  {
    if (getRank () <= 0)
      error (msg);
    else
      // Give process #0 a chance to report the error
      hang ();
  }


  void
  errorRank (std::string msg)
  {
    std::ostringstream text;
    int rank = getRank ();
    if (rank >= 0)
      text << "rank #" << rank << ": ";
    text << msg;
    error (text.str ());
  }
  
  
  void
  checkOnce (bool& flag, std::string msg)
  {
    if (flag)
      errorRank (msg);
    flag = true;
  }

  //a: checks if the an Object is Instantiated once, combined with the obove 2 functions
  void
  checkInstantiatedOnce (bool& isInstantiated, std::string className)
  {
    std::ostringstream msg;
    msg << className << " constructor was called a second time.\n"
      "Only one " << className << " object can exist at any instance of time.";
    checkOnce (isInstantiated, msg.str ());
  }

  
  void
  checkCalledOnce (bool& isCalled, std::string funcName, std::string suffix)
  {
    std::ostringstream msg;
    msg << funcName << " called twice" << suffix;
    checkOnce (isCalled, msg.str ());
  }
  
}
