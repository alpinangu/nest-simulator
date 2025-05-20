/*
 *  multi_network_manager.h
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

#include "connectivity_map.h"
#include <sstream>
#include <iostream>
#include <ostream>

namespace nest
{
  
int ConnectorInfo::maxPortCode_;

//////////////////////ConnectivityInfo/////////////////////////////

void
ConnectivityInfo::addConnection (std::string recApp,
                std::string recPort,
                int recPortCode,
                int remoteLeader,
                int remoteNProc,
                int commType,
                int procMethod
                )
{
    portConnections_.push_back (ConnectorInfo (recApp,
                        recPort,
                        recPortCode,
                        remoteLeader,
                        remoteNProc,
                        commType,
                        procMethod));
}

//////////////////////Connectivity/////////////////////////////

void
Connectivity::add (std::string localPort,
                ConnectivityInfo::PortDirection dir,
                int width,
                std::string recApp,
                std::string recPort,
                int recPortCode, //each receiver port has a unique code.
                int remoteLeader,
                int remoteNProc,
                int commType,
                int procMethod)
{
    std::map<std::string, int>::iterator cmapInfo = connectivityMap_.find (localPort);
    ConnectivityInfo* info;

    if (cmapInfo == connectivityMap_.end ())
    {
	    int index = connections_.size ();
	    connections_.push_back (ConnectivityInfo (localPort, dir, width));
	    info = &connections_.back ();
	    connectivityMap_.insert (std::make_pair (localPort, index));
    }
    else
    {
	    info = &connections_[cmapInfo->second];
	    if (info->direction () != dir)
        {
            //THROW AN EXEPTION        
        }
    }
    info->addConnection (recApp,
			 recPort,
			 recPortCode,
			 remoteLeader,
			 remoteNProc,
			 commType,
			 procMethod);
}

void
Connectivity::write (std::ostream& out)
{   
    out << ':';
    out << connectivityMap_.size();
    std::map<std::string, int>::iterator i;
    for (i = connectivityMap_.begin (); i != connectivityMap_.end ();++i)
    {
        out << ':' << i->first << ':';
        ConnectivityInfo* ci = &connections_[i->second];
        out << ci->direction () << ':' << ci->width () << ':';
	    PortConnectorInfo conns = ci->connections ();
        out << conns.size ();
        PortConnectorInfo::iterator c;

        for (c = conns.begin (); c != conns.end (); ++c)
        {
            out << ':' << c->receiverAppName();
            out << ':' << c->receiverPortName();
            out << ':' << c->receiverPortCode();
            out << ':' << c->remoteLeader();
            out << ':' << c->nRemoteProcesses();
            out << ':' << c->communicationType();
            out << ':' << c->processingMethod();
        }
    }
}

} // namespace nest