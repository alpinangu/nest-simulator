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
#include <mpi.h>
#include <unistd.h>
#include <variant>

// Includes from libnestutil:
#include "error.h"

// Includes from nestkernel:
#include "connectivity_map.h"
#include "token.h" 

//Includes from multi_network
#include "ioutils.h"


namespace nest
{

const char* const MultiNetworkManager::configEnvVarName = "_MUSIC_CONFIG_";

bool MultiNetworkManager::isApplicationMapSet_ = false;
bool MultiNetworkManager::isConnMapCalled_ = false;

struct Argv 
{
    std::vector<std::string> strings;
    std::vector<char*> ptrs;

    Argv(const std::string& cmdline) 
    {
        std::istringstream iss(cmdline);
        std::string token;
        while (iss >> token)
        {
            strings.push_back(std::move(token));
        }
        ptrs.reserve(strings.size()+1);
        for (auto &s : strings) 
        {
            ptrs.push_back(s.data());
        }
        ptrs.push_back(nullptr);
    }

    char* const* argv() const { return ptrs.data(); }
};

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
    std::string binary;
    application_map_.get_variable(app_color_, "binary", &binary);

    writeEnv();

    std::string wd;
    if (application_map_.get_variable(app_color_, "wd", &wd))
    {
        if (chdir (wd.c_str ()))
        {
            //TODO: Howw should I handle this?
            std::cerr << "Error during launching of: " << binary << std::endl;
            perror ("MULTI-NETWWORK");
            exit (1);
        }
    }

    std::string argv;
    application_map_.get_variable(app_color_, "argv", &argv);

    if(binary == "python3")
    {
        std::string cmd = binary + " " + argv;   // "python3 app2.py"
        Argv a(cmd);

        execvp(binary.c_str(), a.argv());
    }
    else
    {
        Argv a(argv);
        execvp(binary.c_str(), a.argv());
    }

    perror("execvp failed");
    std::exit(EXIT_FAILURE);
    

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
        "argv": "arg1 arg1" SHOULD THERE BE A DELIMETER??
        "wd": "working directory" (apps do not have to be in the same directory)
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

    int appcounter = 0;
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

            int np = -1;
            std::map<std::string, std::string> app_dict;
            for (auto  ti = subdict->begin(); ti != subdict->end(); ++ti)
            {
                std::string key  = ti->first.toString();
                Token& value = ti->second;

                // here I might use #include "name.h", it says it is faster?

                if(key == "np")
                    np = getValue<long>(value);
                else
                {
                    if (value.is_a<std::string>())
                    {
                        app_dict[key] = getValue<std::string>(value);
                    }
                    else
                        //TODO: handle case
                        std::cout << "Give the variables as a string" << std::endl;
                }
                    

            }

            if(np == -1)
            {   
                //TODO: HANDLE THIS WITH A LOG
                exit(111);
            }
            //??????
            //should we add binary to the map to or assign color to binaries in a different map?
            std::cout << "Application " << app_name << " with " << np << " has been added as color: " << appcounter << std::endl;
            application_map_.add(app_name, np, appcounter, app_dict);

            appcounter++;
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
                //TODO: handle case
                std::cout << "Give the variables as a string" << std::endl;
            }
        }

    }

    int rank = get_rank();
    app_color_ = application_map_.assign_app(rank);

    application_map_.set_leaders();

    isApplicationMapSet_ = true;
    if(isConnMapCalled_ == true)
    {
        set_connectivity_map(ConnMapBackup_);
    }
    /*
    writeEnv(std::cout, 0);
    std::cout << "" << std::endl;
    writeEnv(std::cout, 1);
    std::cout << "" << std::endl;
    writeEnv(std::cout, 2);
    */

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
                dir, width, receiverAppName, receiverPortName, portCode, remoteInfo->leader(),
                remoteInfo->nProc (), iCommType, iProcMethod);

            }
            else
                std::cout << "Keys must be a dictionary" << std::endl;

        }
    }
}

void
MultiNetworkManager::writeEnv()
{
    //std::ostringstream env;
    const ApplicationInfo& app = application_map_.appAt(app_color_);
    std::ostringstream env;

    env << app.name() << ':'<< app.color() << ':';
    application_map_.write_map (env);
    connectivityMap_[app.name()].write(env);
    application_map_.write_app_dict(env, app_color_);
    application_map_.write_global_dict(env);

    setenv (configEnvVarName, env.str().c_str(), 1);

    //just for debugging:
    if(get_rank() == app.leader())
    {
        std::cout << "From the applicaton with color " << app_color_ << "/" << app.color() << "with rank(leader) " << app.leader() << std::endl;
        std::cout << env.str() << std::endl;

    }
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

int
MultiNetworkManager::get_rank()
{
    char* vpid = getenv ("OMPI_MCA_ns_nds_vpid");
    if (vpid == NULL)
    vpid = getenv ("OMPI_COMM_WORLD_RANK");
    if (vpid == NULL)
    return -1;
    std::istringstream iss (vpid);
    int rank;
    iss >> rank;
    return rank;
}

void
MultiNetworkManager::parse(std::string configStr)
{
    std::istringstream env (configStr);
    //a: sets the name of the application of this proccess, it is the first part of the env var
    app_name_ = IOUtils::read (env);
    //a: deletes the :, delim
    env.ignore (); // delim
    // do not need color information,
    // but we can use for error check
    app_color_ = std::stoi(IOUtils::read (env));
    env.ignore (); // delim
    application_map_.read (env);
    env.ignore ();
    application_map_.set_leaders();
    connectivityMap_[app_name_].read(env);
    // parse config string
    while (!env.eof ())
    {
        env.ignore ();
        std::string var_name = IOUtils::read (env, '=');
        env.ignore ();
        std::string var_value = IOUtils::read (env);
        //  std::cerr << name << " " << v << std::endl;
        application_map_.add_local_dict(app_name_, var_name, var_value);
    }

}



ApplicationMap*
MultiNetworkManager::applications ()
{
    return &application_map_;
}

int
MultiNetworkManager::Color()
{
    return application_map_.lookup (app_name_)->color ();
}


int
MultiNetworkManager::Leader()
{
ApplicationInfo* info = application_map_.lookup (app_name_);
    return info == 0 ? -1 : info->leader ();
}

bool
MultiNetworkManager::lookup(std::string name, std::string* result)
{
    return application_map_.get_variable(app_color_, name, result);
}

bool
MultiNetworkManager::lookup(std::string name, int* result)
{
    std::string* temp;
    application_map_.get_variable(app_color_, name, temp);

    std::istringstream iss(*temp);
    if (! (iss >> *result).fail())
      return true;

    std::ostringstream oss;
    oss << "var " << name << " given wrong type (" << *temp
	<< "; expected int) in config file";
    error(oss.str());
    return true; // Doesn't happen! Just for compiler!


}

// It looks up a configuration variable by name and tries to convert its value to a double.
// Returns true if the variable was found and successfully converted to a doubl
// If the variable exists but is not a valid double, it throws an error
bool
MultiNetworkManager::lookup(std::string name, double* result)
{
    std::string* temp;
    application_map_.get_variable(app_color_, name, temp);

    std::istringstream iss(*temp);
    if (! (iss >> *result).fail())
        return true;


    std::ostringstream oss;
    oss << "var " << name << " given wrong type (" << *temp
    << "; expected double) in config file";
    error(oss.str());
    return true; // Doesn't happen! Just for compiler!
}

bool
MultiNetworkManager::lookup (std::string name, bool* result)
{
    std::string* temp;
    application_map_.get_variable(app_color_, name, temp);

    std::istringstream iss(*temp);
    if (! (iss >> *result).fail())
        return true;

    std::ostringstream oss;
    oss << "var " << name << " given wrong type (" << *temp
    << "; expected bool) in config file";
    error(oss.str ());
    return true; // Doesn't happen! Just for compiler!
}



} // namespace nest
