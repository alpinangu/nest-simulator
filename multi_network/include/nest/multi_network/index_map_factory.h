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

#ifndef MN_INDEX_MAP_FACTORY_H
#define MN_INDEX_MAP_FACTORY_H

#include <vector>

#include <nest/multi_network/index_map.h>

namespace nest_mn {

  class IndexMapFactory : public IndexMap {
    std::vector<IndexInterval> indices_;
  public:
    class iterator : public IndexMap::IteratorImplementation {
      const IndexInterval* intervalPtr;
    public:
      iterator (const IndexInterval* ptr) : intervalPtr (ptr) { }
      virtual const IndexInterval operator* () { return *intervalPtr; }
      virtual const IndexInterval* dereference () { return intervalPtr; }
      virtual bool isEqual (IteratorImplementation* i) const
      {
	return intervalPtr == static_cast<iterator*> (i)->intervalPtr;
      }
      virtual void operator++ () { ++intervalPtr; }
      virtual IteratorImplementation* copy ()
      {
	return new iterator (intervalPtr);
      }
    };
    
    IndexMapFactory ();
    IndexMapFactory (std::vector<IndexInterval>& indices);
    void add (int begin, int end, int local);
    void build ();
    virtual IndexMap::iterator begin ();
    virtual const IndexMap::iterator end () const;
    virtual IndexMap* copy ();    
  };

} // namespace

//#endif /* HAVE_MPI */
#endif /* MN_INDEX_MAP_FACTORY_H */
