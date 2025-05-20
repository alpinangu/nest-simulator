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

#include "multi_network_manager.h"

// C++ includes:
#include <sstream>
#include <algorithm>

// Includes from libnestutil:

// Includes from nestkernel:
#include "connectivity_map.h"
#include "token.h" 




namespace nest
{

bool MultiNetworkManager::isApplicationMapSet_ = false;
bool MultiNetworkManager::isConnMapCalled_ = false;

MultiNetworkManager::MultiNetworkManager()
{

}
MultiNetworkManager::~MultiNetworkManager()
{
    
}

void
MultiNetworkManager::initialize( const bool )
{

}
void
MultiNetworkManager::finalize( const bool )
{

}
void
MultiNetworkManager::set_status( const DictionaryDatum& )
{

}
void
MultiNetworkManager::get_status( DictionaryDatum& )
{

}

void
MultiNetworkManager::launch(const std::string exe, const long n)
{
    std::cout << " dont use this" << std::endl;
}

/*
DictionaryDatum is just a shared_ptr to the class Dictionary.
A Dictionary stores the SLI key-value pairs
*/

/*dict must be of this form
dict = {
    "global_var": "value"
    .
    .

    "app1_name": {
        "binary": "./app",
        "np": 1,
        "args": "arg1 arg1" SHOULD THERE BE A DELIMETER??
        "local_var": "value"
    },
    .
    .
}
*/
void 
MultiNetworkManager::set_application_map(const DictionaryDatum& dict)
{

    std::cout << "you are in the func" << std::endl;

    for (auto it = dict->begin(); it != dict->end(); ++it)
    {
        
        //First is the key, second is the value
        //First is always the application name
        //Second is always a dictionary containing configs of that app
        //we will cast it to a dict datum to itterate over it
        Token& subtoken = it->second;

        if (subtoken.is_a<DictionaryDatum>())
        {
            std::string app_name  = it->first.toString();

            DictionaryDatum subdict = getValue<DictionaryDatum>(subtoken);

            int np;
            std::map<std::string, std::variant<int, std::string, double>> app_dict;
            for (auto  ti = subdict->begin(); ti != subdict->end(); ++ti)
            {
                std::string key  = ti->first.toString();
                Token& value = ti->second;

                // here I might use #include "name.h", it says it is faster?

                if(key == "np")
                    np = getValue<long>(value);
                else
                    app_dict[key] = getValue<std::string>(value);

            }

            //??????
            //should we add binary to the map to or assign color to binaries in a different map?
            std::cout << "Application " << app_name << " with " << np << " has been added " << std::endl;
            application_map_.add(app_name, np, 0, app_dict);
        }
        else
        {
            std::string var_name  = it->first.toString();
            Token& subtoken = it->second;

            
            if (subtoken.is_a<std::string>())
            {
                std::string var_value = getValue<std::string>(subtoken);
                application_map_.add_global_dict(var_name, var_value);
            }

            else
            {
                int var_value = getValue<long>(subtoken);
                application_map_.add_global_dict(var_name, var_value);
            }
        }

    }

    isApplicationMapSet_ = true;
    if(isConnMapCalled_ == true)
    {
        set_connectivity_map(ConnMapBackup_);
    }

    writeEnv(std::cout, 0);
    std::cout << "" << std::endl;
    writeEnv(std::cout, 1);
    std::cout << "" << std::endl;
    writeEnv(std::cout, 2);

}

/*dict must be of this form
dict = {
    "app1_name.port_name": {
        "receiver": "app_name.port_name",
        "width": n,
        "commtype": "point-to-point" or "collective"
        "procmethod": "tree" or "table"
    },

    .
    .
    .
}
*/
// !! FIRST THE APPLICATION MAP MUST BE SET !!
void 
MultiNetworkManager::set_connectivity_map(const DictionaryDatum& dict)
{   
    if(isApplicationMapSet_ == false)
    {
        ConnMapBackup_ = dict;
        isConnMapCalled_ = true;
        return;
    }

    for (std::vector<ApplicationInfo>::iterator app = application_map_.begin(); app != application_map_.end(); ++app)
    {
        connectivityMap_[app->name()];
    }

    // We need a map because one receiver can have multiple senders
    std::map<std::string, int> receiverPortCodes;
    int nextPortCode = 0;

    //Ittarates the apps, because we need a different map for each app
    for(std::map<std::string, Connectivity>::iterator app = connectivityMap_.begin(); app != connectivityMap_.end(); ++app)
    {

        //Itterates the connections
        for (auto it_out = dict->begin(); it_out != dict->end(); ++it_out)
        {
            std::string senderFullName  = it_out->first.toString();
            std::istringstream stream(senderFullName);
            std::string senderAppName;
            std::getline(stream,senderAppName, '.');
            std::string senderPortName;
            std::getline(stream, senderPortName);

            std::string receiverFullName;
            std::string receiverAppName;
            std::string receiverPortName;
            std::string commType;
            std::string procMethod;
            int width = -1;

            //we will cast it to a dict datum to itterate over it
            Token& subtoken = it_out->second;

            if (subtoken.is_a<DictionaryDatum>())
            {
                DictionaryDatum subdict = getValue<DictionaryDatum>(subtoken);

                //Ittarates the information of each connection
                for (auto  it_in = subdict->begin(); it_in != subdict->end(); ++it_in)
                {
                    std::string key  = it_in->first.toString();
                    Token& value = it_in->second;

                    // here I might use #include "name.h", it says it is faster?
                    if(key == "receiver")
                    {
                        receiverFullName = getValue<std::string>(value);
                        std::istringstream stream(receiverFullName);
                        std::getline(stream, receiverAppName, '.');
                        std::getline(stream, receiverPortName);
                    }
                    else if(key == "commType")
                        commType = getValue<std::string>(value);
                    else if(key == "procMethod")
                        procMethod = getValue<std::string>(value);
                    else if (key == "width")
                        width = getValue<long>(value);
                    else
                        std::cout << "not a valid config argument" << std::endl;
                }

                if(senderAppName == receiverAppName)
                {
                    std::cout << "An application can not connect to itself";
                    return; // ERROR HANDLER HERE
                }

                std::transform(commType.begin(), commType.end(), commType.begin(), ::tolower);
                if (!commType.empty() && commType != "collective" && commType != "point-to-point")
                {
                    std::cout << "UNVALID COMMTYPE" << std::endl;
                }

                std::transform (procMethod.begin (), procMethod.end (), procMethod.begin (), ::tolower);
                if (procMethod.length () > 0 && procMethod.compare ("table") && procMethod.compare ("tree"))
                {
                    std::cout << "UNVALID PROCMETHOD" << std::endl;
                }

                if(width == -1)
                {
                    width = ConnectivityInfo::NO_WIDTH;
                }

                // Generate a unique "port code" for each receiver port
                // name.  This will later be used during temporal negotiation

                //We populate the map before checking the app correspondance, because we need to register the port regardless
                //They must have the same ID across the apps
                std::map<std::string, int>::iterator pos = receiverPortCodes.find (receiverFullName);
                int portCode;
                if (pos == receiverPortCodes.end ())
                {
                    portCode = nextPortCode++;
                    receiverPortCodes.insert (std::make_pair (receiverFullName, portCode));
                }
                else
                    portCode = pos->second;
                
                ConnectivityInfo::PortDirection dir;
                ApplicationInfo* remoteInfo;
                if(app->first == senderAppName)
                {
                    dir = ConnectivityInfo::OUTPUT;
                    remoteInfo = application_map_.lookup (receiverAppName);
                }
                else if(app->first == receiverAppName)
                {
                    dir = ConnectivityInfo::INPUT;
                    remoteInfo = application_map_.lookup(senderAppName);
                }
                else
                    continue;; //We look at other connections to see if they are using the current app
                
                int iCommType;
                if (commType.length () == 0 || commType == "point-to-point")
                    iCommType = ConnectorInfo::POINTTOPOINT;
                else
                    iCommType = ConnectorInfo::COLLECTIVE;

                int iProcMethod;
                if (procMethod.length () == 0 || procMethod == "tree")
                    iProcMethod = ConnectorInfo::TREE;
                else
                    iProcMethod = ConnectorInfo::TABLE;

                //leader is always 0
                app->second.add (
                dir == ConnectivityInfo::OUTPUT ? senderPortName : receiverPortName,
                dir, width, receiverAppName, receiverPortName, portCode, 0,
                remoteInfo->nProc (), iCommType, iProcMethod);

            }
            else
                std::cout << "Keys must be a dictionary" << std::endl;

        }
        //std::cout << "" << std::endl;
        //std::cout << app->first << ": " << std::endl;
        //app->second.write(std::cout);
    }
}

void
MultiNetworkManager::writeEnv(std::ostream& env, int app_i)
{
    //std::ostringstream env;
    const ApplicationInfo& app = application_map_.appAt(app_i);

    env << app.name() << ':';
    application_map_.write_map (env);
    connectivityMap_[app.name()].write(env);
    // HERE YOU NEED TO WRITE THE DICT/SECTION SPESIFIC TO THE APP (NP,BINARY,ARGS)
    application_map_.write_app_dict(env, app_i);
    // now global DICt
    application_map_.write_global_dict(env);
} 
/*

std::string
MultiNetworkManager::dict_to_string()
{
    std::stringstream ss;

    for (auto it = application_map_->begin(); it != application_map_->end(); ++it)
    {
        std::string key  = it->first.toString();
        const Token& val = it->second;
        int nprocs = getValue<long>(val);

        ss << key << ":" << nprocs << ":";
    }

    return ss.str();
}
*/

} // namespace nest
