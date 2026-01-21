/*
 *  This file is part of MUSIC.
 *  Copyright (C) 2007, 2008, 2009 INCF
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

#ifndef MN_CONFIGURATION_H
#define MN_CONFIGURATION_H

//#include "music/music-config.hh"

#include <string>
#include <map>

#include <multi_network_common/application_map.h>
#include <multi_network_common/connectivity.h>

namespace nest_mn 
{

  class Configuration {
  public:
      //a: Stores name of the environment variable that stores the config file adress
      //a: It hols: app_name_:colour_(probably rank, but we dont use it during setup):total_number_of_apps:application_map(w/o the coulour)
      //a: application map is the name of the app and its number of process, colour is their order in the env var
      static const char* const configEnvVarName;
      typedef std::map<std::string, std::string> ConfigDict;
  private:

    //a: holds this processes application name, during launch we put its name on the env var first
    std::string app_name_;

    ApplicationMap* applications_;
    Connectivity* connectivityMap_;

    //a: contains the information of non app section key-value pairs
    Configuration* defaultConfig_;

    //a: Here we store the key-value pairs from the parsed sections.
    std::map<std::string, std::string> dict_;


  public:

    Configuration ();
    Configuration (std::string configStr);

    ~Configuration ();

    void writeEnv ();

    bool lookup (std::string name);

    bool lookup (std::string name, int* result);

    bool lookup (std::string name, double* result);

    bool lookup(std::string name, std::string* result);

    bool lookup (std::string name, bool* result);

    void insert (std::string name, std::string value);

    const ConfigDict &getDict();

    void setDict(const ConfigDict &dict);

    void resetDict();

    std::string Name();

    void setName( std::string name);

    int Color ();

    int Leader ();

    ApplicationMap* applications ();

    Connectivity* connectivityMap ();

    Configuration* defaultConfig();

  private:

    void init ();

    void parse (std::string configStr);

    void write (std::ostringstream& env, Configuration* mask);

  };

}
#endif
