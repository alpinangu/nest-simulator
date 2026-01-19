/*
 *  This file is part of MUSIC.
 *  Copyright (C) 2007, 2008, 2009, 2012, 2021 INCF
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

#include <nest/multi_network/configuration.h> // Must be included first on BG/L

#include <nest/multi_network/debug.h>

extern "C" {
#include <stdlib.h>
}

#include <multi_network_common/ioutils.h>
#include <multi_network_common/error.h>
#include <iostream>
#include <fstream>

namespace nest 
{

  //a: This is the static constant string from Configuration class
  //a: It is the name of the environment variable that stores the config file adress
  const char* const Configuration::configEnvVarName = "_MUSIC_CONFIG_";
 // const char* const Configuration::mapFileName = "music.map";

  /*
   * This is the syntax of the _MUSIC_CONFIG_ variable:
   *
   * POSTPONE:COLOR means that configuration is postponed until the
   * first port is created.  At that time, the variable is expected to
   * contain the full configuration information:
   *
   *
   * APPLICATIONNAME:COLOR:APPLICATIONMAP:CONNECTIVITYMAP:CONFIGDICT
   *
   * APPLICATIONNAME = name of this application (string)
   *
   * COLOR = index of section in music configuration file
   *
   * APPLICATIONMAP = SIZE:...:NAMEk:NPROCk:...
   *
   * SIZE = number of applications (integer)
   *
   * NAMEk = name of application k
   *
   * NPROCk = number of processes in application k
   *
   * CONNECTIVITYMAP = SIZE:...:PORTNAMEk:DIRECTIONk:WIDTHk:CONNECTIONSk:...
   *
   * SIZE = number of ports of this application
   *
   * PORTNAMEk = port name
   *
   * DIRECTIONk = port direction, 0 = OUTPUT, 1 = INPUT
   *
   * WIDTHk = port width
   *
   * CONNECTIONSk = SIZE:...:
   *                RECAPPNAME:RECPORTNAME:RECPORTCODE:REMOTELEADER:NREMOTEPROCS
   *                :COMMTYPE:PROCMETHOD:...
   *
   * RECAPPNAME = name of receiving application
   *
   * RECPORTNAME = name of receiver port
   *
   * RECPORTCODE = code unique for every receiver port
   *
   * REMOTELEADER = lowest rank of the remote application
   *
   * NREMOTEPROCS = number of processes in the remote application
   *
   * COMMTYPE = communication algorithm, 0 = collective, 1 = point-to-point
   *
   * PROCMETHOD = processing method, 0 = tree, 1 = table
   *
   * CONFIGDICT = ...:VARNAMEk=VALUEk:...
   */

  Configuration::Configuration ()
  {
    init ();
  }

  Configuration::Configuration (std::string configStr)
  {
    init ();
    parse(configStr);
  }

  Configuration::~Configuration ()
  {
    delete connectivityMap_;
    delete applications_;
    if (defaultConfig_ != NULL)
      delete defaultConfig_;
  }


  void
  Configuration::init ()
  {
    applications_ = new ApplicationMap ();
    connectivityMap_ = new Connectivity ();
    defaultConfig_ = NULL;
  }

  void
  Configuration::parse(std::string configStr)
  {
    std::istringstream env (configStr);
    //a: sets the name of the application of this proccess, it is the first part of the env var
    setName (IOUtils::read (env));
    //a: deletes the :, delim
    env.ignore (); // delim
    // do not need color information,
    // but we can use for error check
    IOUtils::read (env);
    env.ignore (); // delim
    applications_->read (env);
    env.ignore ();
    //TODO: THIS LEADERS IS NOT NEEDED
    std::map<int, int> leaders = applications_->assignLeaders( Name ());
    connectivityMap_->read(env, leaders);
    // parse config string
    while (!env.eof ())
      {
        env.ignore ();
        std::string name = IOUtils::read(env, '=');
        env.ignore ();
        std::string v = IOUtils::read(env);
      //  std::cerr << name << " " << v << std::endl;
        insert (name, v );
      }

  }

  void
  Configuration::write (std::ostringstream& env, Configuration* mask)
  {
    std::map<std::string, std::string>::iterator pos;
    for (pos = dict_.begin (); pos != dict_.end (); ++pos)
    {
      std::string name = pos->first;
      if (!(mask && mask->lookup (name)))
      {
        env << ':' << name << '=';
        IOUtils::write (env, pos->second);
      }
    }
  }


  void
  Configuration::writeEnv ()
  {
    std::ostringstream env;
    //a:  We dont use this color
    env << Name () << ':' << Color () << ':';
    applications_->write (env);
    env << ':';
    connectivityMap_->write (env);
    write (env, 0);
    if (defaultConfig_ != NULL)
      defaultConfig_->write (env, this);
    setenv (configEnvVarName, env.str ().c_str (), 1);
  }


  bool
  Configuration::lookup(std::string name)
  {
    return dict_.find(name) != dict_.end();
  }

  bool
  Configuration::lookup(std::string name, std::string* result)
  {
    std::map<std::string, std::string>::iterator pos = dict_.find(name);
    if (pos == dict_.end())
      return defaultConfig_ && defaultConfig_->lookup(name, result);

    *result = pos->second;
    return true;
  }

  bool
  Configuration::lookup(std::string name, int* result)
  {
    std::map<std::string, std::string>::iterator pos = dict_.find(name);
    if (pos == dict_.end())
      return defaultConfig_ && defaultConfig_->lookup(name, result);

    std::istringstream iss(pos->second);
    if (! (iss >> *result).fail())
      return true;

    std::ostringstream oss;
    oss << "var " << name << " given wrong type (" << pos->second
	<< "; expected int) in config file";
    error(oss.str());
    return true; // Doesn't happen! Just for compiler!
  }

  // It looks up a configuration variable by name and tries to convert its value to a double.
  // Returns true if the variable was found and successfully converted to a doubl
  // If the variable exists but is not a valid double, it throws an error
  bool
  Configuration::lookup(std::string name, double* result)
  {
    std::map<std::string, std::string>::iterator pos = dict_.find(name);
    if (pos == dict_.end())
      return defaultConfig_ && defaultConfig_->lookup (name, result);

    std::istringstream iss(pos->second);
    if (! (iss >> *result).fail())
      return true;

    std::ostringstream oss;
    oss << "var " << name << " given wrong type (" << pos->second
	<< "; expected double) in config file";
    error(oss.str());
    return true; // Doesn't happen! Just for compiler!
  }

  bool
  Configuration::lookup (std::string name, bool* result)
  {
    std::map<std::string, std::string>::iterator pos = dict_.find (name);
    if (pos == dict_.end ())
      return defaultConfig_ && defaultConfig_->lookup (name, result);

    std::istringstream iss(pos->second);
    if (! (iss >> std::boolalpha >> *result).fail ())
      return true;

    std::ostringstream oss;
    oss << "var " << name << " given wrong type (" << pos->second
	<< "; expected bool) in config file";
    error(oss.str ());
    return true; // Doesn't happen! Just for compiler!
  }

  std::string
  Configuration::Name ()
  {
    return app_name_;
  }


  void
  Configuration::setName (std::string app_name)
  {
    app_name_ = app_name;
  }


  int
  Configuration::Color()
  {
    return applications_->lookup (app_name_)->color ();
  }


  int
  Configuration::Leader()
  {
    ApplicationInfo* info = applications_->lookup (app_name_);
    return info == 0 ? -1 : info->leader ();
  }


  void
  Configuration::insert (std::string name, std::string value)
  {

    dict_.insert (std::make_pair (name, value));
  }


  ApplicationMap*
  Configuration::applications ()
  {
    return applications_;
  }


  Configuration*
  Configuration::defaultConfig ()
  {
    if (defaultConfig_ == NULL)
      defaultConfig_ = new Configuration();
    return defaultConfig_;
  }


  Connectivity*
  Configuration::connectivityMap ()
  {
    return connectivityMap_;
  }


  const Configuration::ConfigDict &
  Configuration::getDict()
  {
    return dict_;
  }


  void
  Configuration::setDict(const Configuration::ConfigDict &dict)
  {
    dict_ = dict;

  }


  void
  Configuration::resetDict()
  {
    dict_ = Configuration::ConfigDict ();
  }

}
