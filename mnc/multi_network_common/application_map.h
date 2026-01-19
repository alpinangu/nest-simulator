
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

//#include <variant>
//#include <optional>

namespace nest
{

  //a: like a container (or even like a struct) that holds related data about the running application
  class ApplicationInfo
  {
    std::string name_;
    int nProc_;
    int color_; // The index of the app in the application map vector
    int leader_; // assigned -1 to indicate it has not been resolved yet.
    std::map<std::string, std::string> app_dict_;
 
  public:
    ApplicationInfo (std::string name, int n, int c, std::map<std::string, std::string> app_dict) :
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
    leader() const
    {
      return leader_;
    }

    int
    nProc() const
    {
      return nProc_;
    }

    void
    set_leader(int leader_rank)
    {
      leader_ = leader_rank;
    }

  const std::string*
  get_value(std::string const& key) const 
  {
      auto it = app_dict_.find(key);
      if (it == app_dict_.end())
          return nullptr;         // no such key

      return &it->second;         // take the address of the mapped std::string
  }



    void
    set_app_dict(std::string var_name, std::string var_value)
    {
      app_dict_[var_name] = var_value;
    }

  };


  //a: a vector that contains aplication info as its elements, so we can use vector functions
  class ApplicationMap
  {
    std::vector<ApplicationInfo> apps_;
    std::map<std::string, std::string> globaldict_;

  public:
    ApplicationMap ();

    //Inst color the actual index for the vector?
    //TODO: MKE THEM RETURN A REFERANCE NOT A POINTER
    ApplicationInfo* lookup (int color);
    ApplicationInfo* lookup(std::string name);
    bool get_variable(int color, std::string const& key, std::string* result);

    /*
          std::map<int, int>
    assignLeaders(std::string my_app_label);    
    */


    void add(std::string name, int n, int c, std::map<std::string, std::string> localdict);

    void add_global_dict(std::string var_name, std::string var_value);

    void add_local_dict (std::string app_name_, std::string var_name, std::string var_value);

    void write_map(std::ostream& out);

    void write (std::ostringstream& out);

    void write_app_dict(std::ostream& out, int i);

    void write_global_dict(std::ostream& out);

    ApplicationInfo& appAt(int i);

    int assign_app(int rank);

    void set_leaders();

    std::map<int, int> assignLeaders(std::string my_app_label);

    void read (std::istringstream& in);

    int nProcesses();

    int size() const
    { return apps_.size(); }

    std::vector<ApplicationInfo>::iterator begin() 
    { return apps_.begin(); }

    std::vector<ApplicationInfo>::iterator end()
    { return apps_.end(); }

    ApplicationInfo& operator[](int idx) 
    { return apps_[idx]; }

  };

}

#endif /* APPLICATION_MAP_HH */
