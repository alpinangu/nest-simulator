/*
 *  application_map.cpp
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

#include "application_map.h"

// C++ includes:
#include <mpi.h>
#include <map>
#include <string>

namespace nest
{

void
ApplicationInfo::write_app_dict(std::ostream& out)
{
  for (auto pos = app_dict_.begin (); pos != app_dict_.end (); ++pos)
  {
    std::string name = pos->first;
    out << ':' << name << '=';
    if (auto val = std::get_if<std::string>(&pos->second))
    {
      std::string value = *val;
      out << value;
    }
    else if (auto val = std::get_if<int>(&pos->second))
    {
      int value = *val;
      out << value;
    }
  }
}
////////////////////////////
ApplicationMap::ApplicationMap()
{
}

ApplicationInfo*
ApplicationMap::lookup (int color)
{
  for(auto i = apps_.begin (); i != apps_.end (); ++i)
    {
      if (i->color() == color)
        return &*i;
    }
  return 0;

}

ApplicationInfo*
ApplicationMap::lookup(std::string name)
{
  for(auto it = apps_.begin(); it != apps_.end(); it++)
  {
    if(it->name() == name)
      return &*it;
  }
}

/*
//my_app_label is the app name
//the returned std::map<int,int> leaders has one entry for each distinct application color, where:
//key = application color
//value = the MPI rank of that color’s leader (i.e. the first rank in that color’s block)
//EACH COLOR REPRESENTS AN APPLICATION
std::map<int, int>
ApplicationMap::assignLeaders (std::string my_app_label)
{
//This snippet is setting up and performing a small all-to-all exchange so that every MPI rank ends up with a 
//complete table of “colors” (i.e. application IDs) for all ranks:
//---------
std::map<int, int> leaders;

int size;
MPI_Comm_size (MPI_COMM_WORLD, &size);

int rank;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
int *colors = new int[size];
colors[rank] = lookup (my_app_label)->color ();
MPI_Allgather (MPI_IN_PLACE, 0, MPI_INT, colors, 1, MPI_INT, MPI_COMM_WORLD);
//----------

//a:  every time you cross into a new application color as you walk through the rank list, 
//a: you mark that first rank of the block as the leader for that application.
int prev_color = -1;
for (int i = 0; i < size; ++i)
{
    if (colors[i] != prev_color)
    {
        ApplicationInfo* info = lookup (colors[i]);
        info->setLeader (i);
        leaders[info->color ()] = i;
        prev_color = colors[i];
    }
}

}
*/

void
ApplicationMap::add (std::string name, int n, int c, std::map<std::string, std::variant<int, std::string, double>> localdict)
{
  apps_.push_back (ApplicationInfo (name, n, c, localdict));
}

void 
ApplicationMap::add_global_dict (std::string var_name, std::variant<int, std::string, double> var_value)
{
  globaldict_[var_name] = var_value;
}

void
ApplicationMap::write_map (std::ostream& out)
{
  out << apps_.size();
  for(auto it = apps_.begin(); it != apps_.end(); ++it)
  {
    out << ':';
    out << it->name();
    out << ':' << it->nProc();
  }
}

void
ApplicationMap::write_app_dict(std::ostream& out, int i)
{
  apps_[i].write_app_dict(out);
}

void
ApplicationMap::write_global_dict(std::ostream& out)
{
  for (auto pos = globaldict_.begin (); pos != globaldict_.end (); ++pos)
  {
    std::string name = pos->first;
    out << ':' << name << '=';
    if (auto val = std::get_if<std::string>(&pos->second))
    {
      std::string value = *val;
      out << value;
    }
    else if (auto val = std::get_if<int>(&pos->second))
    {
      int value = *val;
      out << value;
    }
  }
}
ApplicationInfo&
ApplicationMap::appAt (int i)
{
  return apps_[i];
}

}