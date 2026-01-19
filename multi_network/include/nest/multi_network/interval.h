/*
 *  interval.h
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



#ifndef INTERVAL_H
#define INTERVAL_H

#include <ostream>

namespace nest {

  class Interval {
    int begin_;
    int end_;
  public:
    Interval () { }
    Interval (int b, int e) : begin_ (b), end_ (e) { }
    ~Interval () { }
    int begin () const { return begin_; }
    int end () const { return end_; }
    void setBegin (int begin) { begin_ = begin; }
    void setEnd (int end) { end_ = end; }
    bool operator< (const Interval& data) const
    {
      return begin () < data.begin ();
    }

  };

  // support printed representation
  inline std::ostream&
  operator<< (std::ostream& os, const Interval& ival)
  {
    os << "[" << ival.begin () << ", " << ival.end () << ")";
    return os;
  }

} // namespace

#endif /* INTERVAL.H */