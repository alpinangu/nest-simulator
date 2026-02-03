/*
 *  This file is part of MUSIC.
 *  Copyright (C) 2007, 2008, 2009, 2022 INCF
 *
 *  MUSIC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MUSIC is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <nest/multi_network/setup.h>

#include <nest/multi_network/mpi_utils.h>
#include <nest/multi_network/runtime.h>
#include <nest/multi_network/parse.h>
#include <multi_network_common/error.h>
//#include "nest/multi_network/application_mapper.h"
#include <multi_network_common/ioutils.h>
#include <strings.h>
#include <fstream>

namespace nest_mn {

  //a: This is a static bool that is retained between every objects,
  //a: so we can check if it the constructor is called more than once
  //a: the others are also static
  bool Setup::isInstantiated_ = false;
  static std::string err_MPI_Init = "MPI_Init was called before the Setup constructor";
  //a: --music-config command-line option is used to specify the path to the MUSIC configuration file
  const char* const Setup::opConfigFileName = "--music-config";
  const char* const Setup::opAppLabel = "--app-label";

  //a: NEST DOES NOT USE THIS. SEE BELOW
  Setup::Setup (int& argc, char**& argv)
    : argc_ (argc), argv_ (argv)
  {
    checkInstantiatedOnce (isInstantiated_, "Setup");
    if (mpi_is_initialized ())
      errorRank (err_MPI_Init);
    maybeProcessMusicArgv (argc, argv);
    MPI_Init(&argc, &argv);

    init(argc, argv);

  }

  //a: THIS IS THE FIRST FUNCTION THAT SIMULATORS CALL
  //a: it first checks if any other objects were initilised before
  //a: then it checks if MPI is already initilised
  //a: if so => prints an error
  //a: overrides the argc and argv IF the environment variable is set, but it is not with NEST
  //a: Initilises MPI
  //a: 
  Setup::Setup (int& argc, char**& argv, int required, int* provided)
    : argc_ (argc), argv_ (argv)
  {
    //a: ensures that Setup function is only called once and MPI is not already initilised
    checkInstantiatedOnce (isInstantiated_, "Setup");
    if (mpi_is_initialized ())
      errorRank (err_MPI_Init);
    //a: NEST does not use this. you can set an env variable and use it as argv argc
    maybeProcessMusicArgv (argc, argv);
    //a: NEST uses MPI_THREAD_FUNNELED as required
    //a: we pass the argc and argv (which is the original ones we gave to music) 
    //a:so that MPI get neccesery information mpirun -n and stuff
    //a: then it removes its neccesery arguments so we can easyly use the rest
    MPI_Init_thread (&argc, &argv, required, provided);
    //a: this might not have mpi spesific arguments
    init (argc, argv);
  }


  Setup::~Setup ()
    {
      for (std::vector<Port*>::iterator i = ports_.begin ();
           i != ports_.end ();
           ++i)
        (*i)->setupCleanup ();

      if (launchedByMusic ())
        delete temporalNegotiator_;

      // delete connection objects
      for (std::vector<Connection*>::iterator i = connections_->begin ();
           i != connections_->end ();
           ++i)
        delete *i;

      delete connections_;

      delete config_;

      isInstantiated_ = false;
    }

  //a: _MUSIC_CONFIG_ contains the application and the connectivity map, we set it up during launch
  //a: There are 2 types of initilasation. launchedWithExec is the regular launch process in main.cpp
  //a: Launched with Exec — Environment Variable Method
  //a:    - Uses the environment variable: _MUSIC_CONFIG_ that we set dunring launch
  //a:  Launched with MPMD — Command-Line, Multi-Program
  //a:    - You launch all MUSIC-participating applications at once using mpirun or mpiexec
  void
  Setup::init (int& argc, char**& argv)
  {
    //a: sets the variables that will be used throught the function
    //a: MPI_COMM_WORLD is the chat room of all MPI processes
    int myRank = mpi_get_rank(MPI_COMM_WORLD);
    std::string config = "";
    launchedByMusic_ = false;
    postponeSetup_ = false;

    //a: config here represents the information we passed to the environment variable
    //a: application and configuration map, we use this
    if (launchedWithExec(config))
    {
      assert(config.length() > 0);
      //a: Music itself launched the binaries
      launchedByMusic_ = true;
      //a: checking if config starts with the substring "POSTPONE".
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
        config_ = new Configuration(config);
    }
    //a: here, we override the argc and argv using the env. var.
    //a: NEST does NOT use this
    /*
    else if (launchedMPMD (argc, argv, config))
    {
      launchedByMusic_ = true;
      //a: this config_file string holds the config file
      std::string config_file;
      loadConfigFile (config, config_file);

      std::string app_label;
      std::string binary (argv[0]);
      // argv[0] is the name of the program,
      // or an empty string if the name is not available

      //a: here we retrieve the app label from the command line.
      //a: I think it is used to find infromation from the config file, so the section names must match
      if (!getOption (argc, argv, opAppLabel, app_label) && binary.length () == 0)
      {
        std::ostringstream oss;
        oss << "MUSIC: use --app-label to specify application label";
        error0 (oss.str ());
      }

      std::istringstream config_istream (config_file);
      config_ = new Configuration ();
      ApplicationMapper app_mapper(config_);
      app_mapper.map(&config_istream, binary, app_label);
    }
    */
    else
      config_ = new Configuration ();


    connections_ = new std::vector<Connection*>; // destroyed by runtime
    if (launchedByMusic())
    {
      // launched by the music utility
      if (!postponeSetup_)
      {
        fullInit ();
        argc = argc_;
        argv = argv_;
      }
      MPI_Comm_split(MPI_COMM_WORLD, postponeSetup_ ? color_ : config_->Color (), myRank, &comm);
      }
    else
    {
      // launched with mpirun
      comm = MPI_COMM_WORLD;
      timebase_ = MUSIC_DEFAULT_TIMEBASE;
    }
  }

  //a: It searches the command‑line arguments (the argv array) for a specific option
  //a: If it finds that option, it saves the next argument inside result as that option’s value and returns true
  bool
  Setup::getOption (int argc, char** argv, std::string option, std::string& result)
  {
    //a: first clears the result string
    result.assign("");
    for (int i = 1; i < argc; ++i)
       //a: The condition argc > i ensures there's at least one more argument after the option 
       //a: so that the function can safely access argv[i+1]
       if (option.compare(argv[i]) == 0 && argc > i){
           //a: If the option is found, the function assigns the next argumen 
           result.assign(argv[i+1]);  // skip options
             return true;
       }
     return false;
  }

  //a: eturns true, if the environment variable is set. 
  //a: it also overrides result as the value of the environment variable
  bool
  Setup::launchedWithExec (std::string &result)
  {
    // is _MUSIC_CONFIG_ env variable is set ?
    //a: We set it during launch
    char* res = getenv(Configuration::configEnvVarName);
    if (res != NULL)
    {
      result.assign(res);
      return true;
    }
    else
      return false;
  }

  //a: if given option --music-config, the launch is categorized as MPMD
  //a: it also assings the config file path to config
  bool
  Setup::launchedMPMD (int argc, char** argv, std::string& config)
  {
    // if given option --music-config,
    // the launch is categorized as MPMD
    if (!getOption(argc, argv, opConfigFileName, config) )
      return false;
    else
      return true;
  }

  //a: rank 0 reads the configuration file, saves it to result, and then broadcasts its content to all other MPI processes.
  void
  Setup::loadConfigFile (std::string filename, std::string &result)
  {
    //a: open a file (for reading), using the filename variable — only on rank 0
    std::ifstream config;
    char* buffer;
    int size = 0;
    int myRank = mpi_get_rank (MPI_COMM_WORLD);
    // Rank #0 is reading a file and broadcast it to each rank in the launch
    if (myRank == 0)
      {
        config.open (filename.c_str ());
        if (!config.is_open ())
          {
            std::ostringstream oss;
            oss << "MUSIC: Couldn't open configuration file: " << filename << '\n';
            error0 (oss.str ());
          }
        //a: calculates the size of the config file in bytes
        size = config.tellg ();
        config.seekg (0, std::ios_base::end);
        long cur_pos = config.tellg ();
        size = cur_pos - size;
        config.seekg (0, std::ios_base::beg);
      }
    // first broadcast the size of the file
    //a: Process 0 will send the value of size (an int) to all other processes in the group.
    MPI_Bcast (&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    buffer = new char[size];
    //a: rank 0 reads the config file into a buffer
    if (myRank == 0)
      config.read (buffer, size);
    // then broadcast the file but itself
    //a: that comment does not make any sense. it should be this:
    //a: Rank 0 broadcasts the file content (not the file itself) to all other ranks
    MPI_Bcast (buffer, size, MPI_BYTE, 0, MPI_COMM_WORLD);
    // parseMapFile (app_name, std::string (buffer, size), result);
    if (myRank == 0)
      config.close ();
    //a: we save the config file content into result
    result.assign (buffer);
    delete[] buffer;
  }

  //a: extracts the cmd and arguments from the env variable and
  //a: builds an artificial argv array from the input strings and updates *argc to the number of arguments parsed using parsArgs
  void
  Setup::maybeProcessMusicArgv (int& argc, char**& argv)
  {
    //a: gets the value of the env. variable (a string)
    //a: this is not set by NEST nor MUSIC, thats why the name maybe
    char* MUSIC_ARGV = getenv ("MUSIC_ARGV");
    if (MUSIC_ARGV != NULL)
    {
	    std::string cmd;
	    std::string argstring;
      //a: This is a pointer to the first space in the argv
	    char* s = index(MUSIC_ARGV, ' ');
      //a: if there is no space, then the whole argv is the command
	    if (s == NULL)
	    {
	      cmd = std::string (MUSIC_ARGV);
	      argstring = "";
	    }
	    else
	    {
        //a: This extracts just the command (before the space)
	      cmd = std::string (MUSIC_ARGV, s - MUSIC_ARGV);
        //a: Gets the argument string (after the white space)
	      argstring = std::string (s + 1);
	    }
	    argv = parseArgs(cmd, argstring, &argc);
    }
  }


  void
  Setup::maybePostponedSetup ()
  {
    if (postponeSetup_)
      {
	postponeSetup_ = false;
	std::string config = "";
	launchedWithExec (config);
	config_ = new Configuration (config);
	fullInit ();
      }
  }


  void
  Setup::errorChecks ()
  {
    nest_mn::ApplicationMap* apps = applicationMap ();
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


  void
  Setup::fullInit ()
  {
    errorChecks();
    // a:timebase is what this application wants to use as the smallest time unit.
    if (!config("timebase", &timebase_))
      timebase_ = MUSIC_DEFAULT_TIMEBASE;	       // default timebase
    string binary;
    // a: binary is the path or the name of the executalbe
    config_->lookup("binary", &binary);
    string args;
    // a:args are the command-line arguments specific to that binary/application.
    config_->lookup("args", &args);
    /*
    a:  Example:
          binary=./send.py
          args= --option -o somefile
    */

    argv_ = parseArgs(binary, args, &argc_);
    // a:but we need a shared clock for communication
    temporalNegotiator_ = new TemporalNegotiator (this);
  }
  

  bool
  Setup::launchedByMusic ()
  {
    return launchedByMusic_;
  }

  
  MPI_Comm
  Setup::communicator ()
  {
    return comm;
  }


  nest_mn::ConnectivityInfo*
  Setup::portConnectivity (const std::string localName)
  {
    return config_->connectivityMap()->info(localName);
  }


  nest_mn::ApplicationMap*
  Setup::applicationMap ()
  {
    return config_->applications ();
  }


  int
  Setup::applicationColor ()
  {
	  return config_->Color();
  }


  std::string
  Setup::applicationName()
  {
    return config_->Name();
  }


  int
  Setup::leader ()
  {
    return config_->Leader ();
  }


  int
  Setup::nProcs ()
  {
    return mpi_get_comm_size (comm);
  }


  nest_mn::ConnectivityInfo::PortDirection
  Setup::portDirection (const std::string localName)
  {
    return config_->connectivityMap()->direction(localName);
  }


  int
  Setup::portWidth (const std::string localName)
  {
    return config_->connectivityMap ()->width(localName);
  }


  nest_mn::PortConnectorInfo
  Setup::portConnections (const std::string localName)
  {
    return config_->connectivityMap ()->connections(localName);
  }


  bool
  Setup::config (string var, string* result)
  {
    return config_->lookup (var, result);
  }

  
  bool
  Setup::config (string var, int* result)
  {
    return config_->lookup (var, result);
  }

  
  bool
  Setup::config (string var, double* result)
  {
    return config_->lookup (var, result);
  }


  bool
  Setup::config (string var, bool* result)
  {
    return config_->lookup (var, result);
  }

  
  ContInputPort*
  Setup::publishContInput (std::string identifier)
  {
    return new ContInputPort (this, identifier);
  }


  ContOutputPort*
  Setup::publishContOutput (std::string identifier)
  {
    return new ContOutputPort (this, identifier);
  }


  EventInputPort*
  Setup::publishEventInput (std::string identifier)
  {
    return new EventInputPort (this, identifier);
  }


  EventOutputPort*
  Setup::publishEventOutput (std::string identifier)
  {
    return new EventOutputPort (this, identifier);
  }

  
  MessageInputPort*
  Setup::publishMessageInput (std::string identifier)
  {
    return new MessageInputPort (this, identifier);
  }


  MessageOutputPort*
  Setup::publishMessageOutput (std::string identifier)
  {
    return new MessageOutputPort (this, identifier);
  }

  
  void Setup::addPort (Port* p)
  {
    ports_.push_back (p);
  }

  
  void Setup::addConnection (Connection* c)
  {
    connections_->push_back (c);
  }

}
