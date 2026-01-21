/*
 *  This file is part of MUSIC.
 *  Copyright (C) 2007, 2008, 2009, 2021, 2022 INCF
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

#ifndef MN_SETUP_H
#define MN_SETUP_H

//#include "music/music-config.hh"

#include <mpi.h>

#include <string>
#include <vector>

#include <nest/multi_network/port.h>

#include <nest/multi_network/index_map.h>
#include <nest/multi_network/linear_index.h>
#include <nest/multi_network/cont_data.h>
#include <nest/multi_network/connector.h>
#include <nest/multi_network/temporal.h>
#include <nest/multi_network/configuration.h>


using std::string;

#define MUSIC_DEFAULT_TIMEBASE 1e-9

namespace nest_mn {

  class Runtime;

  /*
   * This is the Setup object in the MUSIC API
   *
   * It is documented in section 4.3 of the MUSIC manual
   */
  
  class Setup {
    static const char* const configEnvVarName;
    static const char* const opConfigFileName;
    static const char* const opAppLabel;
  public:
    Setup (int& argc, char**& argv);

    Setup (int& argc, char**& argv, int required, int* provided);

    ~Setup ();

    MPI_Comm communicator ();

    bool config (string var, string* result);

    bool config (string var, int* result);

    bool config (string var, double* result);

    bool config (string var, bool* result);

    ContInputPort* publishContInput (string identifier);

    ContOutputPort* publishContOutput (string identifier);

    EventInputPort* publishEventInput (string identifier);

    EventOutputPort* publishEventOutput (string identifier);

    MessageInputPort* publishMessageInput (string identifier);

    MessageOutputPort* publishMessageOutput (string identifier);

  private:
    MPI_Comm comm;
    Configuration* config_;
    std::vector<Port*> ports_;
    std::vector<Connection*>* connections_;
    TemporalNegotiator* temporalNegotiator_;
    double timebase_;
    static bool isInstantiated_;
    //a: There two are inisilised in fullInit, they simulate command-line arguments parsed from the Config file.
    int& argc_;
    char**& argv_;

    bool launchedByMusic_;
    bool postponeSetup_;
    int color_;

    // Since we don't want to expose this internal interface to the
    // user we put the member functions in the private part and give
    // these classes access through a friend declaration.  Classes are
    // still expected not to access the internal data members
    // directly.  A cleaner solution would be to split this class into
    // one user API part and one internal interface part.
    friend class Runtime;
    friend class Port;
    friend class OutputPort;
    friend class InputPort;
    friend class TemporalNegotiator;
    friend class ApplicationNode;
    
    double timebase () { return timebase_; }

    bool launchedByMusic ();

    void init (int& argc, char**& argv);

    void maybeProcessMusicArgv (int& argc, char**& argv);

    void maybePostponedSetup ();

    void fullInit ();

    nest_mn::ConnectivityInfo* portConnectivity (const std::string localName);

    nest_mn::ApplicationMap* applicationMap ();

    int applicationColor();

    std::string applicationName();

    int leader ();

    int nProcs ();

    nest_mn::ConnectivityInfo::PortDirection
    portDirection (const std::string localName);

    int portWidth (const std::string localName);

    nest_mn::PortConnectorInfo portConnections (const std::string localName);

    std::vector<Port*>* ports ()
    {
      return &ports_;
    }    

    void addPort (Port* p);
    
    std::vector<Connection*>* connections ()
    {
      return connections_;
    }
    
    void addConnection (Connection* c);

    TemporalNegotiator* temporalNegotiator () { return temporalNegotiator_; }
    
    void errorChecks ();

    bool launchedWithExec (std::string &configStr);

    bool launchedMPMD (int argc, char** argv, std::string& config);

    void loadConfigFile (std::string filename, std::string &result);

    bool getOption (int argc, char** argv, std::string option, std::string& result);

  };
  
}
#endif

