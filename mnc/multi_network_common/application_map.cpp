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

#include <multi_network_common/application_map.h>

// C++ includes:
#include <mpi.h>
#include <map>
#include <string>

//Includes from multi_network
#include <multi_network_common/ioutils.h>
//#include "nest/multi_network/mpi_utils.h"

namespace nest
{

void
ApplicationInfo::write_app_dict(std::ostream& out)
{
  for (auto pos = app_dict_.begin (); pos != app_dict_.end (); ++pos)
  {
    std::string name = pos->first;
    out << ':' << name << '=';
    auto val = &pos->second;
    std::string value = *val;
    out << value;
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

bool
ApplicationMap::get_variable(int color, std::string const& key, std::string* result)
{
    ApplicationInfo* app_info = lookup(color);
    if (!app_info) 
        return false;   // no such app

    // Try the app‐specific dictionary
    if (auto opt = app_info->get_value(key)) 
    {
        // must be a string
          *result = *opt;
          return true;
    }

    // Fallback to global map
    auto it = globaldict_.find(key);
    if (it == globaldict_.end())
        return false;   // no such key anywhere

    if (auto pstr = &it->second) 
    {
        *result = *pstr;
        return true;
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
ApplicationMap::add (std::string name, int n, int c, std::map<std::string, std::string> localdict)
{
  apps_.push_back (ApplicationInfo (name, n, c, localdict));
}

void 
ApplicationMap::add_global_dict (std::string var_name, std::string var_value)
{
  globaldict_[var_name] = var_value;
}

void 
ApplicationMap::add_local_dict (std::string app_name_, std::string var_name, std::string var_value)
{
  lookup(app_name_)->set_app_dict(var_name, var_value);
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
  ApplicationMap::write (std::ostringstream& out)
  {
    //a: gives number of applications in the map,
	  out << size ();
	  for(auto it = apps_.begin(); it != apps_.end(); ++it)
	  {
		  out << ':';
      //a: itterators points to the elements of the Map, which are application infos
      //a: we print the names of the applications following the number of processes they use
		  IOUtils::write (out, it->name ());
		  out << ':' << it->nProc ();
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
    auto val = &pos->second;
    std::string value = *val;
    out << value;
  }
}


ApplicationInfo&
ApplicationMap::appAt (int i)
{
  return apps_[i];
}

int 
ApplicationMap::assign_app(int rank)
{
    // Compute total number of ranks available
    int totalProcs = 0;
    for (auto const& app : apps_) {
        totalProcs += app.nProc();
    }

    // Validate that the MPI rank is in [0, totalProcs)
    if (rank < 0 || rank > totalProcs) {
        //GIVE ERROR
        exit(111);
    }

    // Find which app owns this rank
    int cumulative = 0;
    for (auto const& app : apps_) {
        int nextBoundary = cumulative + app.nProc();
        // rank ∈ [cumulative, nextBoundary)
        if (rank < nextBoundary) {
            return app.color();
        }
        cumulative = nextBoundary;
    }

    // Should never get here
    //throw std::runtime_error("Failed to map rank to any application");
}

void 
ApplicationMap::set_leaders()
{
    int offset = 0;                 // first rank in the current app block
    for (auto& app : apps_) {
      app.set_leader(offset);          
      offset += app.nProc();      
    }
    // TODO: my_color not found ⇒ programming error
}

void
ApplicationMap::read (std::istringstream& in)
{
  //a: The stream contains the number of applications first.
  int nApp;
  in >> nApp;

  for (int i = 0; i < nApp; ++i)
    {
      in.ignore ();
      //a: reads the app name
      std::string name = IOUtils::read (in);
      in.ignore ();
      //a: The next integer from the stream is the number of processes (np) this application is assigned.
      int np;
      in >> np;

      //TODO: need to add an updater function to populate this later.
      std::map<std::string, std::string> localdict;

      add (name, np, i, localdict);
    }

}

int ApplicationMap::nProcesses()
{
  int n = 0;
  for ( auto it = begin(); it != end(); ++it )
    n += it->nProc();
  return n;
}


//a: my_app_label is the app name
//a: Yes, exactly—the returned std::map<int,int> leaders has one entry for each distinct application color, where:
//a: key = application color
//a: value = the MPI rank of that color’s leader (i.e. the first rank in that color’s block)
//a: EACH COLOR REPRESENTS AN APPLICATION
  std::map<int, int>
  ApplicationMap::assignLeaders (std::string my_app_label)
  {
    //a:This snippet is setting up and performing a small all-to-all exchange so that every MPI rank ends up with a 
    //a: complete table of “colors” (i.e. application IDs) for all ranks:
    //---------
    std::map<int, int> leaders;

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);

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
            info->set_leader(i);
            leaders[info->color ()] = i;
            prev_color = colors[i];
          }
      }

#ifdef MUSIC_DEBUG
    /*  block for debugging */
    std::ofstream outfile ("ranks");
    for(int i = 0; i < nLeaders; ++i)
      {
        outfile<< i <<":";
        for(int j = 0; j < gsize; ++j)
        if(rbuf[j] == i) outfile<< " " << j;
        outfile<<std::endl;
      }
    outfile<<std::endl;
    outfile.close();
    /*end of block */
#endif // MUSIC_DEBUG
    delete[] colors;
    return leaders;
  }

}