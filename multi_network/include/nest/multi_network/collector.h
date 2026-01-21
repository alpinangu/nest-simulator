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

#ifndef MN_COLLECTOR_H
#define MN_COLLECTOR_H

//#include "nest/multi_network/config.h"


// data_map.hh needs to be included first since it includes mpi.h.
// mpi.h must be included before other header files on BG/L
#include <nest/multi_network/data_map.h>

#include <map>
#include <vector>

#include <nest/multi_network/BIFO.h>
#include <nest/multi_network/interval_tree.h>

namespace nest_mn {

  /**
   * The Collector is responsible for collecting data from a set of
   * input buffers in BufferMap and store them in the state variables
   * represented by a DataMap.
   */
  class Collector {
  public: //for BG compiler
    class Interval : public nest_mn::Interval {
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
    class IntervalCalculator : public IntervalTree<int, nest_mn::Interval, int>::Action {
      Interval& interval_;
      int elementSize_;
    public:
      IntervalCalculator (Interval& interval, int elementSize)
	: interval_ (interval), elementSize_ (elementSize) { };
      void operator() (int& offset);
    };
    
    typedef std::vector<Interval> Intervals;
    typedef std::map<BIFO*, Intervals> BufferMap;

    DataMap* dataMap;
    int maxsize_;
    BufferMap buffers;

    IntervalTree<int, nest_mn::Interval, int>* buildTree ();
  public:
    // caller manages deallocation but guarantees existence
    /* remedius
     * second argument int allowedBuffered was changed to int maxsize
     */
    void configure (DataMap* dmap, int maxsize);
    void initialize ();
    void addRoutingInterval (IndexInterval i, BIFO* b);
    void collect ();
    void collect (ContDataT* base);
  };
    
} // namespace

//#endif /* HAVE_MPI */
#endif /* MN_COLLECTOR_H */