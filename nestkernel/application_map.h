
/*
 *  application_map.h
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

#ifndef APPLICATION_MAP_HH
#define APPLICATION_MAP_HH

#include <sstream>
#include <vector>
#include <map>
namespace nest
{

  //a: like a container (or even like a struct) that holds related data about the running application
  class ApplicationInfo
  {
    std::string name_;
    int nProc_;
    int color_; //a: represents the section number that we assigned ot the application
    int leader_; //a: probably wont need it since we will spawn mpi proccesses in different communicators
    std::map<std::string, std::variant<int, std::string, double>> app_dict_;
 
  public:
    ApplicationInfo (std::string name, int n, int c, std::map<std::string, std::variant<int, std::string, double>> app_dict) :
        name_ (name), nProc_ (n), color_ (c), leader_(-1), app_dict_(app_dict)
    {
    }

    void write_app_dict(std::ostream& out);

    std::string
    name() const
    {
      return name_;
    }


    int
    color() const
    {
      return color_;
    }


    int
    nProc() const
    {
      return nProc_;
    }

  };


  //a: a vector that contains aplication info as its elements, so we can use vector functions
  class ApplicationMap
  {
    std::vector<ApplicationInfo> apps_;
    std::map<std::string, std::variant<int, std::string, double>> globaldict_;

  public:
    ApplicationMap ();

    ApplicationInfo* lookup (int color);
    ApplicationInfo* lookup(std::string name);

    std::map<int, int>
    assignLeaders(std::string my_app_label);

    void add(std::string name, int n, int c, std::map<std::string, std::variant<int, std::string, double>> localdict);

    void add_global_dict(std::string var_name, std::variant<int, std::string, double> var_value);

    void write_map(std::ostream& out);

    void write_app_dict(std::ostream& out, int i);

    void write_global_dict(std::ostream& out);

    ApplicationInfo& appAt(int i);

    std::vector<ApplicationInfo>::iterator begin() 
    { return apps_.begin(); }

    std::vector<ApplicationInfo>::iterator end()
    { return apps_.end(); }

  };

}

#endif /* APPLICATION_MAP_HH */
