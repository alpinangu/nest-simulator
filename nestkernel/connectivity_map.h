/*
 *  connectivity_map.h
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

#ifndef CONNECTIVITY_MAP_HH
#define CONNECTIVITY_MAP_HH


#include <vector>
#include <map>
#include <string>
#include <ostream>

namespace nest
{

class ConnectorInfo
{
public:

    enum CommunicationType
    {
    COLLECTIVE, POINTTOPOINT
    };


    enum ProcessingMethod
    {
    TREE, TABLE
    };

private:
    static int maxPortCode_;
    std::string recApp_;
    std::string recPort_;
    int recPortCode_;
    int remoteLeader_;
    //a: number of Proc of the remote procces
    //Number of proccesses of the remote application
    int remoteNProc_;
    int commType_;
    int procMethod_;

public:
    ConnectorInfo ()
    {
    }


    ConnectorInfo (std::string recApp, std::string recPort, int recPortCode,
        int remoteLeader, int remoteNProc, int commType, int procMethod) :
        recApp_ (recApp), recPort_ (recPort), recPortCode_ (recPortCode), remoteLeader_ (
            remoteLeader), remoteNProc_ (remoteNProc), commType_ (commType), procMethod_ (
            procMethod)
    {
    }

    std::string
    receiverAppName () const
    {
      return recApp_;
    }


    std::string
    receiverPortName () const
    {
      return recPort_;
    }


    int
    receiverPortCode () const
    {
      return recPortCode_;
    }


    int
    remoteLeader () const
    {
      return remoteLeader_;
    }

    void
    setRemoteLeader (int leader)
    {
      remoteLeader_ = leader;
    }

    int
    nRemoteProcesses () const
    {
      return remoteNProc_;
    }


    int
    communicationType () const
    {
      return commType_;
    }


    int
    processingMethod () const
    {
      return procMethod_;
    }

    static void
    registerPortCode (int portCode)
    {
      maxPortCode_ = std::max (portCode, maxPortCode_);
    }
};

typedef std::vector<ConnectorInfo> PortConnectorInfo;
//a: representing all the connectivity details for one port (its name, direction, width, and the connections attached to it
class ConnectivityInfo
{

public:
    enum PortDirection
    {
    OUTPUT, INPUT
    };

    static const int NO_WIDTH = -1;

private:
    std::string portName_;
    PortDirection dir_;
    int width_;
    std::vector<ConnectorInfo> portConnections_;

public:
    ConnectivityInfo (std::string portName, PortDirection dir, int width) :
          portName_ (portName), dir_ (dir), width_ (width)
    {
    }

    void  addConnection (std::string recApp, std::string recName, int recCode,
        int rLeader, int nProc, int commType, int procMethod);

    PortDirection
    direction ()
    {
      return dir_;
    }

    int
    width ()
    {
      return width_;
    } // NO_WIDTH if no width specified

    PortConnectorInfo&
    connections ()
    {
      return portConnections_;
    }

    std::string
    portName ()
    {
      return portName_;
    }
};


//a: Connectivity – a higher-level container that aggregates connectivity information for multiple ports
//This class is spesific to each app and populated differently
class Connectivity
{
  // connectivityMap_ seems obsolote, create a map between the names and connectivityInfo directy.
    //A vector containing all connections of the app
    //std::vector<ConnectivityInfo> connections_;
    //Maps the name of the app's port(receiver or sender) to the index within the connections_ vector
    //std::map<std::string, int> connectivityMap_;

    std::map<std::string, ConnectivityInfo> connectionMap_;

public:
    Connectivity ()
    {
    }

    static constexpr ConnectivityInfo* NO_CONNECTIVITY = nullptr;

    void  add (std::string localPort, ConnectivityInfo::PortDirection dir, int width,
        std::string recApp, std::string recPort, int recPortCode,
        int remoteLeader, int remoteNProc, int commType, int procMethod);

    void write(std::ostream& out);

    void read (std::istringstream& in);

    ConnectivityInfo* info (std::string portName);

    
};

}


#endif /* CONNECTIVITY_MAP_HH */
