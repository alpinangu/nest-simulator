/*
 *  This file is part of MUSIC.
 *  Copyright (C) 2009 INCF
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


#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H


//#include "nest/multi_network/config.h"


// data_map.hh needs to be included first since it includes mpi.h.
// mpi.h must be included before other header files on BG/L
#include <nest/multi_network/data_map.h>

#include <map>
#include <vector>

#include <nest/multi_network/FIBO.h>
#include <nest/multi_network/interval_tree.h>

namespace nest {

  /**
   * The Distributor distributes data from state variables represented
   * by a DataMap ro a set of output buffers in BufferMap.
   */
  class Distributor {
  public: //for BG compiler
    class Interval : public nest::Interval {
    public:
      Interval (IndexInterval& interval);
      bool operator< (const Interval& ref) const
      { return begin () < ref.begin (); }
      // length field is stored overlapping the end field so that the
      // interval information can be "recompiled" for space and time
      // efficiency
      int length () const { return end (); }
      void setLength (int length) { setEnd (length); }
    };
private:
    class IntervalCalculator : public IntervalTree<int, nest::Interval, int>::Action {
      Interval& interval_;
      int elementSize_;
    public:
      IntervalCalculator (Interval& interval, int elementSize): interval_ (interval), elementSize_ (elementSize) { };
      void operator() (int& offset);
    };
    
    typedef std::vector<Interval> Intervals;
    typedef std::map<FIBO*, Intervals> BufferMap;

    DataMap* dataMap;
    BufferMap buffers;

    IntervalTree<int, nest::Interval, int>* buildTree ();
  public:
    // caller manages deallocation but guarantees existence
    void configure (DataMap* dmap);
    void initialize ();
    void addRoutingInterval (IndexInterval i, FIBO* b);
    void distribute ();
  };
    
} // namespace

//#endif /* HAVE_MPI */
#endif /* DISTRIBUTOR_H */
