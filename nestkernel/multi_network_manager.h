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

#ifndef MULTI_NETWORK_MANAGER_H
#define MULTI_NETWORK_MANAGER_H

// C++ includes:


// Includes from libnestutil:
#include "manager_interface.h"


// Includes from multi_network_common:
#include <multi_network_common/application_map.h>
#include <multi_network_common/connectivity.h>

// Includes from sli:
#include "tokenutils.h"



namespace nest
{


class MultiNetworkManager : public ManagerInterface
{
public:
  MultiNetworkManager();
  MultiNetworkManager(std::string configStr)
  {
    parse(configStr);
  }

  ~MultiNetworkManager() override;

  void initialize( const bool ) override;
  void finalize( const bool ) override;
  void set_status( const DictionaryDatum& ) override;
  void get_status( DictionaryDatum& ) override;
  
  void launch(const std::string exe, const long n);
  void set_application_map(const DictionaryDatum& dict);
  void set_connectivity_map(const DictionaryDatum& dict);

  int get_rank();
  void parse(std::string configStr);

  static const char* const configEnvVarName;


  nest_mn::ApplicationMap* applications();

  int Color();

  int Leader();

  bool lookup (std::string name, int* result);
  bool lookup (std::string name, double* result);
  bool lookup(std::string name, std::string* result);
  bool lookup (std::string name, bool* result);

  nest_mn::Connectivity*
  connectivityMap ()
  {
    return &connectivityMap_[app_name_];
  }


private:

  nest_mn::ApplicationMap application_map_;
  std::map<std::string, nest_mn::Connectivity> connectivityMap_;
  int app_color_;
  std::string app_name_;

  static bool isApplicationMapSet_;
  static bool isConnMapCalled_;
  DictionaryDatum ConnMapBackup_;

  void writeEnv();


  //std::string dict_to_string();
};



} // namespace

#endif /* MULTI_NETWORK_MANAGER_H */