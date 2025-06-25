/*
 *  setup_internal.cpp
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

#include "setup_internal.h"
#include "nest/multi_network/parse.h"

// C++ includes:
#include <string>
#include <sstream>

//multi_network
#include "ioutils.h"

// Includes from libnestutil:

// Includes from nestkernel:
#include "multi_network_manager.h"



namespace nest
{

SetupInternal::SetupInternal(Setup& setup)
 : setup_(setup)
{}

void
SetupInternal::init (int& argc, char**& argv)
{
//a: sets the variables that will be used throught the function
//a: MPI_COMM_WORLD is the chat room of all MPI processes

int rank;
MPI_Comm_rank (MPI_COMM_WORLD, &rank);
std::string config = "";
launchedByMusic_ = false;
postponeSetup_ = false;

//a: config here represents the information we passed to the environment variable
//a: application and configuration map
if (launchedWithExec (config))
{
    assert(config.length() > 0);
    
    launchedByMusic_ = true;
    
    if (!config.compare (0, 8, "POSTPONE"))
    {
        postponeSetup_ = true;
        // *fixme* Error checking
        //a: create a string stream
        std::istringstream in (config);
        //a: consumes POSTPONE
        IOUtils::read (in); // POSTPONE
        //a: skip the delim
        in.ignore (); // delim
        //a: save the colour from the next part of the env. variable
        std::string colorString = IOUtils::read (in);
        color_ = atoi (colorString.c_str ());
    }
    else
        manager_ = new MultiNetworkManager(config); // destroyed by runtime

}
//a: here, we override the argc and argv using the env. var.
//a: NEST does NOT use this
//TODO: other launch options
else
{
    std::cout << "Launch method not known." << std::endl;
}

connections_ = new std::vector<Connection*>; // destroyed by runtime
if (launchedByMusic_)
    {
    // launched by the music utility
    if (!postponeSetup_)
        {
        fullInit();
        argc = setup_.argc_;
        argv = setup_.argv_;
        }
    MPI_Comm_split (MPI_COMM_WORLD, postponeSetup_ ? color_ : manager_->Color (), rank, &setup_.comm);
    }
else
    {
    // launched with mpirun
    setup_.comm = MPI_COMM_WORLD;
    timebase_ = MUSIC_DEFAULT_TIMEBASE;
    }

}

void
SetupInternal::fullInit ()
{
    errorChecks();
    // a:timebase is what this application wants to use as the smallest time unit.
    if (!manager_->lookup("timebase", &timebase_))
        timebase_ = MUSIC_DEFAULT_TIMEBASE;	       // default timebase
    std::string binary;
    // a: binary is the path or the name of the executalbe
    manager_->lookup ("binary", &binary);
    std::string args;
    // a:args are the command-line arguments specific to that binary/application.
    manager_->lookup ("args", &args);
    /*
    a:  Example:
            binary=./send.py
            args= --option -o somefile
    */

    setup_.argv_ = parseArgs (binary, args, &setup_.argc_);
    // a:but we need a shared clock for communication
    temporalNegotiator_ = new TemporalNegotiator (this);
}
  
bool
SetupInternal::launchedWithExec (std::string &result)
{
// is _MUSIC_CONFIG_ env variable is set ?
//a: We set it during launch
char* res = getenv (MultiNetworkManager::configEnvVarName);
if (res != NULL)
    {
    result.assign (res);
    return true;
    }
else
    return false;
}

void
SetupInternal::errorChecks ()
{
    ApplicationMap* apps = manager_->applications();
    int nRequestedProc = apps->nProcesses ();
    int nMPIProc = mpi_get_comm_size (MPI_COMM_WORLD);
    if (nMPIProc != nRequestedProc)
        {
    std::ostringstream msg;
    msg << "configuration file specifies " << nRequestedProc
        << " MPI processes but MUSIC was given " << nMPIProc
        << std::endl;
    error0 (msg.str ());
        }
}

bool
SetupInternal::config (std::string var, std::string* result)
{
    return manager_->lookup(var, result);
}

bool
SetupInternal::config (std::string var, int* result)
{
    return manager_->lookup(var, result);
}

bool
SetupInternal::config (std::string var, double* result)
{
    return manager_->lookup (var, result);
}

bool
SetupInternal::config (std::string var, bool* result)
{
    return manager_->lookup(var, result);
}



ApplicationMap*
SetupInternal::applicationMap ()
{
 return manager_->applications ();
}

int
SetupInternal::applicationColor ()
{
    return manager_->Color();
}

MPI_Comm
SetupInternal::communicator ()
{
    return setup_.comm;
}

int
SetupInternal::leader ()
{
return manager_->Leader ();
}


int
SetupInternal::nProcs ()
{
return mpi_get_comm_size (setup_.comm);
}

void
SetupInternal::maybePostponedSetup ()
{
if (postponeSetup_)
    {
postponeSetup_ = false;
std::string config = "";
launchedWithExec (config);
manager_ = new MultiNetworkManager (config);
fullInit ();
    }
}


} // namespace nest
