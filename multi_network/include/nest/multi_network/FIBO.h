/*
 *  FIBO.h
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


#ifndef FIBO_H
#define FIBO_H
#include "nest/multi_network/config.h"
#include <vector>

namespace nest {

  class FIBO {
  private:
    static const int nInitial_ = 10;
    
    std::vector<char> buffer_;
    int elementSize_;
    int size_;
    int current_;

    void grow (int newSize);
    
  public:
    FIBO () { }
    FIBO (int elementSize);
    void configure (int elementSize);
    bool isEmpty ();
    // NOTE: find better return type
    void* insert ();
    void insert (void* elements, int n_elements);
    void clear ();
    void nextBlockNoClear (void*& data, int& size);
    void nextBlock (void*& data, int& size);
    unsigned int size () { return size_; } //*fixme* should be n elements
  };
  
  
}
#endif /* FIBO.H */